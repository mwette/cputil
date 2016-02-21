/* cputil.h - part of valgrind cpu utilization tool

   Copyright (C) 2013,2015 Matthew R. Wette
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
#ifndef __cputil_h__
#define __cputil_h__

/* There are two methods for interaction.   
 *  Method 1: Use the macros CU_CLRCTR and CU_GETCTR like the functions:
 *       void CU_CLRCTR1();             - zero the counter
 *       unsigned long CU_GETCTR1();     - return counter value
 *     Usage:
 *       unsigned long foo_cnt;
 *       CU_CLRCTR1();
 *       foo();
 *       foo_cnt = CU_GETCTR1();
 *       printf("counts=%u\n", foo_cnt);
 *
 *  Method 2: Use the macro CU_SETCTR2PTR like the funcntion:
 *       void CU_SETCTR2PTR(unsigned long *ctrptr);
 *     Usage: 
 *       volatile unsigned long foo_cnt = 0;
 *       CU_SETCTR2PTR(&foo_cnt);
 *       foo();
 *       CU_SETCTR2PTR(0);
 *       printf("counts=%u\n", foo_cnt);
 *     Some concerns have been received about safety of this approach.
 *
 * If any counter grows above ULONG_MAX/2 cputil stops counting and generates
 * an message after the client program completes execution.
 */

#define CPUTIL_CLR_CTR1    CU_CLRCTR1
#define CPUTIL_GET_CTR1    CU_CLRCTR1
#define CPUTIL_SET_COUNTER CU_SETCTR2PTR

/* autocoding: beg */
#if defined(__APPLE__) && defined(__i386__)

#define CU_CLRCTR1()	\
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

#define CU_GETCTR1()	\
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

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({volatile unsigned int \
	_zzq_args[6]; volatile unsigned int _zzq_result; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	__asm__ volatile("roll $3,  %%edi ; roll $13, %%edi\n\t" \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; }); } while (0)

#elif defined(__APPLE__) && defined(__x86_64__)

