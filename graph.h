// Copyright Connor Taffe 2016

#ifndef GRAPH_H_
#define GRAPH_H_

#include "./yuid.h"

typedef struct {
  yuid hash;   // hash of msg
  yuid key;    // sender's public key
  yuid recip;  // reciprient's public key
} ykey;

typedef struct {
  ykey parent;  // parent's key
  ykey key;     // index key
  size_t size;
  const void *msg;
} ynode;

// Binary Search Tree (Merkle Tree)
// sorted by event hash
// TODO(cptaffe): auto-balance (Red-Black tree)
typedef struct ytree {
  ynode *head;
  struct ytree *parent, *left, *right;
} ytree;

ynode *ypoint(void *v, size_t s);
ytree *yleaf(ynode *n);
void yuntree(ytree *t);
ytree *ysearch(ytree *t, ykey *k);
ytree *yinsert(ytree *t, ytree *n);
void ydelete(ytree *t);
void ypprint(ytree *t);
bool ytraverse(ytree *t, void *v, bool (*f)(void *, ytree *));

#endif  // GRAPH_H_
