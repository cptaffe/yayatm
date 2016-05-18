// Copyright Connor Taffe 2016

#ifndef YATM_GRAPH_H_
#define YATM_GRAPH_H_

#include "yuid.h"

typedef struct {
  yevent event;
  yuid *leaves;
} ynode;

// Binary Search Tree
// TODO: auto-balance (Red-Black tree)
typedef struct ytree {
  ynode key;
  struct ytree *parent, *left, *right;
} ytree;

typedef struct {
  ytree *tree;
  yuid parent;
  yuid *lids;
  ynode *leaves;  // current leaves
} ygraph;

ytree *ytleaf(ynode n);
ynode *ynleaf(yevent e);
ygraph *yggen();
ytree *ysearch(ytree *t, yuid id);
void yinsert(ytree *t, ytree *n);
void ydelete(ytree *t);
void ypprint(ytree *t);
void yadd(ygraph *g, ynode n);

#endif  // YATM_GRAPH_H_
