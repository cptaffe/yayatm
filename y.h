// Copyright 2016 Connor Taffe

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stddef.h>

#include "graph.h"
#include "yuid.h"

typedef struct ylink {
  yuid id;
  struct ylink *next;
} ylink;

typedef struct {
  ytree *tree;
  ylink *unlinked;
  yuid privkey, pubkey;
} y;

y *yy();
void ydy(y *y);
yhead *ymsg(y *y, void *v, size_t s);

#endif  // MESSAGE_H_
