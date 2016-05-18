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
#include "yuid.h"

int main() {
  ygraph *g = yggen();
  for (size_t i = 0; i < 100; i++) {
    yevent e;
    yrandom(e.id);
    yadd(g, *ynleaf(e));
  }
  ypprint(g->tree);
}
