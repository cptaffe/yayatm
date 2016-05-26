// Copyright 2016 Connor Taffe

#ifndef Y_H_
#define Y_H_

#include <stddef.h>

#include "./graph.h"
#include "./yuid.h"

typedef struct ylink {
  ykey key;
  struct ylink *next;
} ylink;

typedef struct {
  ytree *tree;
  ylink *unlinked;
  yuid privkey, pubkey;
} y;

y *yy(void);
void yuny(y *y);
void yunchain(ylink *l);
ynode *ymsg(y *y, void *v, size_t s);

#endif  // Y_H_
