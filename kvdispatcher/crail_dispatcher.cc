#include "crail_dispatcher.h"

CrailDispatcher::CrailDispatcher() {}

CrailDispatcher::~CrailDispatcher() {}

int CrailDispatcher::Initialize(string address, int port) {
  return this->crail_.Initialize(address, port);
}

int CrailDispatcher::MakeDir(string name) {
  unique_ptr<CrailNode> crail_node = crail_.Create(name, FileType::Directory);
  return 0;
}

int CrailDispatcher::GetFile(string src_file, string local_file) { return 0; }

int CrailDispatcher::PutFile(string local_file, string dst_file) { return 0; }

int CrailDispatcher::DeleteDir(string directory) { return 0; }

int CrailDispatcher::DeleteFile(string file) { return 0; }
