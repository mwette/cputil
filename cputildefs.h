/* cputildefs.h - execution profiling under valgrind

   Copyright (C) 2013-2013 Matthew R. Wette
   mwette@alumni.caltech.edu
 
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
 */
#ifndef __CPUTILDEFS_H
#define __CPUTILDEFS_H

#include "valgrind.h"

typedef
   enum { 
      VG_USERREQ__CU_NOOP = VG_USERREQ_TOOL_BASE('C','U'),
      VG_USERREQ__CU_CLRCTR1,
      VG_USERREQ__CU_GETCTR1,
      VG_USERREQ__CU_SETCTR2PTR,
      VG_USERREQ__END_OF_XPROF = VG_USERREQ_TOOL_BASE('C','U') + 256
   } Vg_CputilClientRequest;

/* 
 * void CU_CLRCTR1(void);
 * unsigned long CU_GETCTR1();
 * void CU_SETCTR2PTR(unsigned long *ptr);
 */

#define CU_CLRCTR1()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_CLRCTR1, 0,0,0,0,0)

#define CU_GETCTR1()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_GETCTR1, 0,0,0,0,0)

#define CU_SETCTR2PTR(PTR)             \
   VALGRIND_DO_CLIENT_REQUEST_STMT(VG_USERREQ__CU_SETCTR2PTR, PTR, 0,0,0,0)

#endif
/* --- last line of cputildefs.h --- */
