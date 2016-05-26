// Copyright Connor Taffe 2016

#ifndef YUID_H_
#define YUID_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "./hash.h"

void yrandom(void *v, size_t s);
void yhcomb(yuid y, const yuid a);
char *base64(const void *src, size_t n);

#endif  // YUID_H_
