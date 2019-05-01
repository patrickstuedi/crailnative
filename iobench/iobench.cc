/*
 * CppCrail: Native Crail
 *
 * Author: Patrick Stuedi  <stu@zurich.ibm.com>
 *
 * Copyright (C) 2015-2018, IBM Corporation
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "iobench/iobench.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>

#include "crail/client/crail_file.h"
#include "crail/client/utils/micro_clock.h"

using namespace std;

enum class Operation {
  Undefined = 0,
  GetFile = 1,
  CopyFromLocal = 2,
  CopyToLocal = 3,
  Write = 4,
  Read = 5,
  PutKey = 6,
  GetKey = 7,
  PutBenchmark = 8,
  GetBenchmark = 9,
  Cat = 10
};

struct Settings {
  string address;
  int port;
  Operation operation;
  string filename;
  int loop;
  int size;
  string dstfile;
  bool enumerable;
};

Operation getOperation(string name) {
  if (name == "GetFile") {
    return Operation::GetFile;
  } else if (name == "CopyFromLocal") {
    return Operation::CopyFromLocal;
  } else if (name == "CopyToLocal") {
    return Operation::CopyToLocal;
  } else if (name == "Write") {
    return Operation::Write;
  } else if (name == "Read") {
    return Operation::Read;
  } else if (name == "PutKey") {
    return Operation::PutKey;
  } else if (name == "GetKey") {
    return Operation::GetKey;
  } else if (name == "PutBenchmark") {
    return Operation::PutBenchmark;
  } else if (name == "GetBenchmark") {
    return Operation::GetBenchmark;
  } else if (name == "Cat") {
    return Operation::Cat;
  } else {
    return Operation::Undefined;
  }
}

void printSettings(Settings &settings) {
  cout << "Settings, address " << settings.address << ", port " << settings.port
       << ", operation " << static_cast<int>(settings.operation)
       << ", filename " << settings.filename << ", loop " << settings.loop
       << ", size " << settings.size << ", dstfile " << settings.dstfile
       << ", enumerable " << settings.enumerable << endl;
}

void setDefaults(Settings &settings) {
  settings.address = "127.0.0.1";
  settings.port = 9060;
  settings.operation = Operation::Undefined;
  settings.filename = "/tmp";
  settings.loop = 1;
  settings.size = 1024;
  settings.dstfile = "/dst";
  settings.enumerable = false;
}

void usage() {
  cout << "iobench -t "
          "[\"GetFile|CopyFromLocal|CopyToLocal|Write|Read|PutKey|GetKey|"
          "PutBenchmark|GetBenchmark|Cat\"]"
       << endl;
  cout << "\t-f <filename> -k <loop> -s <size> -a <IP address> -p <port> -d "
          "<destination file> -e"
       << endl;
}

Iobench::Iobench(string address, int port) { crail_.Initialize(address, port); }

Iobench::~Iobench() {}

int Iobench::GetFile(string &filename, const int loop) {
  MicroClock clock;
  clock.Start();
  for (int i = 0; i < loop; i++) {
    auto node = crail_.Lookup<CrailNode>(filename);
  }
  clock.Stop();
  auto micros = clock.Duration() / loop;
  cout << "latency [us/op] " << micros << endl;
  return 0;
}

int Iobench::CopyFromLocal(string local_file, string dst_file,
                           bool enumerable) {
  FILE *fp = fopen(local_file.c_str(), "r");
  if (!fp) {
    cout << "could not open local file " << local_file.c_str() << endl;
    return -1;
  }

  auto file = crail_.Create<CrailFile>(dst_file, 0, 0, enumerable).get();
  if (!file.valid()) {
    cout << "create node failed" << endl;
    return -1;
  }

  unique_ptr<CrailOutputstream> outputstream = file.outputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);
  while (size_t len = fread(buf->get_bytes(), 1, buf->remaining(), fp)) {
    buf->set_position(buf->position() + len);
    if (buf->remaining() > 0) {
      continue;
    }

    buf->Flip();
    while (buf->remaining() > 0) {
      if (outputstream->Write(buf).get() < 0) {
        return -1;
      }
    }
    buf->Clear();
  }

  if (buf->position() > 0) {
    buf->Flip();
    while (buf->remaining() > 0) {
      if (outputstream->Write(buf).get() < 0) {
        return -1;
      }
    }
  }

  fclose(fp);
  outputstream->Close().get();

  return 0;
}

int Iobench::CopyToLocal(string src_file, string local_file) {
  CrailFile file = crail_.Lookup<CrailFile>(src_file).get();
  if (!file.valid()) {
    cout << "lookup node failed" << endl;
    return -1;
  }

  FILE *fp = fopen(local_file.c_str(), "w");
  if (!fp) {
    cout << "could not open local file " << local_file.c_str() << endl;
    return -1;
  }

  unique_ptr<CrailInputstream> inputstream = file.inputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);
  while (inputstream->Read(buf).get() > 0) {
    buf->Flip();
    while (buf->remaining()) {
      if (size_t len = fwrite(buf->get_bytes(), 1, buf->remaining(), fp)) {
        buf->set_position(buf->position() + len);
      } else {
        break;
      }
    }
    buf->Clear();
  }
  fclose(fp);
  inputstream->Close();

  return 0;
}

int Iobench::Cat(string src_file) {
  CrailFile file = crail_.Lookup<CrailFile>(src_file).get();
  if (!file.valid()) {
    cout << "lookup node failed" << endl;
    return -1;
  }

  unique_ptr<CrailInputstream> inputstream = file.inputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(kBufferSize);
  while (inputstream->Read(buf).get() > 0) {
    buf->Flip();
    std::cout.write(reinterpret_cast<const char *>(buf->get_bytes()),
                    buf->remaining());
  }
  inputstream->Close();

  return 0;
}

int Iobench::Write(string dst_file, int len, int loop) {
  MicroClock clock;
  clock.Start();

  auto file = crail_.Create<CrailFile>(dst_file, 0, 0, true).get();
  if (!file.valid()) {
    cout << "create node failed" << endl;
    return -1;
  }

  unique_ptr<CrailOutputstream> outputstream = file.outputstream();

  char data[len];
  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(len);
  buf->PutBytes(data, len);

  for (int i = 0; i < loop; i++) {
    buf->Clear();
    while (buf->remaining() > 0) {
      if (outputstream->Write(buf).get() < 0) {
        cout << "error while writing " << endl;
        break;
      }
    }
  }
  outputstream->Close().get();

  clock.Stop();

  double _len = double(len);
  double _loop = double(loop);
  double _totalBytes = _len * _loop;
  double _totalGBytes = _totalBytes / 1000000000;
  double _totalSecs = clock.Duration() / 1000000;
  double _throughput = _totalGBytes * 8 / _totalSecs;

  cout << "data: " << _totalGBytes << " [GBs], time: " << _totalSecs
       << " [sec], throughput " << _throughput << " [Gb/s]" << endl;

  return 0;
}

int Iobench::Read(string src_file, int len, int loop) {
  MicroClock clock;
  clock.Start();

  CrailFile file = crail_.Lookup<CrailFile>(src_file).get();
  if (!file.valid()) {
    cout << "lookup node failed" << endl;
    return -1;
  }

  unique_ptr<CrailInputstream> inputstream = file.inputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(len);

  for (int i = 0; i < loop; i++) {
    buf->Clear();
    while (buf->remaining()) {
      if (inputstream->Read(buf).get() < 0) {
        return -1;
      }
    }
  }
  inputstream->Close().get();

  clock.Stop();

  double _len = double(len);
  double _loop = double(loop);
  double _totalBytes = _len * _loop;
  double _totalGBytes = _totalBytes / 1000000000;
  double _totalSecs = clock.Duration() / 1000000;
  double _throughput = _totalGBytes * 8 / _totalSecs;

  cout << "data: " << _totalGBytes << " [GBs], time: " << _totalSecs
       << " [sec], throughput " << _throughput << " [Gb/s]" << endl;

  return 0;
}

int Iobench::PutKey(const char data[], int len, string dst_file,
                    bool enumerable) {
  auto file = crail_.Create<CrailFile>(dst_file, 0, 0, enumerable).get();
  if (!file.valid()) {
    cout << "create node failed" << endl;
    return -1;
  }

  unique_ptr<CrailOutputstream> outputstream = file.outputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(len);
  buf->PutBytes(data, len);
  buf->Flip();
  while (buf->remaining() > 0) {
    if (outputstream->Write(buf).get() < 0) {
      return -1;
    }
  }
  outputstream->Close().get();

  return 0;
}

int Iobench::GetKey(char data[], int len, string src_file) {
  CrailFile file = crail_.Lookup<CrailFile>(src_file).get();
  if (!file.valid()) {
    cout << "lookup node failed" << endl;
    return -1;
  }

  unique_ptr<CrailInputstream> inputstream = file.inputstream();

  shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(len);
  while (buf->remaining()) {
    if (inputstream->Read(buf).get() < 0) {
      return -1;
    }
  }
  buf->Clear();
  memcpy(data, buf->get_bytes(), len);

  inputstream->Close().get();

  return 0;
}

int main(int argc, char *argv[]) {
  Settings settings;
  setDefaults(settings);

  int opt = 0;
  while ((opt = getopt(argc, argv, "t:f:k:s:a:p:d:e")) != -1) {
    switch (opt) {
    case 't':
      settings.operation = getOperation(optarg);
      break;
    case 'f':
      settings.filename = optarg;
      break;
    case 'k':
      settings.loop = atoi(optarg);
      break;
    case 's':
      settings.size = atoi(optarg);
      break;
    case 'a':
      settings.address = optarg;
      break;
    case 'p':
      settings.port = atoi(optarg);
      break;
    case 'd':
      settings.dstfile = optarg;
    case 'e':
      settings.enumerable = true;
      break;
    }
  }

  if (settings.operation == Operation::Undefined) {
    cout << "Operation undefined " << endl;
    usage();
    return -1;
  }

  printSettings(settings);

  Iobench iobench(settings.address, settings.port);

  int res = -1;
  if (settings.operation == Operation::GetFile) {
    res = iobench.GetFile(settings.filename, settings.loop);
  } else if (settings.operation == Operation::CopyFromLocal) {
    res = iobench.CopyFromLocal(settings.filename, settings.dstfile, true);
  } else if (settings.operation == Operation::CopyToLocal) {
    res = iobench.CopyToLocal(settings.filename, settings.dstfile);
  } else if (settings.operation == Operation::Write) {
    res = iobench.Write(settings.filename, settings.size, settings.loop);
  } else if (settings.operation == Operation::Read) {
    res = iobench.Read(settings.filename, settings.size, settings.loop);
  } else if (settings.operation == Operation::PutKey) {
    char data[settings.size];
    iobench.PutKey(data, settings.size, settings.filename, settings.enumerable);
  } else if (settings.operation == Operation::GetKey) {
    char data[settings.size];
    iobench.GetKey(data, settings.size, settings.filename);
  } else if (settings.operation == Operation::PutBenchmark ||
             settings.operation == Operation::GetBenchmark) {
    vector<string> names;
    for (int i = 0; i < settings.loop; i++) {
      string tmp = settings.filename;
      tmp.append(to_string(i));
      names.push_back(tmp);
    }

    char data[settings.size];
    MicroClock clock;
    clock.Start();
    for (string n : names) {
      if (settings.operation == Operation::PutBenchmark) {
        res = iobench.PutKey(data, settings.size, n, settings.enumerable);
      } else if (settings.operation == Operation::GetBenchmark) {
        res = iobench.GetKey(data, settings.size, n);
      } else {
        cout << "No valid operation defined " << endl;
        break;
      }
      if (res < 0) {
        cout << "Operation failed " << endl;
        break;
      }
    }
    clock.Stop();

    double _latency = clock.Duration() / settings.loop;
    cout << "Latency " << _latency << "[us/op]" << endl;
  } else if (settings.operation == Operation::Cat) {
    res = iobench.Cat(settings.filename);
  }
  return res;
}
