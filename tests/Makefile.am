
include $(top_srcdir)/Makefile.tool-tests.am

SUBDIRS = .
DIST_SUBDIRS = .

check_PROGRAMS = demo1

EXTRA_DIST = \
	demo1a.vgtest demo1b.vgtest

AM_CFLAGS   += -I..

demo1_LDADD = -lm
