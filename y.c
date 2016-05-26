// Copyright 2016 Connor Taffe

#include <stdlib.h>
#include <string.h>

#include "y.h"

static void yilink(y *y, ykey *k, size_t i) {
  size_t len = 0;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    if (len == i) {
      memcpy(k, &l->key, sizeof(ykey));
    }
    len++;
  }
  return;
}

// return a random link
static void yrlink(y *y, ykey *k) {
  if (y->unlinked == NULL) {
    return;
  }
  size_t len = 0;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    len++;
  }
  size_t i;
  yrandom(&i, sizeof(i));
  yilink(y, k, i % len);
}

static void ylrm(y *y, ykey *k) {
  ylink *last = NULL;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    if (!memcmp(&l->key, k, sizeof(ykey))) {
      if (!last) {
        y->unlinked = l->next;
      } else {
        last->next = l->next;
      }
      free(l);
      return;
    }
    last = l;
  }
}

ynode *ymsg(y *y, void *v, size_t s) {
  // generate a header
  ynode *n = ypoint(v, s);
  memcpy(n->key.key, y->pubkey, sizeof(yuid));
  yrlink(y, &n->parent);
  ylrm(y, &n->parent);
  // insert into the tree
  ytree *lf = yleaf(n);
  y->tree = yinsert(y->tree, lf);
  // insert into list, list by node hash
  ylink *l = malloc(sizeof(ylink));
  *l = (ylink){
      .key = n->key,
  };
  l->next = y->unlinked;
  y->unlinked = l;
  return n;
}

y *yy() { return calloc(sizeof(y), 1); }

void yunchain(ylink *l) {
  if (l != NULL) {
    yunchain(l->next);
  }
  free(l);
}

void yuny(y *y) {
  yuntree(y->tree);
  yunchain(y->unlinked);
  free(y);
}
