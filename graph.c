// Copyright Connor Taffe 2016

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

// NOTE: messages should be indexed by hash, which means they must include
// the time of the transaction. Perhaps the time should be within some error?

// tree generate hash
static void ytgh(ytree *t) {
  if (t->left || t->right) {
    if (t->left) {
      ytgh(t->left);
      yhcomb(t->hash, t->left->msg.id);
    }
    if (t->right) {
      ytgh(t->right);
      yhcomb(t->hash, t->right->msg.id);
    }
  } else {
    yhash(t->hash, &t->msg, sizeof(ymsg));
  }
}

ytd yleaf(ytree *t, ymsg m) {
  ytree *nt = malloc(sizeof(ytree));
  *nt = (ytree){
      .msg = m,
  };
  yinsert(t, nt);
  return nt->msg.id;
}

ytree *ysearch(ytree *t, ytd id) {
  int o = memcmp(t->msg.id, id, sizeof(yuid));
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
  if (memcmp(n->msg.id, t->msg.id, sizeof(yuid)) > 0) {
    if (t->right != NULL) {
      yinsert(t->right, n);
    } else {
      t->right = n;
      ytgh(t);
    }
  } else {
    if (t->left != NULL) {
      yinsert(t->left, n);
    } else {
      t->left = n;
      ytgh(t);
    }
  }
}

void ydelete(ytree *t) {
  if (t->parent->right != NULL) {
    if (memcmp(t->msg.id, t->parent->right->msg.id, sizeof(yuid)) == 0) {
      t->parent->right = NULL;
    } else {
      t->parent->left = NULL;
    }
  } else {
    t->parent->left = NULL;
  }
  yinsert(t->parent, t->left);
  yinsert(t->parent, t->right);
  ytgh(t->parent);  // rehash tree
}

static void lpprint(ytree *t, size_t l) {
  char *sbuf = calloc(sizeof(char), l + 1);
  memset(sbuf, '-', l);
  printf("%s", sbuf);
  char *buf = base64(t->msg.id, sizeof(yuid)),
       *buf2 = base64(t->hash, sizeof(yuid));
  printf("%s[%s]\n", buf, buf2);
  free(buf);
  free(buf2);
  if (t->left != NULL) lpprint(t->left, l + 1);
  if (t->right != NULL) lpprint(t->right, l + 1);
}

void ypprint(ytree *t) { lpprint(t, 0); }
