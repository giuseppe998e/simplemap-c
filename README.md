# HashMap-C
This is an implementation of a `HashMap` written in **C** just for fun and with educational purposes.  
**It is not intended for use in a production scenario.**

### Usage example (string)
```c
#include <stdio.h>
#include "hashmap.h"

void main(int argc, char *argv) {
  hashmap hm = hm_new(NULL);

  hm_put(hm, "3x12xx", "HELLO", sizeof(char) * 5 /* strlen + 1 */);

  hm_put(hm, "x3xx12", "WORLD", sizeof(char) * 6/* strlen + 1 */);
  char *x = (char *)hm_get(hm, "x3xx12");
  printf("%s\n", x); // WORLD

  hm_put(hm, "x3xx12", "MARS", sizeof(char) * 5/* strlen + 1 */);
  char *y = (char *)hm_get(hm, "x3xx12");
  printf("%s\n", y); // MARS

  hm_rem(hm, "x3xx12");
  //char *yy = (char *)hm_get(hm, "x3xx12"); // Return NULL pointer

  char *z = (char *)hm_get(hm, "3x12xx");
  printf("%s\n", z); // HELLO

  hm_free(hm);
}
```

### Usage example (struct)
```c
#include <stdio.h>
#include "hashmap.h"

void main(int argc, char *argv) {
  struct test {int x; char* y;};
  
  struct test xx = {1998, "HELLO WORLD!"};
  hashmap hm = hm_new(NULL);

  hm_put(hm, "test", &xx, sizeof(struct test));

  struct test* yy = (struct test*)hm_get(hm, "test");
  printf("%d & %s\n", yy->x, yy->y);

  hm_free(hm);
}
```
