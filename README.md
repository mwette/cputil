# cputil - a valgrind tool for estimating CPU utilization

Copyright (C) 2016 -- Matthew R. Wette.

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.3 or
any later version published by the Free Software Foundation; with no
Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.  A
copy of the license is included with the distribution as COPYING.DOC.


## Installation

To install:
1. Download and unpack valgring-3.0.11
2. clone or otherwise install this cputil distribution in a directory
   called *cputil* under valgrind-3.0.11
3. in the subdirectory cputil, execute *patch1*:
   ```
   $ pwd
   .../valgring-3.0.11/cputil
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

===========================================--=================================
