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

#include "crail_dispatcher.h"

using namespace std;

enum class Operation {
  MakeDir = 1,
  Lookup = 2,
  Enumerate = 3,
};

struct Settings {
  string address;
  int port;
  Operation operation;
  string filename;
  int loop;
  int size;
};

Operation getOperation(string name) {
  if (name == "MakeDir") {
    return Operation::MakeDir;
  }
  if (name == "Lookup") {
    return Operation::Lookup;
  }
  if (name == "Enumerate") {
    return Operation::Enumerate;
  } else {
    return Operation::MakeDir;
  }
}

void printSettings(Settings &settings) {
  cout << "Settings, address " << settings.address << ", port " << settings.port
       << ", operation " << static_cast<int>(settings.operation)
       << ", filename " << settings.filename << ", loop " << settings.loop
       << ", size " << settings.size << endl;
}

int main(int argc, char *argv[]) {
  Settings settings;

  int opt = 0;
  while ((opt = getopt(argc, argv, "t:f:k:s:a:p:")) != -1) {
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
    }
  }

  printSettings(settings);

  CrailDispatcher benchmark;
  if (benchmark.Initialize(settings.address, settings.port)) {
    cout << "Cannot initialize benchmark" << endl;
    return -1;
  }

  if (settings.operation == Operation::MakeDir) {
    benchmark.MakeDir(settings.filename);
  } else if (settings.operation == Operation::Lookup) {
    benchmark.Lookup(settings.filename);
  } else if (settings.operation == Operation::Enumerate) {
    benchmark.Enumerate(settings.filename);
  }

  return 0;
}
