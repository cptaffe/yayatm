// Copyright Connor Taffe 2016

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

// NOTE: messages should be indexed by crypt.id, which means they must include
// the time of the transaction. Perhaps the time should be within some error?

// tree generate crypt.id
static void ytgh(ytree *t) {
  if (t->left || t->right) {
    if (t->left) {
      ytgh(t->left);
      yhcomb(t->hash, t->left->head->crypt.id);
    }
    if (t->right) {
      ytgh(t->right);
      yhcomb(t->hash, t->right->head->crypt.id);
    }
  } else {
    yhash(t->hash, t->head, sizeof(yhead) + t->head->length);
  }
}

ytree *yleaf(yhead *h) {
  ytree *nt = calloc(sizeof(ytree), 1);
  *nt = (ytree){
      .head = h,
  };
  return nt;
}

void ydleaf(ytree *t) {
  if (t != NULL) {
    ydleaf(t->left);
    ydleaf(t->right);
    free(t->head);
    free(t);
  }
}

ytree *ysearch(ytree *t, yuid id) {
  int o = memcmp(t->head->crypt.id, id, sizeof(yuid));
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

ytree *yinsert(ytree *t, ytree *n) {
  n->parent = t;
  if (t == NULL) {
    return n;
  }
  int cmp = memcmp(n->head->crypt.id, t->head->crypt.id, sizeof(yuid));
  if (cmp > 0) {
    if (t->right != NULL) {
      yinsert(t->right, n);
    } else {
      t->right = n;
      ytgh(t);
    }
  } else if (cmp < 0) {
    if (t->left != NULL) {
      yinsert(t->left, n);
    } else {
      t->left = n;
      ytgh(t);
    }
  }
  // NOTE: else crypt.ides are the same:
  // assuming they are the same message and it is already
  // in the tree
  return t;
}

void ydelete(ytree *t) {
  if (t->parent->right != NULL) {
    if (memcmp(t->head->crypt.id, t->parent->right->head->crypt.id,
               sizeof(yuid)) == 0) {
      t->parent->right = NULL;
    } else {
      t->parent->left = NULL;
    }
  } else {
    t->parent->left = NULL;
  }
  yinsert(t->parent, t->left);
  yinsert(t->parent, t->right);
  ytgh(t->parent);  // recrypt.id tree
}

static void lpprint(ytree *t, size_t l) {
  char *sbuf = calloc(sizeof(char), l + 1);
  memset(sbuf, '-', l);
  printf("%s", sbuf);
  free(sbuf);
  char *buf = base64(t->head->crypt.id, sizeof(yuid)),
       *buf2 = base64(t->hash, sizeof(yuid));
  printf("%s[%s]\n", buf, buf2);
  free(buf);
  free(buf2);
  if (t->left != NULL) lpprint(t->left, l + 1);
  if (t->right != NULL) lpprint(t->right, l + 1);
}

void ypprint(ytree *t) { lpprint(t, 0); }
