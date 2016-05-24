// Copyright Connor Taffe 2016

#ifndef YATM_YUID_H_
#define YATM_YUID_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t yuid[2];

void yrandom(void *v, size_t s);
void yhash(yuid y, const void *src, size_t n);
void yhcomb(yuid y, const yuid a);
char *base64(const void *src, size_t n);

#endif  // YATM_YUID_H_
