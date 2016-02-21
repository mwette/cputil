# cputilREADME for valgrind/cputil - execution profiler under valgrind

Developed by Matt Wette, mwette@alumni.caltech.edu.

This valgrind tool provides approximate cycle counts for user-defined
regions of code.

==============================================================================

There is no way to dump the op-tables w/o specifing a program to run the
tool on.  That is, "valgrind --tool=cputil --dump-op_tables=abc won't work.
You need to do "valgrind --tool=cputil --dump-op_tables=abc /bin/echo".

==============================================================================

Notes:
* valgrind.org uses svn
* debug options: valgrind --help-debug
* idea: for timings unknown, set to 0 and print notice after

Printing out blocks
~~~~~~~~~~~~~~~~~~~
Try running with "--vex-guest-chase-thresh=0 --trace-flags=10000000
--trace-notbelow=999999".  This should print one line for each block
translated, and that includes the address.  Re-run w/ 999999 changed 
to highest bb number shown.

===========================================--=================================
