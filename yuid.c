// Copyright Connor Taffe 2016

#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tiny_sha3/sha3.h"
#include "yuid.h"

void yrandom(yuid y) { syscall(SYS_getrandom, y, kYuidSize, 0); }

// return a hash of some bytes
void yhash(yuid y, void *src, size_t n) { sha3(src, n, y, kYuidSize); }

char *base64(void *srcv, size_t n) {
  const size_t s = ((n / 3) + 1) * 4;
  const char *dict =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+/";
  uint8_t *src = srcv;
  char *buf = calloc(sizeof(char), s);
  size_t j = 0;
  for (size_t i = 0; i < n; i++) {
    if (i % 3 == 0) {
      // bottom 6 bits
      buf[j++] = dict[src[i] & 0x3f];
      buf[j] = dict[((src[i] & 0xc0) >> 6)];
    } else if (i % 3 == 1) {
      // top two bits | bottom 4 bits
      buf[j++] = dict[((src[i - 1] & 0xc0) >> 6) | ((src[i] & 0xf) << 2)];
      buf[j] = dict[((src[i] & 0xf0) >> 4)];
    } else if (i % 3 == 2) {
      // top 4 bits | bottom 2 bits
      buf[j++] = dict[((src[i - 1] & 0xf0) >> 4) | ((src[i] & 0x3) << 4)];
      // top 6 bits
      buf[j++] = dict[((src[i] & 0xfc) >> 2)];
    }
  }
  if (j == 0 || j == 1) {
    buf[j + 1] = '=';
  }
  if (j == 0) {
    buf[j + 2] = '=';
  }
  // already zeroed because calloc
  return buf;
}
