/**
 * HashMap-C - A "just for fun" implementation of a HashMap in C
 * Copyright (C) 2021 - Giuseppe Eletto <peppe.eletto@gmail.com>
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPLv3 along with
 * this library. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

struct __entry {
  char *key;             // Entry Key
  void *value;           // Entry value
  struct __entry *next;  // Next entry
};

struct __hashmap {
  size_t capacity;           // N# buckets (initial 16) - Power of 2
  size_t entries;            // N# of ALL items
  struct __entry **buckets;  // List of buckets of the map
  void (*free_v)(void*);     // Entry value custom free fn
};

/**
 * 
 */
hashmap hm_new(void (*free_v)(void*)) {
  struct __hashmap *hm = malloc(sizeof(*hm));
  if (!hm)
    return NULL;
  
  // Initialize HashMap struct
  hm->capacity = HM_INITCAPACITY;
  hm->entries = 0;
  hm->buckets = (struct __entry **)malloc(HM_INITCAPACITY * sizeof(struct __entry *));
  hm->free_v = free_v ? free_v : free;

  return hm;
}

/**
 * Return the current load factor of the hm.
 */
static float __loadfactor(hashmap hm) {
  return (float)hm->entries / (float)hm->capacity;
}

/**
 * Hash function to find the bucket index.
 * (Bernstein hash DJB2)
 */
static size_t __hashfunc(hashmap hm, char *key) {
  size_t hash = 5381; // prime number
  while(*key) hash = ((hash << 5) + hash) ^ *key++; // (hash * 33) ^ *key++
  return hash % hm->capacity;
}

/**
 * Support function for __expandmap.
 */
static void __expandmap_entry(hashmap hm, struct __entry *entry) {
  if (entry->next)
    __expandmap_entry(hm, entry->next);

  size_t new_i = __hashfunc(hm, entry->key);

  if (hm->buckets[new_i] == NULL) {
    hm->buckets[new_i] = entry;
  } else {
    struct __entry *new_pos = hm->buckets[new_i],
                      *new_prev;

    while (new_pos) {
      new_prev = new_pos;
      new_pos = new_pos->next;
    }

    new_prev->next = entry;
  }

  entry->next = NULL;
}

/**
 * Expand the hm, doubling its capacity.
 */
static void __expandmap(hashmap hm) {
  int old_capacity = hm->capacity;

  hm->capacity = old_capacity * 2;
  hm->buckets = (struct __entry **)realloc(hm->buckets, hm->capacity * sizeof(struct __entry *));

  for (int i = 0; i < old_capacity; i++) {
    if (hm->buckets[i] == NULL) continue;

    __expandmap_entry(hm, hm->buckets[i]);
    hm->buckets[i] = NULL;
  }
}

/**
 * Support function for hm_put
 */
static struct __entry *__newentry(hashmap hm, char *key, void *value, size_t size) {
  struct __entry *entry = (struct __entry *)malloc(sizeof(struct __entry));

  entry->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(entry->key, key);

  entry->value = (char *)malloc(size);
  memcpy(entry->value, value, size);

  entry->next = NULL;

  hm->entries += 1;
  return entry;
}

/**
 * Add (or replace) a <key, value> tuple to the hm
 */
void hm_put(hashmap hm, char *key, void *value, size_t size) {
  if (__loadfactor(hm) > HM_LOADFACTOR)
    __expandmap(hm);

  int index = __hashfunc(hm, key);

  if (hm->buckets[index] == NULL) {
    hm->buckets[index] = __newentry(hm, key, value, size);
    return;
  }

  struct __entry *prev,
                    *pos = hm->buckets[index];
  while (pos) {
    if (strcmp(pos->key, key) == 0) {
      hm->free_v(pos->value);
      pos->value = (void *)malloc(size);
      memcpy(pos->value, value, size);
      return;
    }

    prev = pos;
    pos = pos->next;
  }

  prev->next = __newentry(hm, key, value, size);
}

/**
 * Get the value associated with the given key
 */
void *hm_get(hashmap hm, char *key) {
  int index = __hashfunc(hm, key);
  struct __entry *pos = hm->buckets[index];

  for (; pos; pos = pos->next)
    if (strcmp(pos->key, key) == 0)
      return pos->value;

  return NULL;
}

/**
 *
 */
static void __free_entry(hashmap hm, struct __entry *entry) {
    free(entry->key);
    hm->free_v(entry->value);
    free(entry);
}

/**
 * Remove a <key, value> tuple from the hm
 */
void hm_rem(hashmap hm, char *key) {
  int index = __hashfunc(hm, key);

  if (hm->buckets[index] == NULL)
    return;

  if (strcmp(hm->buckets[index]->key, key) == 0) {
    struct __entry *to_del = hm->buckets[index];
    hm->buckets[index] = to_del->next;
    __free_entry(hm, to_del);

    hm->entries -= 1;
    return;
  }

  struct __entry *prev = hm->buckets[index],
                    *to_del = prev->next;

  while (to_del) {
    if (strcmp(to_del->key, key) == 0) {
      prev->next = to_del->next;
      __free_entry(hm, to_del);

      hm->entries -= 1;
      return;
    }

    prev = to_del;
    to_del = to_del->next;
  }
}

/**
 * Support function for hm_free.
 */
static void __free_bucket(hashmap hm, struct __entry *entry) {
  while (entry) {
    struct __entry *tmp = entry;
    entry = tmp->next;
    __free_entry(hm, tmp);
  }
}

/**
 * 
 */
void hm_free(hashmap hm) {
  for (int i = 0; i < hm->capacity; i++) {
    if (hm->buckets[i]) 
      __free_bucket(hm, hm->buckets[i]);
  }

  free(hm->buckets);
  free(hm);
}
