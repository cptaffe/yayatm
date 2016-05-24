// Copyright Connor Taffe 2016

#ifndef YATM_YUID_H_
#define YATM_YUID_H_

#include <stddef.h>
#include <stdint.h>

// multi-byte crytographically sound random number,
// which serves as the equivalent of a UUID.
enum { kYuidSize = 2 };

typedef uint8_t yuid[kYuidSize];

void yrandom(yuid y);
void yhash(yuid y, void *src, size_t n);
void yhcomb(yuid y, yuid a);
char *base64(void *src, size_t n);

typedef struct {
  yuid id;
  yuid parent;
} ymsg;

#endif  // YATM_YUID_H_
