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

int main() {
  y *y = yy();
  for (size_t i = 0; i < 10; i++) {
    ymsg(y, &i, sizeof(i));
  }
  ypprint(y->tree);

  yuid key;
  yrandom(key, sizeof(yuid));
  int a = 4;
  yhead *h = ywire(key, key, NULL, &a, sizeof(a));
  printf("%d\n", yunwire(h));
  free(h);
  ydy(y);
}
