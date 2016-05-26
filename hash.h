// Copyright 2016 Connor Taffe

#ifndef HASH_H_
#define HASH_H_

#include <stddef.h>
#include <stdint.h>

typedef uint8_t yuid[64];

void yhash(yuid h, const void *v, size_t s);

#endif  // HASH_H_
