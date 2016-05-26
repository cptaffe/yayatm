// Copyright Connor Taffe 2016

#include <linux/random.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "graph.h"
#include "y.h"

bool yhistory(y *y, ytree *t, void *v, bool (*const f)(void *, ytree *));
bool ydepth(y *y, ytree *t, size_t d, void *v,
            bool (*const f)(void *, ytree *, size_t));

// call method on each node of the tree
// traverse by message parent hash
bool yhistory(y *y, ytree *t, void *v, bool (*const f)(void *, ytree *)) {
  return (t != NULL)
             ? (yhistory(y, ysearch(y->tree, &t->head->parent), v, f) ? f(v, t)
                                                                      : false)
             : true;
}

bool ydepth(y *y, ytree *t, size_t d, void *v,
            bool (*const f)(void *, ytree *, size_t)) {
  return (t != NULL)
             ? (ydepth(y, t->left, d + 1, v, f)
                    ? (ydepth(y, t->right, d + 1, v, f) ? f(v, t, d) : false)
                    : false)
             : true;
}

static bool ywrapper(__attribute__((unused)) void *v, ytree *t) {
  char *buf = base64(&t->head->key, sizeof(ykey));
  printf("%ld, %s\n", *(const uint64_t *)t->head->msg, buf);
  free(buf);
  return true;
}

struct ydepths {
  size_t max, sum, count, collisions;
};

static bool ydepthf(struct ydepths *ds, ytree *t, size_t d) {
  ds->max = (ds->max >= d) ? ds->max : d;
  ds->sum += d;
  ds->count++;
  if (t->parent) {
    ds->collisions +=
        memcmp(t->head->key.hash, t->parent->head->key.hash, sizeof(ykey)) ? 0
                                                                           : 1;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s n\n", argv[0]);
    return 1;
  }
  y *y = yy();
  for (size_t i = 0; i < (size_t)atoi(argv[1]); i++) {
    size_t *j = malloc(sizeof(size_t));
    *j = i;
    ymsg(y, j, sizeof(size_t));
  }
  // tree stats
  struct ydepths d;
  memset(&d, 0, sizeof(d));
  ydepth(y, y->tree, 0, &d, (bool (*const)(void *, ytree *, size_t))ydepthf);
  printf("max: %ld, sum: %ld, count: %ld, avg: %lf, collisons: %ld\n", d.max,
         d.sum, d.count, ((double)d.sum) / d.count, d.collisions);
  // print tree
  ypprint(y->tree);
  // print history in reverse chronological order
  /*
  printf("%s\n",
         yhistory(y, ysearch(y->tree, &y->unlinked->key), NULL, ywrapper)
             ? "success"
             : "failure");
             */
  yuny(y);
}
