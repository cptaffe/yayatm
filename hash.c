// Copyright 2016 Connor Taffe

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "y.h"

typedef struct { uint64_t lanes[5][5]; } yhstate;

static uint64_t rotl(uint64_t a, size_t b) { return a << b | a >> (64 - b); }

static const uint64_t rndc[24] = {
    0x0000000000000001, 0x000000008000808B, 0x0000000000008082,
    0x800000000000008B, 0x800000000000808A, 0x8000000000008089,
    0x8000000080008000, 0x8000000000008003, 0x000000000000808B,
    0x8000000000008002, 0x0000000080000001, 0x8000000000000080,
    0x8000000080008081, 0x000000000000800A, 0x8000000000008009,
    0x800000008000000A, 0x000000000000008A, 0x8000000080008081,
    0x0000000000000088, 0x8000000000008080, 0x0000000080008009,
    0x0000000080000001, 0x000000008000000A, 0x8000000080008008};
static const size_t rotc[24] = {1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
                                27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44};
static const size_t piln[24] = {10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
                                15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1};

static void hash(yhstate *s) {
  for (size_t r = 0; r < 24; r++) {
    uint64_t buf[5];
    // theta
    for (size_t i = 0; i < 5; i++)
      buf[i] = s->lanes[i][0] ^ s->lanes[i][1] ^ s->lanes[i][2] ^
               s->lanes[i][3] ^ s->lanes[i][4];

    for (size_t i = 0; i < 5; i++) {
      uint64_t t = buf[(i + 4) % 5] ^ rotl(buf[(i + 1) % 5], 1);
      for (size_t j = 0; j < 5; j++) s->lanes[j][i] ^= t;
    }

    // rho pi
    uint64_t t = s->lanes[1][0];
    for (size_t i = 0; i < 24; i++) {
      size_t j = piln[i];
      buf[0] = ((uint64_t *)s->lanes)[j];
      ((uint64_t *)s->lanes)[j] = rotl(t, rotc[i]);
      t = buf[0];
    }

    // chi
    for (size_t j = 0; j < 5; j++) {
      for (size_t i = 0; i < 5; i++) {
        buf[i] = s->lanes[j][i];
      }
      for (size_t i = 0; i < 5; i++) {
        s->lanes[j][i] ^= (~buf[(i + 1) % 5]) & buf[(i + 2) % 5];
      }
    }

    // iota
    s->lanes[0][0] ^= rndc[r];
  }
}

void yhash(yuid h, const void *v, size_t s) {
  yhstate hs;
  memset(&hs, 0, sizeof(hs));
  // padding
  for (size_t i = 0; i < s; i++) {
    ((uint8_t *)hs.lanes)[i % (25 * sizeof(uint64_t))] ^=
        ((const uint8_t *)v)[i];
    if (i % 25 * sizeof(uint64_t) == 0 && i) {
      hash(&hs);
    }
  }
  ((uint8_t *)hs.lanes)[s % (25 * sizeof(uint64_t))] ^= 0x06;
  ((uint8_t *)hs.lanes)[(25 * sizeof(uint64_t)) - 1] ^= 0x80;
  hash(&hs);

  memcpy(h, hs.lanes, sizeof(yuid));
}
