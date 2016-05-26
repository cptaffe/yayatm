// Copyright 2016 Connor Taffe

#ifndef WIRE_H_
#define WIRE_H_

#include "./yuid.h"

// wire format header
typedef struct {
  yuid pubkey;
  struct {
    yuid id, parent;
  } crypt;
  uint64_t length;
} yhead;

yhead *ywire(const yuid pubkey, const yuid privkey, const yuid parent,
             const void *v, size_t s);
yhead *yunwire(const yhead *w);

#endif  // WIRE_H_
