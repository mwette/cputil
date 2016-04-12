/* cputil.h - part of valgrind cpu utilization tool

   Copyright (C) 2013,2016 Matthew R. Wette
   mwette@alumni.caltech.edu

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.
 */
#ifndef __cputil_h__
#define __cputil_h__

/* Use the macros CU_REGTHR, CU_CLRCTR, CU_GETCTR and CU_GETMUL like the 
 * following function declarations:
 *       void CPUTIL_REG_THR();            - register the calling thread
 *       void CPUTIL_CLR_CTR();            - zero the counter
 *       unsigned long CPUTIL_GET_CTR();   - return counter value
 *       unsigned long CPUTIL_GET_DIV();   - return the divisor
 *
 * Example:
 *       unsigned long foo_cnt;
 *       unsigned long foo_div;
 *
 *       CPUTIL_REG_THR();                 - in task initialization
 *       
 *       CPUTIL_CLR_CTR();                 - before each call
 *       foo();
 *       foo_cnt = CPUTIL_GET_CTR();       - after each call
 *
 *       foo_div = CPUTIL_GET_DIV();       - to print results
 *       printf("counts=%u/%d\n", foo_cnt, foo_div);
 *
 * If any counter grows above ULONG_MAX>>1 cputil stops counting and generates
 * an error message after the client program completes execution.
 */

#define CPUTIL_REG_THR     CU_REGTHR
#define CPUTIL_CLR_CTR     CU_CLRCTR
#define CPUTIL_GET_CTR     CU_GETCTR
#define CPUTIL_GET_DIV     CU_GETDIV

/* backward compatiblity */
#define CPUTIL_CLR_CTR1    CU_CLRCTR
#define CPUTIL_GET_CTR1    CU_GETCTR

/* autocoding: beg */
#if defined(__APPLE__) && defined(__i386__)

#define CU_REGTHR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550001); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550002); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550003); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550004); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#elif defined(__APPLE__) && defined(__x86_64__)

#define CU_REGTHR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550001); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550002); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550003); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550004); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#elif defined(__MINGW32__) || defined(__CYGWIN32__) || (defined(_WIN32) && \
	 defined(_M_IX86))

#define CU_REGTHR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550001); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550002); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550003); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550004); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#elif defined(__linux__) && defined(__i386__)

#define CU_REGTHR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550001); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550002); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550003); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550004); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile( \
	"roll $3,  %%edi ; roll $13, %%edi\n\t"  \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#elif defined(__linux__) && defined(__x86_64__)

#define CU_REGTHR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550001); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550002); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550003); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({ volatile unsigned long int _zzq_args[6]; \
	volatile unsigned long int _zzq_result; _zzq_args[0] = \
	(unsigned long int)(0x43550004); _zzq_args[1] = (unsigned \
	long int)(0); _zzq_args[2] = (unsigned long int)(0); \
	_zzq_args[3] = (unsigned long int)(0); _zzq_args[4] = \
	(unsigned long int)(0); _zzq_args[5] = (unsigned long \
	int)(0); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#elif defined(__linux__) && defined(__powerpc__) && !defined(__powerpc64__)

#define CU_REGTHR()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550001); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,31  ; rlwinm 0,0,13,0,31\n\t"  \
	"rlwinm 0,0,29,0,31 ; rlwinm 0,0,19,0,31\n\t"  \
	"or 1,1,1\n\t" "mr %0,3" : "=b" (_zzq_result) : "b" (0), \
	"b" (_zzq_ptr) : "cc", "memory", "r3", "r4"); _zzq_result; \
	})

#define CU_CLRCTR()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550002); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,31  ; rlwinm 0,0,13,0,31\n\t"  \
	"rlwinm 0,0,29,0,31 ; rlwinm 0,0,19,0,31\n\t"  \
	"or 1,1,1\n\t" "mr %0,3" : "=b" (_zzq_result) : "b" (0), \
	"b" (_zzq_ptr) : "cc", "memory", "r3", "r4"); _zzq_result; \
	})

