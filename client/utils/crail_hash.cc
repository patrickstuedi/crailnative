#include "crail_hash.h"

int file_hash(string name) {
  const char *str = name.c_str();
  int length = name.length();
  int hash_code = 0;
  for (uint32_t i = 0; i < length; i++) {
    hash_code = 31 * hash_code + str[i];
  }
  return hash_code;
}

int file_hash(string &name, int &start) {
  int hash_code = 0;
  while (start < name.length() && name.at(start) != '/') {
    hash_code = 31 * hash_code + name.at(start);
    start++;
  }
  return hash_code;
}
