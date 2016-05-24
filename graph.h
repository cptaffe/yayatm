// Copyright Connor Taffe 2016

#ifndef YATM_GRAPH_H_
#define YATM_GRAPH_H_

#include "yuid.h"

// Binary Search Tree
// sorted by event hash
// TODO: auto-balance (Red-Black tree)
// Merkle tree
typedef struct ytree {
  yuid hash;
  ymsg msg;
  struct ytree *parent, *left, *right;
} ytree;

typedef uint8_t *ytd;  // tree descriptor (tnode hash)

ytd yleaf(ytree *t, ymsg m);
ytree *ysearch(ytree *t, yuid id);
void yinsert(ytree *t, ytree *n);
void ydelete(ytree *t);
void ypprint(ytree *t);

#endif  // YATM_GRAPH_H_
