/* demo1.c - demo program using xprof 
 */
#include <stdio.h>
#include <math.h>
#include "cputil.h"

double
foo(double x, double y)
{
  /* first call to sqrt is lots of instructions - dynamic loader? */
  return sqrt((3.0 + x)/(9.4 + y));
}

volatile unsigned long curr, prev;

int
main()
{
  unsigned long i;
  double z;

  z = foo(1.2, 3.6);

  printf("method 1:\n");
  prev = 0; curr = 0;
  for (i = 0; i < 5; i++) {
    CU_CLRCTR();
    z = foo(1.2, 3.6);
    curr = CU_GETCTR();
    printf("count: %lu\t", curr + prev);
    printf("delta: %lu\n", curr);
    prev = prev + curr;
  }

  return 0;
}
/* --- last line of demo1.c --- */
