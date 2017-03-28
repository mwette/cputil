/* cputildefs.h - execution profiling under valgrind

   Copyright (C) 2013,2016 Matthew R. Wette
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
      VG_USERREQ__CU_REGTHR,
      VG_USERREQ__CU_CLRCTR,
      VG_USERREQ__CU_GETCTR,
      VG_USERREQ__CU_GETDIV,
      VG_USERREQ__CU_CINVAL,
      VG_USERREQ__CU_CFLUSH,
      VG_USERREQ__END_OF_CPUTIL = VG_USERREQ_TOOL_BASE('C','U') + 256
   } Vg_CputilClientRequest;

/* 
 * void CU_REGTHR(void);
 * void CU_CLRCTR(void);
 * unsigned long CU_GETCTR();
 * unsigned long CU_GETDIV();
 * void CU_CINVAL(unsigned long id);
 * void CU_CFLUSH(unsigned long id);
 */

#define CU_REGTHR()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_REGTHR, 0,0,0,0,0)

#define CU_CLRCTR()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_CLRCTR, 0,0,0,0,0)

#define CU_GETCTR()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_GETCTR, 0,0,0,0,0)

#define CU_GETDIV()                     \
   VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_GETDIV, 0,0,0,0,0)

#define CU_CINVAL(id)                   \
  VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_CINVAL, (id),0,0,0,0)

#define CU_CFLUSH(id)                   \
  VALGRIND_DO_CLIENT_REQUEST_EXPR(0, VG_USERREQ__CU_CFLUSH, (id),0,0,0,0)

#endif
/* --- last line --- */
