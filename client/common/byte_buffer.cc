#include "byte_buffer.h"

#include <arpa/inet.h>
#include <endian.h>
#include <iostream>
#include <string.h>

using namespace crail;
using namespace std;

ByteBuffer::ByteBuffer(int size) {
  this->buf_ = new unsigned char[size];
  this->size_ = size;
  this->limit_ = size;
  this->position_ = 0;
  this->order_ = ByteOrder::BigEndian;
  Zero();
}

ByteBuffer::~ByteBuffer() { delete[] buf_; }

void ByteBuffer::PutInt(int value) {
  int *_tmp = (int *)get_bytes();
  if (order_ == ByteOrder::BigEndian) {
    *_tmp = htonl(value);
  } else {
    *_tmp = value;
  }
  this->position_ += sizeof(value);
}

void ByteBuffer::PutShort(short value) {
  short *_tmp = (short *)get_bytes();
  if (order_ == ByteOrder::BigEndian) {
    *_tmp = htons(value);
  } else {
    *_tmp = value;
  }
  this->position_ += sizeof(value);
}

void ByteBuffer::PutLong(long long value) {
  long long *_tmp = (long long *)get_bytes();
  if (order_ == ByteOrder::BigEndian) {
    *_tmp = htobe64(value);
  } else {
    *_tmp = value;
  }
  this->position_ += sizeof(value);
}

void ByteBuffer::PutBytes(const char value[], int length) {
  unsigned char *_tmp = (unsigned char *)get_bytes();
  memcpy(_tmp, value, length);
  this->position_ += length;
}

short ByteBuffer::GetShort() {
  short *_tmp = (short *)get_bytes();
  this->position_ += sizeof(short);
  if (order_ == ByteOrder::BigEndian) {
    return ntohs(*_tmp);
  } else {
    return *_tmp;
  }
}

int ByteBuffer::GetInt() {
  int *_tmp = (int *)get_bytes();
  this->position_ += sizeof(int);
  if (order_ == ByteOrder::BigEndian) {
    return ntohl(*_tmp);
  } else {
    return *_tmp;
  }
}

long long ByteBuffer::GetLong() {
  long long *_tmp = (long long *)get_bytes();
  this->position_ += sizeof(long long);
  if (order_ == ByteOrder::BigEndian) {
    return be64toh(*_tmp);
  } else {
    return *_tmp;
  }
}

void ByteBuffer::GetBytes(char value[], int length) {
  unsigned char *_tmp = (unsigned char *)get_bytes();
  memcpy(value, _tmp, length);
  this->position_ += length;
}

void ByteBuffer::PrintBytes(string message) {
  cout << "printbytes [" << message.c_str() << "]" << endl;
  for (int i = 0; i < size_; i++) {
    unsigned int ch = (unsigned int)buf_[i];
    cout << ch << ".";
  }
  cout << endl;
}

void ByteBuffer::Zero() { bzero(buf_, size_); }
