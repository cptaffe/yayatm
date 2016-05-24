// Copyright 2016 Connor Taffe

#include <stdlib.h>
#include <string.h>

#include "wire.h"

static void ycrypt(void *a, const void *b, const yuid key, size_t s) {
  // TODO: ECC
  const uint8_t *bytes = (uint8_t *)b;
  uint8_t *abytes = (uint8_t *)a;
  for (size_t i = 0; i < s; i++) {
    abytes[i] = bytes[i] ^ key[i % sizeof(yuid)];
  }
}

yhead *ywire(const yuid pubkey, const yuid privkey, const yuid parent, void *v,
             size_t s) {
  yhead *w = calloc(sizeof(yhead) + s, 1);
  w->length = s;
  memcpy(&((uint8_t *)w)[sizeof(yhead)], v, s);
  memcpy(w->pubkey, pubkey, sizeof(yuid));
  if (parent != NULL) {
    memcpy(w->crypt.parent, parent, sizeof(yuid));
  }
  yhash(w->crypt.id, v, s);
  ycrypt(&w->crypt, &w->crypt, privkey, sizeof(w->crypt));
  return w;
}

bool yunwire(yhead *w) {
  // decrypt
  ycrypt(&w->crypt, &w->crypt, w->pubkey, sizeof(w->crypt));
  // verify that the hashes work.
  yuid mh;
  yhash(mh, &((const uint8_t *)w)[sizeof(yhead)], w->length);
  if (memcmp(mh, &w->crypt.id, sizeof(w->crypt.id))) {
    return false;  // unverified msg hash
  }
  return true;
}
