#include "crail_dispatcher.h"

#include <iostream>

#include "crail_directory.h"

using namespace std;

CrailDispatcher::CrailDispatcher() {}

CrailDispatcher::~CrailDispatcher() {}

int CrailDispatcher::Initialize(string address, int port) {
  return this->crail_.Initialize(address, port);
}

int CrailDispatcher::MakeDir(string name) {
  unique_ptr<CrailNode> crail_node = crail_.Create(name, FileType::Directory);
  if (crail_node) {
    cout << "create node of type " << crail_node->type() << endl;
  } else {
    cout << "create node failed " << endl;
  }
  return 0;
}

int CrailDispatcher::Lookup(string name) {
  unique_ptr<CrailNode> crail_node = crail_.Lookup(name);
  if (crail_node) {
    cout << "lookup node of type " << crail_node->type() << endl;
  } else {
    cout << "lookup node failed" << endl;
  }
  return 0;
}

int CrailDispatcher::Enumerate(string name) {
  unique_ptr<CrailNode> crail_node = crail_.Lookup(name);
  if (crail_node &&
      crail_node->type() == static_cast<int>(FileType::Directory)) {
    CrailNode *node = crail_node.get();
    CrailDirectory *directory = static_cast<CrailDirectory *>(node);
    directory->Enumerate();
  } else {
    cout << "dispatcher::enumerate, could not retrieve directory " << endl;
  }
}

int CrailDispatcher::GetFile(string src_file, string local_file) { return 0; }

int CrailDispatcher::PutFile(string local_file, string dst_file) { return 0; }

int CrailDispatcher::DeleteDir(string directory) { return 0; }

int CrailDispatcher::DeleteFile(string file) { return 0; }
