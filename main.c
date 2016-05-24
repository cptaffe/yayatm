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
  ytree *t = &(ytree){};
  uint8_t *last = NULL;
  for (size_t i = 0; i < 10; i++) {
    ymsg m = {};
    yrandom(m.id);
    if (last != NULL) {
      memcpy(m.parent, last, sizeof(yuid));
    } else {
      yrandom(m.parent);
    }
    last = m.id;
    yleaf(t, m);
  }
  ypprint(t);
}
