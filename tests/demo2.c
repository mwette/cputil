/* demo2.c */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "xprofreq.h"

typedef struct {
  int mode;				/* 0: idle, 1: acquire, 2: track */
  double x[3];				/* state */
} con_t;

con_t con1;

void
con_exec(con_t *self, double *y, double *u)
{
  double t;

  t = (3.0 + x)/(9.4 + y);
  return t;
}

void
plt_exec(double *y, double *x, double *u)
{
}

unsigned long con_cnt[3], plt_ctr;

int
main()
{
  int i; 
  unsigned long prev;
  double z;
  unsigned long vals[10];
  int difs[10];

  counter1 = 0;

  CPUTIL_SET_COUNTER(&counter1);

  for (i = 0; i < 1000; i++) {
    con_exec(&con1);
  }
  prev = 0;
  for (i = 0; i < 10; i++) {
    z = foo(1.2, 3.6);
    vals[i] = counter1;
    difs[i] = counter1 - prev;
    prev = counter1;
  }
  for (i = 0; i < 10; i++) {
    printf("count: %lu\t", vals[i]);
    printf("delta: %d\n", difs[i]);
  }
}

