/**
 * SimpleMap - A "just for fun" implementation of a C HashMap
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

#include "simplemap.h"

struct __sm_entry {
  char *key;
  char *value;
  struct __sm_entry *next;
};

struct __sm_map {
  size_t capacity; // N# buckets (initial 16) - Power of 2
  size_t entries;  // N# of ALL items
  struct __sm_entry **buckets;
};

/**
 * Create a new map
 */
simplemap *sm_new() {
  simplemap *map = (simplemap *)malloc(sizeof(simplemap));
  map->capacity = __SM_INITCAPACITY;
  map->buckets = (struct __sm_entry **)malloc(__SM_INITCAPACITY * sizeof(struct __sm_entry *));

  return map;
}

/**
 * Support function for sm_free
 */
void __sm_entryfree(struct __sm_entry *entry) {
  if (entry->next != NULL) {
    __sm_entryfree(entry->next);
  }
    
  free(entry->key);
  free(entry->value);
  free(entry);
}

/**
 * Free up the space occupied by the map
 */
void sm_free(simplemap *map) {
  for (int i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
      __sm_entryfree(map->buckets[i]);
    }
  }

  free(map->buckets);
  free(map);
}

/**
 * Return the current load factor of the map
 */
float __sm_loadfactor(simplemap *map) {
  return (float)map->entries / (float)map->capacity;
}

/**
 * Hash function to find the bucket index
 * (Bernstein hash DJB2)
 */
size_t __sm_hashfunc(simplemap *map, char *k) {
  size_t hash = 5381; // prime number
  while(*k) hash = ((hash << 5) + hash) ^ *k++; // (hash * 33) ^ *k++
  return hash % map->capacity;
}

/**
 * Support function for __sm_expandmap
 */
void __sm_expandmap_entry(simplemap *map, struct __sm_entry *entry) {
  if (entry->next != NULL) {
    __sm_expandmap_entry(map, entry->next);
  }

  size_t new_i = __sm_hashfunc(map, entry->key);

  if (map->buckets[new_i] == NULL) {
    map->buckets[new_i] = entry;
  } else {
    struct __sm_entry *new_pos = map->buckets[new_i],
                      *new_prev;
    while (new_pos != NULL) {
      new_prev = new_pos;
      new_pos = new_pos->next;
    }

    new_prev->next = entry;
  }

  entry->next = NULL;
}

/**
 * Expand the map, doubling its capacity
 */
void __sm_expandmap(simplemap *map) {
  int old_capacity = map->capacity;

  map->capacity = old_capacity * 2;
  map->buckets = (struct __sm_entry **)realloc(map->buckets, map->capacity * sizeof(struct __sm_entry *));

  for (int i = 0; i < old_capacity; i++) {
    if (map->buckets[i] != NULL) {
      __sm_expandmap_entry(map, map->buckets[i]);
      map->buckets[i] = NULL;
    }
  }
}

/**
 * Support function for sm_put
 */
struct __sm_entry *__sm_newentry(simplemap *map, char *k, char *v) {
  struct __sm_entry *entry = (struct __sm_entry *)malloc(sizeof(struct __sm_entry));
  
  entry->key = (char *)malloc(sizeof(char) * (strlen(k) + 1));
  strcpy(entry->key, k);

  entry->value = (char *)malloc(sizeof(char) * (strlen(v) + 1));
  strcpy(entry->value, v);
  
  entry->next = NULL;

  map->entries += 1;
  return entry;
}

/**
 * Add (or replace) a <key, value> tuple to the map
 */
void sm_put(simplemap *map, char *k, char *v) {
  if (__sm_loadfactor(map) > __SM_LOADFACTOR) {
    __sm_expandmap(map);
  }

  int index = __sm_hashfunc(map, k);

  if (map->buckets[index] == NULL) {
    map->buckets[index] = __sm_newentry(map, k, v);
    return;
  }

  struct __sm_entry *prev,
                    *pos = map->buckets[index];
  while (pos != NULL) {
    if (strcmp(pos->key, k) == 0) {
      free(pos->value);
      pos->value = (char *)malloc(sizeof(char) * (strlen(v) + 1));
      strcpy(pos->value, v);
      return;
    }

    prev = pos;
    pos = pos->next;
  }

  prev->next = __sm_newentry(map, k, v);
}

/**
 * Get the value associated with the given key
 */
char *sm_get(simplemap *map, char *k) {
  int index = __sm_hashfunc(map, k);
  struct __sm_entry *pos = map->buckets[index];

  while (pos != NULL) {
    if (strcmp(pos->key, k) == 0) {
      return pos->value;
    }

    pos = pos->next;
  }

  return "";
}

/**
 * Remove a <key, value> tuple from the map
 */
void sm_rem(simplemap *map, char *k) {
  int index = __sm_hashfunc(map, k);

  if (map->buckets[index] == NULL) {
    return;
  }

  if (strcmp(map->buckets[index]->key, k) == 0) {
    struct __sm_entry *to_del = map->buckets[index];
    map->buckets[index] = to_del->next;
    free(to_del);
    map->entries -= 1; 
    return;
  }

  struct __sm_entry *prev = map->buckets[index],
                    *to_del = prev->next;

  while (to_del != NULL) {
    if (strcmp(to_del->key, k) == 0) {
      prev->next = to_del->next;
      free(to_del);
      map->entries -= 1;
      return;
    }
    
    prev = to_del;
    to_del = to_del->next;
  }
}
