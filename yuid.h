// Copyright Connor Taffe 2016

#ifndef YATM_YUID_H_
#define YATM_YUID_H_

#include <stddef.h>
#include <stdint.h>

// multi-byte crytographically sound random number,
// which serves as the equivalent of a UUID.
enum { kYuidSize = 64 };

typedef uint8_t yuid[kYuidSize];

void yrandom(yuid y);
void yhash(yuid y, void *src, size_t n);
char *base64(void *src, size_t n);

typedef enum {
  kYatmUserPresenseOnline,
  kYatmUserPresenseAway,
  kYatmUserPresenseOffline,
} yuser_presense;

// User data structure
typedef struct {
  yuid id;
  char *name;
  char *displayName;
  yuser_presense presense;
} yuser;

typedef struct {
  yuid id;  // User ID
  enum {
    kYatmUserEventPresense,
  };
  union {
    yuser_presense presense;
  };
} yuser_event;

// Yatmable is anything that Yatm knows about.
// Any entity Yatm is able to understand is a Yatmable
typedef struct {
  yuid id;
  enum {
    kYatmUser,
    kYatmChan,
  } type;
  union {
    yuser user;
  };
} ything;

typedef struct {
  yuid id;
  yuid parent;
  enum {
    kYatmEventUser,
  } type;
  union {
    yuser_event user;
  };
} yevent;

#endif  // YATM_YUID_H_
