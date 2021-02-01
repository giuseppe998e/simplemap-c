# SimpleMap
This is an implementation of a `HashMap` written in **C** just for fun and with educational purposes.  
**It is not intended for use in a production scenario.**

### Usage example
```c
#include <stdio.h>
#include "simplemap.h"

void main(int argc, char *argv) {
  simplemap *sm = sm_new();

  sm_put(sm, "3x12xx", "HELLO");

  sm_put(sm, "x3xx12", "WORLD");
  char *x = (char *)sm_get(sm, "x3xx12");
  printf("%s\n", x); // WORLD

  sm_put(sm, "x3xx12", "MARS");
  char *y = (char *)sm_get(sm, "x3xx12");
  printf("%s\n", y); // MARS

  sm_rem(sm, "x3xx12");
  //char *y = (char *)sm_get(sm, "x3xx12");
  //printf("%s\n", y); // Segmentation fault (to resolve)

  char *z = (char *)sm_get(sm, "3x12xx");
  printf("%s\n", z); // HELLO

  sm_free(sm);
}
```
