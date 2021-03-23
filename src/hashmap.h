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

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stddef.h>

#define HM_INITCAPACITY 16
#define HM_LOADFACTOR   .75F

/**
 * 
 */
typedef struct __hashmap *hashmap;

/**
 * 
 */
hashmap hm_new(void(*)(void*));

/**
 * 
 */
void hm_put(hashmap, char*, void*, size_t);

/**
 * 
 */
void *hm_get(hashmap, char*);

/**
 * 
 */
void hm_rem(hashmap, char*);

/**
 * 
 */
void hm_free(hashmap);

#endif
