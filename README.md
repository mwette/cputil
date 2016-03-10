# cputil - a valgrind tool for estimating CPU utilization

Copyright (C) 2016 -- Matthew R. Wette.

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.3 or
any later version published by the Free Software Foundation; with no
Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.  A
copy of the license is included with the distribution as COPYING.DOC.

The cputil program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

## Installation

This works with valgrind-3.11.0.  Goal will be to work w/ >3.0.

To install:

1. Download and unpack valgring-3.11.0

2. Clone or otherwise install this cputil distribution in a directory
   called *cputil* under valgrind-3.11.0

3. in the subdirectory cputil, execute *patch1*:
   ```
   $ pwd
   .../valgring-3.11.0/cputil
   $ ./patch1
   ```

4. In the valgrind top direcotry run configure, make and install


## Notes

This valgrind tool provides approximate cycle counts for user-defined
regions of code.

### dumping the op-count tables
There is no way to dump the op-tables w/o specifing a program to run the
tool on.  That is, "valgrind --tool=cputil --dump-op_tables=abc won't work.
You need to do "valgrind --tool=cputil --dump-op_tables=abc /bin/echo".

* debug options: valgrind --help-debug
* idea: for timings unknown, set to 0 and print notice after

### Printing out blocks

Try running with "--vex-guest-chase-thresh=0 --trace-flags=10000000
--trace-notbelow=999999".  This should print one line for each block
translated, and that includes the address.  Re-run w/ 999999 changed 
to highest bb number shown.

### Threads

No support for multi-threaded code.   Here are some calls provided by VG:

```
  extern ThreadId VG_(get_running_tid) ( void );
```