#define CU_GETCTR()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550003); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,31  ; rlwinm 0,0,13,0,31\n\t"  \
	"rlwinm 0,0,29,0,31 ; rlwinm 0,0,19,0,31\n\t"  \
	"or 1,1,1\n\t" "mr %0,3" : "=b" (_zzq_result) : "b" (0), \
	"b" (_zzq_ptr) : "cc", "memory", "r3", "r4"); _zzq_result; \
	})

#define CU_GETDIV()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550004); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,31  ; rlwinm 0,0,13,0,31\n\t"  \
	"rlwinm 0,0,29,0,31 ; rlwinm 0,0,19,0,31\n\t"  \
	"or 1,1,1\n\t" "mr %0,3" : "=b" (_zzq_result) : "b" (0), \
	"b" (_zzq_ptr) : "cc", "memory", "r3", "r4"); _zzq_result; \
	})

#elif defined(__linux__) && defined(__powerpc__) && defined(__powerpc64__)

#define CU_REGTHR()	\
	__extension__ ({ unsigned long int _zzq_args[6]; unsigned \
	long int _zzq_result; unsigned long int* _zzq_ptr; \
	_zzq_args[0] = (unsigned long int)(0x43550001); \
	_zzq_args[1] = (unsigned long int)(0); _zzq_args[2] = \
	(unsigned long int)(0); _zzq_args[3] = (unsigned long \
	int)(0); _zzq_args[4] = (unsigned long int)(0); \
	_zzq_args[5] = (unsigned long int)(0); _zzq_ptr = \
	_zzq_args; __asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t" \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({ unsigned long int _zzq_args[6]; unsigned \
	long int _zzq_result; unsigned long int* _zzq_ptr; \
	_zzq_args[0] = (unsigned long int)(0x43550002); \
	_zzq_args[1] = (unsigned long int)(0); _zzq_args[2] = \
	(unsigned long int)(0); _zzq_args[3] = (unsigned long \
	int)(0); _zzq_args[4] = (unsigned long int)(0); \
	_zzq_args[5] = (unsigned long int)(0); _zzq_ptr = \
	_zzq_args; __asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t" \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({ unsigned long int _zzq_args[6]; unsigned \
	long int _zzq_result; unsigned long int* _zzq_ptr; \
	_zzq_args[0] = (unsigned long int)(0x43550003); \
	_zzq_args[1] = (unsigned long int)(0); _zzq_args[2] = \
	(unsigned long int)(0); _zzq_args[3] = (unsigned long \
	int)(0); _zzq_args[4] = (unsigned long int)(0); \
	_zzq_args[5] = (unsigned long int)(0); _zzq_ptr = \
	_zzq_args; __asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t" \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({ unsigned long int _zzq_args[6]; unsigned \
	long int _zzq_result; unsigned long int* _zzq_ptr; \
	_zzq_args[0] = (unsigned long int)(0x43550004); \
	_zzq_args[1] = (unsigned long int)(0); _zzq_args[2] = \
	(unsigned long int)(0); _zzq_args[3] = (unsigned long \
	int)(0); _zzq_args[4] = (unsigned long int)(0); \
	_zzq_args[5] = (unsigned long int)(0); _zzq_ptr = \
	_zzq_args; __asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t" \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#elif defined(__linux__) && defined(__arm__)

#define CU_REGTHR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550001); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("mov r3, %1\n\t"  \
	"mov r4, %2\n\t"  \
	"mov r12, r12, ror #3  ; mov r12, r12, ror #13 \n\t"  \
	"mov r12, r12, ror #29 ; mov r12, r12, ror #19 \n\t"  \
	"orr r10, r10, r10\n\t" "mov %0, r3" : "=r" (_zzq_result) : \
	"r" (0), "r" (&_zzq_args[0]) : "cc","memory", "r3", "r4"); \
	_zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550002); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("mov r3, %1\n\t"  \
	"mov r4, %2\n\t"  \
	"mov r12, r12, ror #3  ; mov r12, r12, ror #13 \n\t"  \
	"mov r12, r12, ror #29 ; mov r12, r12, ror #19 \n\t"  \
	"orr r10, r10, r10\n\t" "mov %0, r3" : "=r" (_zzq_result) : \
	"r" (0), "r" (&_zzq_args[0]) : "cc","memory", "r3", "r4"); \
	_zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550003); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("mov r3, %1\n\t"  \
	"mov r4, %2\n\t"  \
	"mov r12, r12, ror #3  ; mov r12, r12, ror #13 \n\t"  \
	"mov r12, r12, ror #29 ; mov r12, r12, ror #19 \n\t"  \
	"orr r10, r10, r10\n\t" "mov %0, r3" : "=r" (_zzq_result) : \
	"r" (0), "r" (&_zzq_args[0]) : "cc","memory", "r3", "r4"); \
	_zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550004); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("mov r3, %1\n\t"  \
	"mov r4, %2\n\t"  \
	"mov r12, r12, ror #3  ; mov r12, r12, ror #13 \n\t"  \
	"mov r12, r12, ror #29 ; mov r12, r12, ror #19 \n\t"  \
	"orr r10, r10, r10\n\t" "mov %0, r3" : "=r" (_zzq_result) : \
	"r" (0), "r" (&_zzq_args[0]) : "cc","memory", "r3", "r4"); \
	_zzq_result; })

