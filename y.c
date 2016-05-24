// Copyright 2016 Connor Taffe

#include <stdlib.h>
#include <string.h>

#include "y.h"

// return a random link
static const yuid *yrlink(y *y) {
  size_t len = 0;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    len++;
  }
  size_t i;
  yrandom(&i, sizeof(i));
  len = 0;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    if (len == i) {
      return &l->id;
    }
    len++;
  }
  return NULL;
}

static void ylrm(y *y, yuid id) {
  ylink *last = NULL;
  for (ylink *l = y->unlinked; l != NULL; l = l->next) {
    if (!memcmp(l->id, id, sizeof(yuid))) {
      if (!last) {
        y->unlinked = l->next;
      } else {
        last->next = l->next;
      }
      free(l);
    }
    last = l;
  }
}

yhead *ymsg(y *y, void *v, size_t s) {
  // generate a header
  yhead *h = ywire(y->privkey, y->pubkey, *yrlink(y), v, s);
  // insert into the tree
  y->tree = yinsert(y->tree, yleaf(h));
  // add to list of parentless entries.
  ylrm(y, h->crypt.parent);
  return h;
}

y *yy() { return calloc(sizeof(y), 1); }

void ydlink(ylink *l) {
  if (l != NULL) {
    ydlink(l->next);
  }
  free(l);
}

void ydy(y *y) {
  ydleaf(y->tree);
  ydlink(y->unlinked);
  free(y);
}
