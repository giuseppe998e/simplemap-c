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

#ifndef __SIMPLEMAP_H
#define __SIMPLEMAP_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define __SM_INITCAPACITY 16
#define __SM_LOADFACTOR   .75F

typedef struct __sm_map *simplemap;

simplemap sm_new();
void sm_free(simplemap);

void  sm_put(simplemap, char*, void*, size_t);
void *sm_get(simplemap, char*);
void  sm_rem(simplemap, char*);

#endif