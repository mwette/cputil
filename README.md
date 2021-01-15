# cputil - a valgrind tool for estimating CPU utilization

Copyright (C) 2016-2017,2021 -- Matthew R. Wette.

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.3 or
any later version published by the Free Software Foundation; with no
Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.  A
copy of the license is included with the distribution as COPYING.DOC.

The cputil program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

## Introduction

This valgrind tool provides approximate cycle counts for user-defined
regions of code.  It works on a per-thread basis.  The following program,
when executed under cputil/valgrind will report the estimated number of 
clock cycles required to execute foo().  The default processor model is
a PowerPC750, but can be changed with a user-loadable table.

```
#include <stdio.h>
#include "cputil.h"
int main() {
  long cycles;
  CU_REGTHR();                /* register the current thread */
  for (i = 0; i < 5; i++) {
    CU_CLRCTR();              /* clear the cycle counter */
    foo(i);
    cycles += CU_GETCTR();    /* read the cycle counter */
    bar(i);
  }
  printf("clk-cycles=%d\n", cycles)
}
```

## Installation

This works with valgrind-3.16.1.  I assume it will work with 3.16.X.
To work with older versions of valgrind, try other git branches.

To install:

1. Download and unpack valgrind-3.16.X

2. Clone or otherwise install this cputil distribution in a directory
   called *cputil* under valgrind-3.16.X

3. In the subdirectory cputil, execute *patch1*:
   ```
   $ pwd
   .../valgring-3.16.X/cputil
   $ ./patch1
   ```

   Note that the above tries to edit the autogenerted configure and
   Makefile.in files.  If that does not work then you can try
   ```
   $ cd ..
   $ patch -b -p0 cputil/patch/valgrind1.patch
   $ ./autogen.sh
   ```

4a. In the valgrind top directory run configure, make and install:
   ```
   $ ./configure --prefix=/usr/local
   $ make 
   $ sudo make install
   ```

On macOS, you may need to perform the following steps:
   ```
   $ CFLAGS="" ./configure --prefix=/usr/local \
     ./configure --enable-only64bit --prefix=/usr/local
   ```

4b. Optionally, if you want a minimal distribution, do use this sequence:
   ```
   $ ./configure --prefix=/usr/local
   $ mkdir -p /var/tmp/bindist
   $ make DESTDIR=/var/tmp/bindist pkglibdir=/usr/local/lib/cputil
   $ make DESTDIR=/var/tmp/bindist pkglibdir=/usr/local/lib/cputil install
   $ sh cputil/mindist /var/tmp/bindist /usr/local/lib/cputil
   ```
The above will generate a distribution.  To install, become root if necessary,
then execute the following:
   ```
   # cd /var/tmp/bindist
   # cd usr/local
   # tar cf - . | (cd /usr/local; tar xvf -)
   ```

### dumping the op-count tables
To dump the internal tables execute
   ```
   $ valgrind --tool=cputil --dump-op-tables=dump.cut true
   ```
There is no way to dump the op-tables w/o specifing a program to so
we use use /bin/true.


### loading user-defined op-count tables
A dumped op table can be edited for clock counts and used for another run:
   ```
   $ valgrind --tool=cputil --load-op-tables=dump.cut my_program
   ```

### Threads

Support for multi-threaded code has been added.


### Demo

Look in cputil/tests/demo1.c

### Other issues

If you see the following error during compilation then your version of 
valgrind still does works to MPI1.  

  libmpiwrap.c: In function 'showTy':
  libmpiwrap.c:281:19: error: 'MPI_UB' undeclared (first use in this function);
      did you mean 'MPI_IO'?
    281 |    else if (ty == MPI_UB)             fprintf(f,"UB");
        |                   ^~~~~~
        |                   MPI_IO

The included patch
	0001-Drop-MPI-1-support.patch
from
	https://bugs.kde.org/show_bug.cgi?id=401416#c3
should help.  To apply, from valgring top directory, 
   ```
   $ patch -b -p1 < cputil/patch/0001-Drop-MPI-1-support.patch
   ```

