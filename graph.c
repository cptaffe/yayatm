// Copyright Connor Taffe 2016

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

// NOTE: messages should be indexed by hash, which means they must include
// the time of the transaction. Perhaps the time should be within some error?

ytree *ytleaf(ynode n) {
  ytree *t = malloc(sizeof(ytree));
  *t = (ytree){
      .key = n,
  };
  return t;
}
ynode *ynleaf(yevent e) {
  ynode *n = malloc(sizeof(ynode));
  *n = (ynode){
      .event = e,
  };
  return n;
}

ygraph *yggen() { return calloc(sizeof(ygraph), 1); }

ytree *ysearch(ytree *t, yuid id) {
  int o = memcmp(t->key.event.id, id, sizeof(yuid));
  if (!o) {
    return t;
  }
  if (o > 0) {
    return ysearch(t->right, id);
  } else if (o < 0) {
    return ysearch(t->left, id);
  }
  return NULL;
}

void yinsert(ytree *t, ytree *n) {
  n->parent = t;
  if (memcmp(n->key.event.id, t->key.event.id, sizeof(yuid)) > 0) {
    if (t->right != NULL) {
      yinsert(t->right, n);
    } else {
      t->right = n;
    }
  } else {
    if (t->left != NULL) {
      yinsert(t->left, n);
    } else {
      t->left = n;
    }
  }
}

void ydelete(ytree *t) {
  if (t->parent->right != NULL) {
    if (memcmp(t->key.event.id, t->parent->right->key.event.id, sizeof(yuid)) ==
        0) {
      t->parent->right = NULL;
    } else {
      t->parent->left = NULL;
    }
  } else {
    t->parent->left = NULL;
  }
  yinsert(t->parent, t->left);
  yinsert(t->parent, t->right);
}

void ypprint(ytree *t) {
  char *buf = base64(t->key.event.id, sizeof(yuid));
  printf("%s : {{", buf);
  free(buf);
  if (t->left != NULL) {
    ypprint(t->left);
  }
  printf("}, {");
  if (t->right != NULL) {
    ypprint(t->right);
  }
  printf("}");
}

size_t yleaflen(ygraph *g) {
  if (g->leaves == NULL) {
    return 0;
  }
  ynode en = {};
  for (size_t i = 0;; i++) {
    if (memcmp(&g->leaves[i], &en, sizeof(ynode)) == 0) {
      return i;
    }
  }
}

size_t ylidlen(ygraph *g) {
  if (g->lids == NULL) {
    return 0;
  }
  yuid eid = {};
  for (size_t i = 0;; i++) {
    if (memcmp(&g->lids[i], &eid, sizeof(yuid)) == 0) {
      return i;
    }
  }
}

void yadd(ygraph *g, ynode n) {
  n.leaves = g->lids;  // associate with lids buffer
  // add to tree
  ytree *t = ytleaf(n);
  if (g->tree != NULL) {
    yinsert(g->tree, t);
  } else {
    g->tree = t;
  }
  // check whether this node closes the loop on another node.
  // NOTE: in the special case where parent is zeroed, it is
  // impossible for them to match, therefore it will do the
  // right thing in teh else.
  if (memcmp(g->parent, n.event.parent, sizeof(yuid)) == 0) {
    // doesn't, insert into leaf list
    // update lids buffer
    size_t len = ylidlen(g);
    g->lids = realloc(g->lids, (len + 2) * sizeof(yuid));
    memcpy(g->lids[len + 1], (yuid){}, sizeof(yuid));
    memcpy(g->lids[len], n.event.id, sizeof(yuid));
    len = yleaflen(g);
    g->leaves = realloc(g->leaves, (len + 2) * sizeof(ynode));
    g->leaves[len + 1] = (ynode){};
    g->leaves[len] = n;
  } else {
    memcpy(g->parent, n.event.parent, sizeof(yuid));
    // empty buffers, add id to lids
    g->lids = calloc(sizeof(yuid), 2);
    memcpy(g->lids[0], n.event.id, sizeof(yuid));
    free(g->leaves);
    g->leaves = calloc(sizeof(ynode), 2);
    memcpy(&g->leaves[0], &n, sizeof(ynode));
  }
}