#elif defined(__linux__) && defined(__x390__) && defined(__s390x__)

#define CU_REGTHR()	\
	(0)

#define CU_CLRCTR()	\
	(0)

#define CU_GETCTR()	\
	(0)

#define CU_GETDIV()	\
	(0)

#elif defined(__linux__) && defined(__mips__)

#define CU_REGTHR()	\
	__extension__ ({ volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550001); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("move $11, %1\n\t" \
	"move $12, %2\n\t" "srl $0, $0, 13\n\t"  \
	"srl $0, $0, 29\n\t" "srl $0, $0, 3\n\t"  \
	"srl $0, $0, 19\n\t" "or $13, $13, $13\n\t"  \
	"move %0, $11\n\t" : "=r" (_zzq_result) : "r" (0), "r" \
	(&_zzq_args[0]) : "$11", "$12"); _zzq_result; })

#define CU_CLRCTR()	\
	__extension__ ({ volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550002); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("move $11, %1\n\t" \
	"move $12, %2\n\t" "srl $0, $0, 13\n\t"  \
	"srl $0, $0, 29\n\t" "srl $0, $0, 3\n\t"  \
	"srl $0, $0, 19\n\t" "or $13, $13, $13\n\t"  \
	"move %0, $11\n\t" : "=r" (_zzq_result) : "r" (0), "r" \
	(&_zzq_args[0]) : "$11", "$12"); _zzq_result; })

#define CU_GETCTR()	\
	__extension__ ({ volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550003); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("move $11, %1\n\t" \
	"move $12, %2\n\t" "srl $0, $0, 13\n\t"  \
	"srl $0, $0, 29\n\t" "srl $0, $0, 3\n\t"  \
	"srl $0, $0, 19\n\t" "or $13, $13, $13\n\t"  \
	"move %0, $11\n\t" : "=r" (_zzq_result) : "r" (0), "r" \
	(&_zzq_args[0]) : "$11", "$12"); _zzq_result; })

#define CU_GETDIV()	\
	__extension__ ({ volatile unsigned int _zzq_args[6]; \
	volatile unsigned int _zzq_result; _zzq_args[0] = (unsigned \
	int)(0x43550004); _zzq_args[1] = (unsigned int)(0); \
	_zzq_args[2] = (unsigned int)(0); _zzq_args[3] = (unsigned \
	int)(0); _zzq_args[4] = (unsigned int)(0); _zzq_args[5] = \
	(unsigned int)(0); __asm__ volatile("move $11, %1\n\t" \
	"move $12, %2\n\t" "srl $0, $0, 13\n\t"  \
	"srl $0, $0, 29\n\t" "srl $0, $0, 3\n\t"  \
	"srl $0, $0, 19\n\t" "or $13, $13, $13\n\t"  \
	"move %0, $11\n\t" : "=r" (_zzq_result) : "r" (0), "r" \
	(&_zzq_args[0]) : "$11", "$12"); _zzq_result; })

#else
#define CU_REGTHR() 0
#define CU_CLRCTR() 0
#define CU_GETCTR() 0
#define CU_GETDIV() 0
#endif
/* autocoding: end */

#endif
/* --- last line --- */
