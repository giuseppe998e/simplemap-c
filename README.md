# SimpleMap
This is an implementation of a `HashMap` written in **C** just for fun and with educational purposes.
**It is not intended for use in a production scenario.**

### Usage example
```c
#include <stdio.h>
#include "simplemap.h"

void main(int argc, char *argv) {
  simplemap *sm = sm_new();

  sm_put(sm, "3x12xx", "HELLO", sizeof(char) * 5 /* strlen + 1 */);

  sm_put(sm, "x3xx12", "WORLD", sizeof(char) * 6/* strlen + 1 */);
  char *x = (char *)sm_get(sm, "x3xx12");
  printf("%s\n", x); // WORLD

  sm_put(sm, "x3xx12", "MARS", sizeof(char) * 5/* strlen + 1 */);
  char *y = (char *)sm_get(sm, "x3xx12");
  printf("%s\n", y); // MARS

  sm_rem(sm, "x3xx12");
  //char *yy = (char *)sm_get(sm, "x3xx12"); // Return NULL pointer

  char *z = (char *)sm_get(sm, "3x12xx");
  printf("%s\n", z); // HELLO

  sm_free(sm);
}
```
