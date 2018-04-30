/*
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

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

#include "pocket_dispatcher.h"

using namespace std;

enum class Operation {
  Undefined = 0,
  MakeDir = 1,
  Lookup = 2,
  Enumerate = 3,
  Put = 4,
  Get = 5,
  DeleteDir = 6,
  DeleteFile = 7,
  Ioctl = 8
};

struct Settings {
  string address;
  int port;
  Operation operation;
  string filename;
  int loop;
  int size;
  string dstfile;
};

Operation getOperation(string name) {
  if (name == "MakeDir") {
    return Operation::MakeDir;
  } else if (name == "Lookup") {
    return Operation::Lookup;
  } else if (name == "Enumerate") {
    return Operation::Enumerate;
  } else if (name == "Put") {
    return Operation::Put;
  } else if (name == "Get") {
    return Operation::Get;
  } else if (name == "DeleteDir") {
    return Operation::DeleteDir;
  } else if (name == "DeleteFile") {
    return Operation::DeleteFile;
  } else if (name == "Ioctl") {
    return Operation::Ioctl;
  } else {
    return Operation::Undefined;
  }
}

void printSettings(Settings &settings) {
  cout << "Settings, address " << settings.address << ", port " << settings.port
       << ", operation " << static_cast<int>(settings.operation)
       << ", filename " << settings.filename << ", loop " << settings.loop
       << ", size " << settings.size << ", dstfile " << settings.dstfile
       << endl;
}

int main(int argc, char *argv[]) {
  Settings settings;

  int opt = 0;
  while ((opt = getopt(argc, argv, "t:f:k:s:a:p:d:")) != -1) {
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
      break;
    }
  }

  printSettings(settings);

  PocketDispatcher dispatcher;
  if (dispatcher.Initialize(settings.address, settings.port)) {
    cout << "Cannot initialize dispatcher" << endl;
    return -1;
  }

  int res = -1;
  if (settings.operation == Operation::MakeDir) {
    res = dispatcher.MakeDir(settings.filename);
  } else if (settings.operation == Operation::Lookup) {
    res = dispatcher.Lookup(settings.filename);
  } else if (settings.operation == Operation::Enumerate) {
    res = dispatcher.Enumerate(settings.filename);
  } else if (settings.operation == Operation::Put) {
    res = dispatcher.PutFile(settings.filename, settings.dstfile, true);
  } else if (settings.operation == Operation::Get) {
    res = dispatcher.GetFile(settings.filename, settings.dstfile);
  } else if (settings.operation == Operation::DeleteDir) {
    res = dispatcher.DeleteDir(settings.filename);
  } else if (settings.operation == Operation::DeleteFile) {
    res = dispatcher.DeleteFile(settings.filename);
  } else if (settings.operation == Operation::Ioctl) {
    res = dispatcher.CountFiles(settings.filename);
  }

  if (res >= 0) {
    cout << "pocket operation successful" << endl;
  } else {
    cout << "pocket operation failed " << endl;
  }

  return 0;
}
