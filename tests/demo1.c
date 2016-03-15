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

unsigned long curr, prev, div;

int
main()
{
  unsigned long i;
  double z;

  CU_REGTHR();

  z = foo(1.2, 3.6);

  prev = 0; curr = 0;
  for (i = 0; i < 5; i++) {
    CU_CLRCTR();
    z = foo(1.2, 3.6);
    curr = CU_GETCTR();
    div = CU_GETDIV();
    printf("count: %lu/%lu\t", curr + prev, div);
    printf("delta: %lu/%lu\n", curr, div);
    prev = prev + curr;
  }

  return 0;
}
/* --- last line of demo1.c --- */
