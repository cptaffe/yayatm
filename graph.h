// Copyright Connor Taffe 2016

#ifndef YATM_GRAPH_H_
#define YATM_GRAPH_H_

#include "wire.h"
#include "yuid.h"

// Binary Search Tree (Merkle Tree)
// sorted by event hash
// TODO: auto-balance (Red-Black tree)
typedef struct ytree {
  yuid hash;
  yhead *head;
  struct ytree *parent, *left, *right;
} ytree;

ytree *yleaf(yhead *h);
void ydleaf(ytree *t);
ytree *ysearch(ytree *t, yuid id);
ytree *yinsert(ytree *t, ytree *n);
void ydelete(ytree *t);
void ypprint(ytree *t);

#endif  // YATM_GRAPH_H_