#define CU_CLRCTR1()	\
	__extension__ ({ volatile unsigned long long int \
	_zzq_args[6]; volatile unsigned long long int _zzq_result; \
	_zzq_args[0] = (unsigned long long int)(0x43550001); \
	_zzq_args[1] = (unsigned long long int)(0); _zzq_args[2] = \
	(unsigned long long int)(0); _zzq_args[3] = (unsigned long \
	long int)(0); _zzq_args[4] = (unsigned long long int)(0); \
	_zzq_args[5] = (unsigned long long int)(0); __asm__ \
	volatile("rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR1()	\
	__extension__ ({ volatile unsigned long long int \
	_zzq_args[6]; volatile unsigned long long int _zzq_result; \
	_zzq_args[0] = (unsigned long long int)(0x43550002); \
	_zzq_args[1] = (unsigned long long int)(0); _zzq_args[2] = \
	(unsigned long long int)(0); _zzq_args[3] = (unsigned long \
	long int)(0); _zzq_args[4] = (unsigned long long int)(0); \
	_zzq_args[5] = (unsigned long long int)(0); __asm__ \
	volatile("rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({ volatile unsigned long long \
	int _zzq_args[6]; volatile unsigned long long int \
	_zzq_result; _zzq_args[0] = (unsigned long long \
	int)((0x43550003)); _zzq_args[1] = (unsigned long long \
	int)((PTR)); _zzq_args[2] = (unsigned long long int)((0)); \
	_zzq_args[3] = (unsigned long long int)((0)); _zzq_args[4] \
	= (unsigned long long int)((0)); _zzq_args[5] = (unsigned \
	long long int)((0)); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; }); } while (0)

#elif defined(__MINGW32__) || defined(__CYGWIN32__) || (defined(_WIN32) && \
	 defined(_M_IX86))

#define CU_CLRCTR1()	\
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

#define CU_GETCTR1()	\
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

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({volatile unsigned int \
	_zzq_args[6]; volatile unsigned int _zzq_result; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	__asm__ volatile("roll $3,  %%edi ; roll $13, %%edi\n\t" \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__i386__)

#define CU_CLRCTR1()	\
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

#define CU_GETCTR1()	\
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

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({volatile unsigned int \
	_zzq_args[6]; volatile unsigned int _zzq_result; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	__asm__ volatile("roll $3,  %%edi ; roll $13, %%edi\n\t" \
	"roll $29, %%edi ; roll $19, %%edi\n\t" "xchgl %%ebx,%%ebx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__x86_64__)

#define CU_CLRCTR1()	\
	__extension__ ({ volatile unsigned long long int \
	_zzq_args[6]; volatile unsigned long long int _zzq_result; \
	_zzq_args[0] = (unsigned long long int)(0x43550001); \
	_zzq_args[1] = (unsigned long long int)(0); _zzq_args[2] = \
	(unsigned long long int)(0); _zzq_args[3] = (unsigned long \
	long int)(0); _zzq_args[4] = (unsigned long long int)(0); \
	_zzq_args[5] = (unsigned long long int)(0); __asm__ \
	volatile("rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_GETCTR1()	\
	__extension__ ({ volatile unsigned long long int \
	_zzq_args[6]; volatile unsigned long long int _zzq_result; \
	_zzq_args[0] = (unsigned long long int)(0x43550002); \
	_zzq_args[1] = (unsigned long long int)(0); _zzq_args[2] = \
	(unsigned long long int)(0); _zzq_args[3] = (unsigned long \
	long int)(0); _zzq_args[4] = (unsigned long long int)(0); \
	_zzq_args[5] = (unsigned long long int)(0); __asm__ \
	volatile("rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; })

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({ volatile unsigned long long \
	int _zzq_args[6]; volatile unsigned long long int \
	_zzq_result; _zzq_args[0] = (unsigned long long \
	int)((0x43550003)); _zzq_args[1] = (unsigned long long \
	int)((PTR)); _zzq_args[2] = (unsigned long long int)((0)); \
	_zzq_args[3] = (unsigned long long int)((0)); _zzq_args[4] \
	= (unsigned long long int)((0)); _zzq_args[5] = (unsigned \
	long long int)((0)); __asm__ volatile( \
	"rolq $3,  %%rdi ; rolq $13, %%rdi\n\t"  \
	"rolq $61, %%rdi ; rolq $51, %%rdi\n\t" "xchgq %%rbx,%%rbx" \
	: "=d" (_zzq_result) : "a" (&_zzq_args[0]), "0" (0) : "cc", \
	"memory" ); _zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__powerpc__) && !defined(__powerpc64__)

#define CU_CLRCTR1()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550001); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,0  ; rlwinm 0,0,13,0,0\n\t"  \
	"rlwinm 0,0,29,0,0 ; rlwinm 0,0,19,0,0\n\t" "or 1,1,1\n\t" \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_GETCTR1()	\
	__extension__ ({ unsigned int _zzq_args[6]; unsigned int \
	_zzq_result; unsigned int* _zzq_ptr; _zzq_args[0] = \
	(unsigned int)(0x43550002); _zzq_args[1] = (unsigned \
	int)(0); _zzq_args[2] = (unsigned int)(0); _zzq_args[3] = \
	(unsigned int)(0); _zzq_args[4] = (unsigned int)(0); \
	_zzq_args[5] = (unsigned int)(0); _zzq_ptr = _zzq_args; \
	__asm__ volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rlwinm 0,0,3,0,0  ; rlwinm 0,0,13,0,0\n\t"  \
	"rlwinm 0,0,29,0,0 ; rlwinm 0,0,19,0,0\n\t" "or 1,1,1\n\t" \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({ unsigned int _zzq_args[6]; \
	unsigned int _zzq_result; unsigned int* _zzq_ptr; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	_zzq_ptr = _zzq_args; __asm__ volatile("mr 3,%1\n\t"  \
	"mr 4,%2\n\t" "rlwinm 0,0,3,0,0  ; rlwinm 0,0,13,0,0\n\t" \
	"rlwinm 0,0,29,0,0 ; rlwinm 0,0,19,0,0\n\t" "or 1,1,1\n\t" \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__powerpc__) && defined(__powerpc64__)

#define CU_CLRCTR1()	\
	__extension__ ({ unsigned long long int _zzq_args[6]; \
	unsigned long long int _zzq_result; unsigned long long int* \
	_zzq_ptr; _zzq_args[0] = (unsigned long long \
	int)(0x43550001); _zzq_args[1] = (unsigned long long \
	int)(0); _zzq_args[2] = (unsigned long long int)(0); \
	_zzq_args[3] = (unsigned long long int)(0); _zzq_args[4] = \
	(unsigned long long int)(0); _zzq_args[5] = (unsigned long \
	long int)(0); _zzq_ptr = _zzq_args; __asm__ volatile( \
	"mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_GETCTR1()	\
	__extension__ ({ unsigned long long int _zzq_args[6]; \
	unsigned long long int _zzq_result; unsigned long long int* \
	_zzq_ptr; _zzq_args[0] = (unsigned long long \
	int)(0x43550002); _zzq_args[1] = (unsigned long long \
	int)(0); _zzq_args[2] = (unsigned long long int)(0); \
	_zzq_args[3] = (unsigned long long int)(0); _zzq_args[4] = \
	(unsigned long long int)(0); _zzq_args[5] = (unsigned long \
	long int)(0); _zzq_ptr = _zzq_args; __asm__ volatile( \
	"mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; })

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({ unsigned long long int \
	_zzq_args[6]; unsigned long long int _zzq_result; unsigned \
	long long int* _zzq_ptr; _zzq_args[0] = (unsigned long long \
	int)((0x43550003)); _zzq_args[1] = (unsigned long long \
	int)((PTR)); _zzq_args[2] = (unsigned long long int)((0)); \
	_zzq_args[3] = (unsigned long long int)((0)); _zzq_args[4] \
	= (unsigned long long int)((0)); _zzq_args[5] = (unsigned \
	long long int)((0)); _zzq_ptr = _zzq_args; __asm__ \
	volatile("mr 3,%1\n\t" "mr 4,%2\n\t"  \
	"rotldi 0,0,3  ; rotldi 0,0,13\n\t"  \
	"rotldi 0,0,61 ; rotldi 0,0,51\n\t" "or 1,1,1\n\t"  \
	"mr %0,3" : "=b" (_zzq_result) : "b" (0), "b" (_zzq_ptr) : \
	"cc", "memory", "r3", "r4"); _zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__arm__)

#define CU_CLRCTR1()	\
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

#define CU_GETCTR1()	\
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

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({volatile unsigned int \
	_zzq_args[6]; volatile unsigned int _zzq_result; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	__asm__ volatile("mov r3, %1\n\t" "mov r4, %2\n\t"  \
	"mov r12, r12, ror #3  ; mov r12, r12, ror #13 \n\t"  \
	"mov r12, r12, ror #29 ; mov r12, r12, ror #19 \n\t"  \
	"orr r10, r10, r10\n\t" "mov %0, r3" : "=r" (_zzq_result) : \
	"r" (0), "r" (&_zzq_args[0]) : "cc","memory", "r3", "r4"); \
	_zzq_result; }); } while (0)

#elif defined(__linux__) && defined(__x390__) && defined(__s390x__)

#define CU_CLRCTR1()	\
	(0)

#define CU_GETCTR1()	\
	(0)

#define CU_SETCTR2PTR(PTR)	\
	do { (void) (0); } while (0)

#elif defined(__linux__) && defined(__mips__)

#define CU_CLRCTR1()	\
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
	(&_zzq_args[0]) : "cc","memory", "t3", "t4"); _zzq_result; \
	})

#define CU_GETCTR1()	\
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
	(&_zzq_args[0]) : "cc","memory", "t3", "t4"); _zzq_result; \
	})

#define CU_SETCTR2PTR(PTR)	\
	do { (void) __extension__ ({ volatile unsigned int \
	_zzq_args[6]; volatile unsigned int _zzq_result; \
	_zzq_args[0] = (unsigned int)((0x43550003)); _zzq_args[1] = \
	(unsigned int)((PTR)); _zzq_args[2] = (unsigned int)((0)); \
	_zzq_args[3] = (unsigned int)((0)); _zzq_args[4] = \
	(unsigned int)((0)); _zzq_args[5] = (unsigned int)((0)); \
	__asm__ volatile("move $11, %1\n\t" "move $12, %2\n\t"  \
	"srl $0, $0, 13\n\t" "srl $0, $0, 29\n\t"  \
	"srl $0, $0, 3\n\t" "srl $0, $0, 19\n\t"  \
	"or $13, $13, $13\n\t" "move %0, $11\n\t" : "=r" \
	(_zzq_result) : "r" (0), "r" (&_zzq_args[0]) : \
	"cc","memory", "t3", "t4"); _zzq_result; }); } while (0)

#else
#define CU_CLRCTR1() 0
#define CU_GETCTR1() 0
#define CU_SETCTR2PTR(PTR) 0
#endif
/* autocoding: end */

#endif
/* --- last line --- */
