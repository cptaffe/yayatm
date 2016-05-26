// Copyright Connor Taffe 2016

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

ynode *ypoint(void *v, size_t s) {
  ynode *n = calloc(sizeof(ynode), 1);
  n->msg = v;
  n->size = s;
  yhash(n->key.hash, v, s);
  return n;
}

ytree *yleaf(ynode *n) {
  ytree *t = calloc(sizeof(ytree), 1);
  *t = (ytree){
      .head = n,
  };
  return t;
}

void yuntree(ytree *t) {
  if (t != NULL) {
    yuntree(t->left);
    yuntree(t->right);
    free(t->head);
    free(t);
  }
}

ytree *ysearch(ytree *t, ykey *k) {
  if (!t) {
    return NULL;
  }
  int o = memcmp(&t->head->key, k, sizeof(ykey));
  if (o < 0)
    return ysearch(t->left, k);
  else if (o > 0)
    return ysearch(t->right, k);
  return t;
}

ytree *yinsert(ytree *t, ytree *n) {
  n->parent = t;
  if (!t) {
    return n;
  }
  int o = memcmp(&t->head->key, &n->head->key, sizeof(ykey));
  if (o <= 0) {
    if (t->left)
      yinsert(t->left, n);
    else
      t->left = n;
  } else if (o > 0) {
    if (t->right)
      yinsert(t->right, n);
    else
      t->right = n;
  }
  return t;
}

void ydelete(ytree *t) {
  if (t->parent->right != NULL) {
    if (memcmp(&t->head->key, &t->parent->right->head->key, sizeof(ykey)) ==
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

// call method on each node of the tree
// traverse recursive descent
bool ytraverse(ytree *t, void *v, bool (*f)(void *, ytree *)) {
  return (t != NULL) ? (ytraverse(t->left, v, f)
                            ? (ytraverse(t->right, v, f) ? f(v, t) : false)
                            : false)
                     : true;
}

static void lpprint(ytree *t, size_t l) {
  char *sbuf = calloc(sizeof(char), l + 1);
  memset(sbuf, '-', l);
  printf("%s", sbuf);
  free(sbuf);
  if (t == NULL) {
    printf("x\n");
    return;
  }
  char *bufs[3] = {base64(&t->head->key, sizeof(ykey)),
                   base64(t->head->key.hash, sizeof(yuid)),
                   base64(&t->head->parent, sizeof(ykey))};
  printf("%s[%s:%s]\n", bufs[0], bufs[1], bufs[2]);
  for (size_t i = 0; i < sizeof(bufs) / sizeof(char *); i++) {
    free(bufs[i]);
  }
  if (t->left || t->right) {
    lpprint(t->left, l + 1);
    lpprint(t->right, l + 1);
  }
}

void ypprint(ytree *t) { lpprint(t, 0); }
