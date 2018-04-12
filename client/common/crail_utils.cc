#include "crail_utils.h"

int file_hash(string name) {
  const char *str = name.c_str();
  int length = name.length();
  int hash_code = 0;
  for (uint32_t i = 0; i < length; i++) {
    hash_code = 31 * hash_code + str[i];
  }
  return hash_code;
}
