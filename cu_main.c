/* cu_main.c - execution profiler under valgrind

   Copyright (C) 2013,2015,2016 Matthew R. Wette
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
   along with this program; if not, see http://www.gnu.org/licenses,
   or write to the Free Software Foundation, Inc., 59 Temple Place, 
   Suite 330, Boston, MA 02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
 */
#include <limits.h>			/* need LONG_MAX */
#include "pub_tool_basics.h"
#include "pub_tool_vki.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_libcfile.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_options.h"
#include "pub_tool_machine.h"
#include "cputildefs.h"

static const HChar cu_version[] = "v160121a";

/*------------------------------------------------------------*/
/*--- Instrumentation                                      ---*/
/*------------------------------------------------------------*/

/* count_tables: beg */
#define NUM_OP 828
static const HChar *cu_op_names[] = {
   "Iop_INVALID", "Iop_Add8", "Iop_Add16", "Iop_Add32", "Iop_Add64",
   "Iop_Sub8", "Iop_Sub16", "Iop_Sub32", "Iop_Sub64", "Iop_Mul8",
   "Iop_Mul16", "Iop_Mul32", "Iop_Mul64", "Iop_Or8", "Iop_Or16", "Iop_Or32",
   "Iop_Or64", "Iop_And8", "Iop_And16", "Iop_And32", "Iop_And64",
   "Iop_Xor8", "Iop_Xor16", "Iop_Xor32", "Iop_Xor64", "Iop_Shl8",
   "Iop_Shl16", "Iop_Shl32", "Iop_Shl64", "Iop_Shr8", "Iop_Shr16",
   "Iop_Shr32", "Iop_Shr64", "Iop_Sar8", "Iop_Sar16", "Iop_Sar32",
   "Iop_Sar64", "Iop_CmpEQ8", "Iop_CmpEQ16", "Iop_CmpEQ32", "Iop_CmpEQ64",
   "Iop_CmpNE8", "Iop_CmpNE16", "Iop_CmpNE32", "Iop_CmpNE64", "Iop_Not8",
   "Iop_Not16", "Iop_Not32", "Iop_Not64", "Iop_CasCmpEQ8", "Iop_CasCmpEQ16",
   "Iop_CasCmpEQ32", "Iop_CasCmpEQ64", "Iop_CasCmpNE8", "Iop_CasCmpNE16",
   "Iop_CasCmpNE32", "Iop_CasCmpNE64", "Iop_MullS8", "Iop_MullS16",
   "Iop_MullS32", "Iop_MullS64", "Iop_MullU8", "Iop_MullU16", "Iop_MullU32",
   "Iop_MullU64", "Iop_Clz64", "Iop_Clz32", "Iop_Ctz64", "Iop_Ctz32",
   "Iop_CmpLT32S", "Iop_CmpLT64S", "Iop_CmpLE32S", "Iop_CmpLE64S",
   "Iop_CmpLT32U", "Iop_CmpLT64U", "Iop_CmpLE32U", "Iop_CmpLE64U",
   "Iop_CmpNEZ8", "Iop_CmpNEZ16", "Iop_CmpNEZ32", "Iop_CmpNEZ64",
   "Iop_CmpwNEZ32", "Iop_CmpwNEZ64", "Iop_Left8", "Iop_Left16",
   "Iop_Left32", "Iop_Left64", "Iop_Max32U", "Iop_CmpORD32U",
   "Iop_CmpORD64U", "Iop_CmpORD32S", "Iop_CmpORD64S", "Iop_DivU32",
   "Iop_DivS32", "Iop_DivU64", "Iop_DivS64", "Iop_DivU64E", "Iop_DivS64E",
   "Iop_DivU32E", "Iop_DivS32E", "Iop_DivModU64to32", "Iop_DivModS64to32",
   "Iop_DivModU128to64", "Iop_DivModS128to64", "Iop_DivModS64to64",
   "Iop_8Uto16", "Iop_8Uto32", "Iop_8Uto64", "Iop_16Uto32", "Iop_16Uto64",
   "Iop_32Uto64", "Iop_8Sto16", "Iop_8Sto32", "Iop_8Sto64", "Iop_16Sto32",
   "Iop_16Sto64", "Iop_32Sto64", "Iop_64to8", "Iop_32to8", "Iop_64to16",
   "Iop_16to8", "Iop_16HIto8", "Iop_8HLto16", "Iop_32to16", "Iop_32HIto16",
   "Iop_16HLto32", "Iop_64to32", "Iop_64HIto32", "Iop_32HLto64",
   "Iop_128to64", "Iop_128HIto64", "Iop_64HLto128", "Iop_Not1", "Iop_32to1",
   "Iop_64to1", "Iop_1Uto8", "Iop_1Uto32", "Iop_1Uto64", "Iop_1Sto8",
   "Iop_1Sto16", "Iop_1Sto32", "Iop_1Sto64", "Iop_AddF64", "Iop_SubF64",
   "Iop_MulF64", "Iop_DivF64", "Iop_AddF32", "Iop_SubF32", "Iop_MulF32",
   "Iop_DivF32", "Iop_AddF64r32", "Iop_SubF64r32", "Iop_MulF64r32",
   "Iop_DivF64r32", "Iop_NegF64", "Iop_AbsF64", "Iop_NegF32", "Iop_AbsF32",
   "Iop_SqrtF64", "Iop_SqrtF64r32", "Iop_SqrtF32", "Iop_CmpF64",
   "Iop_CmpF32", "Iop_CmpF128", "Iop_F64toI16S", "Iop_F64toI32S",
   "Iop_F64toI64S", "Iop_F64toI64U", "Iop_F64toI32U", "Iop_I16StoF64",
   "Iop_I32StoF64", "Iop_I64StoF64", "Iop_I64UtoF64", "Iop_I64UtoF32",
   "Iop_I32UtoF64", "Iop_F32toI16S", "Iop_F32toI32S", "Iop_F32toI64S",
   "Iop_I16StoF32", "Iop_I32StoF32", "Iop_I64StoF32", "Iop_F32toF64",
   "Iop_F64toF32", "Iop_ReinterpF64asI64", "Iop_ReinterpI64asF64",
   "Iop_ReinterpF32asI32", "Iop_ReinterpI32asF32", "Iop_F64HLtoF128",
   "Iop_F128HItoF64", "Iop_F128LOtoF64", "Iop_AddF128", "Iop_SubF128",
   "Iop_MulF128", "Iop_DivF128", "Iop_NegF128", "Iop_AbsF128",
   "Iop_SqrtF128", "Iop_I32StoF128", "Iop_I64StoF128", "Iop_F32toF128",
   "Iop_F64toF128", "Iop_F128toI32S", "Iop_F128toI64S", "Iop_F128toF64",
   "Iop_F128toF32", "Iop_AtanF64", "Iop_Yl2xF64", "Iop_Yl2xp1F64",
   "Iop_PRemF64", "Iop_PRemC3210F64", "Iop_PRem1F64", "Iop_PRem1C3210F64",
   "Iop_ScaleF64", "Iop_SinF64", "Iop_CosF64", "Iop_TanF64", "Iop_2xm1F64",
   "Iop_RoundF64toInt", "Iop_RoundF32toInt", "Iop_MAddF32", "Iop_MSubF32",
   "Iop_MAddF64", "Iop_MSubF64", "Iop_MAddF64r32", "Iop_MSubF64r32",
   "Iop_Est5FRSqrt", "Iop_RoundF64toF64_NEAREST",
   "Iop_RoundF64toF64_NegINF", "Iop_RoundF64toF64_PosINF",
   "Iop_RoundF64toF64_ZERO", "Iop_TruncF64asF32", "Iop_RoundF64toF32",
   "Iop_CalcFPRF", "Iop_QAdd32S", "Iop_QSub32S", "Iop_Add16x2",
   "Iop_Sub16x2", "Iop_QAdd16Sx2", "Iop_QAdd16Ux2", "Iop_QSub16Sx2",
   "Iop_QSub16Ux2", "Iop_HAdd16Ux2", "Iop_HAdd16Sx2", "Iop_HSub16Ux2",
   "Iop_HSub16Sx2", "Iop_Add8x4", "Iop_Sub8x4", "Iop_QAdd8Sx4",
   "Iop_QAdd8Ux4", "Iop_QSub8Sx4", "Iop_QSub8Ux4", "Iop_HAdd8Ux4",
   "Iop_HAdd8Sx4", "Iop_HSub8Ux4", "Iop_HSub8Sx4", "Iop_Sad8Ux4",
   "Iop_CmpNEZ16x2", "Iop_CmpNEZ8x4", "Iop_I32UtoFx2", "Iop_I32StoFx2",
   "Iop_FtoI32Ux2_RZ", "Iop_FtoI32Sx2_RZ", "Iop_F32ToFixed32Ux2_RZ",
   "Iop_F32ToFixed32Sx2_RZ", "Iop_Fixed32UToF32x2_RN",
   "Iop_Fixed32SToF32x2_RN", "Iop_Max32Fx2", "Iop_Min32Fx2",
   "Iop_PwMax32Fx2", "Iop_PwMin32Fx2", "Iop_CmpEQ32Fx2", "Iop_CmpGT32Fx2",
   "Iop_CmpGE32Fx2", "Iop_Recip32Fx2", "Iop_Recps32Fx2", "Iop_Rsqrte32Fx2",
   "Iop_Rsqrts32Fx2", "Iop_Neg32Fx2", "Iop_Abs32Fx2", "Iop_CmpNEZ8x8",
   "Iop_CmpNEZ16x4", "Iop_CmpNEZ32x2", "Iop_Add8x8", "Iop_Add16x4",
   "Iop_Add32x2", "Iop_QAdd8Ux8", "Iop_QAdd16Ux4", "Iop_QAdd32Ux2",
   "Iop_QAdd64Ux1", "Iop_QAdd8Sx8", "Iop_QAdd16Sx4", "Iop_QAdd32Sx2",
   "Iop_QAdd64Sx1", "Iop_PwAdd8x8", "Iop_PwAdd16x4", "Iop_PwAdd32x2",
   "Iop_PwMax8Sx8", "Iop_PwMax16Sx4", "Iop_PwMax32Sx2", "Iop_PwMax8Ux8",
   "Iop_PwMax16Ux4", "Iop_PwMax32Ux2", "Iop_PwMin8Sx8", "Iop_PwMin16Sx4",
   "Iop_PwMin32Sx2", "Iop_PwMin8Ux8", "Iop_PwMin16Ux4", "Iop_PwMin32Ux2",
   "Iop_PwAddL8Ux8", "Iop_PwAddL16Ux4", "Iop_PwAddL32Ux2", "Iop_PwAddL8Sx8",
   "Iop_PwAddL16Sx4", "Iop_PwAddL32Sx2", "Iop_Sub8x8", "Iop_Sub16x4",
   "Iop_Sub32x2", "Iop_QSub8Ux8", "Iop_QSub16Ux4", "Iop_QSub32Ux2",
   "Iop_QSub64Ux1", "Iop_QSub8Sx8", "Iop_QSub16Sx4", "Iop_QSub32Sx2",
   "Iop_QSub64Sx1", "Iop_Abs8x8", "Iop_Abs16x4", "Iop_Abs32x2",
   "Iop_Mul8x8", "Iop_Mul16x4", "Iop_Mul32x2", "Iop_Mul32Fx2",
   "Iop_MulHi16Ux4", "Iop_MulHi16Sx4", "Iop_PolynomialMul8x8",
   "Iop_QDMulHi16Sx4", "Iop_QDMulHi32Sx2", "Iop_QRDMulHi16Sx4",
   "Iop_QRDMulHi32Sx2", "Iop_Avg8Ux8", "Iop_Avg16Ux4", "Iop_Max8Sx8",
   "Iop_Max16Sx4", "Iop_Max32Sx2", "Iop_Max8Ux8", "Iop_Max16Ux4",
   "Iop_Max32Ux2", "Iop_Min8Sx8", "Iop_Min16Sx4", "Iop_Min32Sx2",
   "Iop_Min8Ux8", "Iop_Min16Ux4", "Iop_Min32Ux2", "Iop_CmpEQ8x8",
   "Iop_CmpEQ16x4", "Iop_CmpEQ32x2", "Iop_CmpGT8Ux8", "Iop_CmpGT16Ux4",
   "Iop_CmpGT32Ux2", "Iop_CmpGT8Sx8", "Iop_CmpGT16Sx4", "Iop_CmpGT32Sx2",
   "Iop_Cnt8x8", "Iop_Clz8Sx8", "Iop_Clz16Sx4", "Iop_Clz32Sx2",
   "Iop_Cls8Sx8", "Iop_Cls16Sx4", "Iop_Cls32Sx2", "Iop_Shl8x8",
   "Iop_Shl16x4", "Iop_Shl32x2", "Iop_Shr8x8", "Iop_Shr16x4", "Iop_Shr32x2",
   "Iop_Sar8x8", "Iop_Sar16x4", "Iop_Sar32x2", "Iop_Sal8x8", "Iop_Sal16x4",
   "Iop_Sal32x2", "Iop_Sal64x1", "Iop_ShlN8x8", "Iop_ShlN16x4",
   "Iop_ShlN32x2", "Iop_ShrN8x8", "Iop_ShrN16x4", "Iop_ShrN32x2",
   "Iop_SarN8x8", "Iop_SarN16x4", "Iop_SarN32x2", "Iop_QShl8x8",
   "Iop_QShl16x4", "Iop_QShl32x2", "Iop_QShl64x1", "Iop_QSal8x8",
   "Iop_QSal16x4", "Iop_QSal32x2", "Iop_QSal64x1", "Iop_QShlN8Sx8",
   "Iop_QShlN16Sx4", "Iop_QShlN32Sx2", "Iop_QShlN64Sx1", "Iop_QShlN8x8",
   "Iop_QShlN16x4", "Iop_QShlN32x2", "Iop_QShlN64x1", "Iop_QSalN8x8",
   "Iop_QSalN16x4", "Iop_QSalN32x2", "Iop_QSalN64x1",
   "Iop_QNarrowBin16Sto8Ux8", "Iop_QNarrowBin16Sto8Sx8",
   "Iop_QNarrowBin32Sto16Sx4", "Iop_NarrowBin16to8x8",
   "Iop_NarrowBin32to16x4", "Iop_InterleaveHI8x8", "Iop_InterleaveHI16x4",
   "Iop_InterleaveHI32x2", "Iop_InterleaveLO8x8", "Iop_InterleaveLO16x4",
   "Iop_InterleaveLO32x2", "Iop_InterleaveOddLanes8x8",
   "Iop_InterleaveEvenLanes8x8", "Iop_InterleaveOddLanes16x4",
   "Iop_InterleaveEvenLanes16x4", "Iop_CatOddLanes8x8",
   "Iop_CatOddLanes16x4", "Iop_CatEvenLanes8x8", "Iop_CatEvenLanes16x4",
   "Iop_GetElem8x8", "Iop_GetElem16x4", "Iop_GetElem32x2", "Iop_SetElem8x8",
   "Iop_SetElem16x4", "Iop_SetElem32x2", "Iop_Dup8x8", "Iop_Dup16x4",
   "Iop_Dup32x2", "Iop_Extract64", "Iop_Reverse16_8x8", "Iop_Reverse32_8x8",
   "Iop_Reverse32_16x4", "Iop_Reverse64_8x8", "Iop_Reverse64_16x4",
   "Iop_Reverse64_32x2", "Iop_Perm8x8", "Iop_Recip32x2", "Iop_Rsqrte32x2",
   "Iop_AddD64", "Iop_SubD64", "Iop_MulD64", "Iop_DivD64", "Iop_AddD128",
   "Iop_SubD128", "Iop_MulD128", "Iop_DivD128", "Iop_ShlD64", "Iop_ShrD64",
   "Iop_ShlD128", "Iop_ShrD128", "Iop_D32toD64", "Iop_D64toD128",
   "Iop_I64StoD128", "Iop_D64toD32", "Iop_D128toD64", "Iop_I64StoD64",
   "Iop_D64toI64S", "Iop_D128toI64S", "Iop_RoundD64toInt",
   "Iop_RoundD128toInt", "Iop_CmpD64", "Iop_CmpD128", "Iop_QuantizeD64",
   "Iop_QuantizeD128", "Iop_SignificanceRoundD64",
   "Iop_SignificanceRoundD128", "Iop_ExtractExpD64", "Iop_ExtractExpD128",
   "Iop_InsertExpD64", "Iop_InsertExpD128", "Iop_D64HLtoD128",
   "Iop_D128HItoD64", "Iop_D128LOtoD64", "Iop_DPBtoBCD", "Iop_BCDtoDPB",
   "Iop_ReinterpI64asD64", "Iop_ReinterpD64asI64", "Iop_Add32Fx4",
   "Iop_Sub32Fx4", "Iop_Mul32Fx4", "Iop_Div32Fx4", "Iop_Max32Fx4",
   "Iop_Min32Fx4", "Iop_Add32Fx2", "Iop_Sub32Fx2", "Iop_CmpEQ32Fx4",
   "Iop_CmpLT32Fx4", "Iop_CmpLE32Fx4", "Iop_CmpUN32Fx4", "Iop_CmpGT32Fx4",
   "Iop_CmpGE32Fx4", "Iop_Abs32Fx4", "Iop_PwMax32Fx4", "Iop_PwMin32Fx4",
   "Iop_Sqrt32Fx4", "Iop_RSqrt32Fx4", "Iop_Neg32Fx4", "Iop_Recip32Fx4",
   "Iop_Recps32Fx4", "Iop_Rsqrte32Fx4", "Iop_Rsqrts32Fx4", "Iop_I32UtoFx4",
   "Iop_I32StoFx4", "Iop_FtoI32Ux4_RZ", "Iop_FtoI32Sx4_RZ",
   "Iop_QFtoI32Ux4_RZ", "Iop_QFtoI32Sx4_RZ", "Iop_RoundF32x4_RM",
   "Iop_RoundF32x4_RP", "Iop_RoundF32x4_RN", "Iop_RoundF32x4_RZ",
   "Iop_F32ToFixed32Ux4_RZ", "Iop_F32ToFixed32Sx4_RZ",
   "Iop_Fixed32UToF32x4_RN", "Iop_Fixed32SToF32x4_RN", "Iop_F32toF16x4",
   "Iop_F16toF32x4", "Iop_Add32F0x4", "Iop_Sub32F0x4", "Iop_Mul32F0x4",
   "Iop_Div32F0x4", "Iop_Max32F0x4", "Iop_Min32F0x4", "Iop_CmpEQ32F0x4",
   "Iop_CmpLT32F0x4", "Iop_CmpLE32F0x4", "Iop_CmpUN32F0x4",
   "Iop_Recip32F0x4", "Iop_Sqrt32F0x4", "Iop_RSqrt32F0x4", "Iop_Add64Fx2",
   "Iop_Sub64Fx2", "Iop_Mul64Fx2", "Iop_Div64Fx2", "Iop_Max64Fx2",
   "Iop_Min64Fx2", "Iop_CmpEQ64Fx2", "Iop_CmpLT64Fx2", "Iop_CmpLE64Fx2",
   "Iop_CmpUN64Fx2", "Iop_Recip64Fx2", "Iop_Sqrt64Fx2", "Iop_RSqrt64Fx2",
   "Iop_Add64F0x2", "Iop_Sub64F0x2", "Iop_Mul64F0x2", "Iop_Div64F0x2",
   "Iop_Max64F0x2", "Iop_Min64F0x2", "Iop_CmpEQ64F0x2", "Iop_CmpLT64F0x2",
   "Iop_CmpLE64F0x2", "Iop_CmpUN64F0x2", "Iop_Recip64F0x2",
   "Iop_Sqrt64F0x2", "Iop_RSqrt64F0x2", "Iop_V128to64", "Iop_V128HIto64",
   "Iop_64HLtoV128", "Iop_64UtoV128", "Iop_SetV128lo64", "Iop_32UtoV128",
   "Iop_V128to32", "Iop_SetV128lo32", "Iop_NotV128", "Iop_AndV128",
   "Iop_OrV128", "Iop_XorV128", "Iop_ShlV128", "Iop_ShrV128",
   "Iop_CmpNEZ8x16", "Iop_CmpNEZ16x8", "Iop_CmpNEZ32x4", "Iop_CmpNEZ64x2",
   "Iop_Add8x16", "Iop_Add16x8", "Iop_Add32x4", "Iop_Add64x2",
   "Iop_QAdd8Ux16", "Iop_QAdd16Ux8", "Iop_QAdd32Ux4", "Iop_QAdd64Ux2",
   "Iop_QAdd8Sx16", "Iop_QAdd16Sx8", "Iop_QAdd32Sx4", "Iop_QAdd64Sx2",
   "Iop_Sub8x16", "Iop_Sub16x8", "Iop_Sub32x4", "Iop_Sub64x2",
   "Iop_QSub8Ux16", "Iop_QSub16Ux8", "Iop_QSub32Ux4", "Iop_QSub64Ux2",
   "Iop_QSub8Sx16", "Iop_QSub16Sx8", "Iop_QSub32Sx4", "Iop_QSub64Sx2",
   "Iop_Mul8x16", "Iop_Mul16x8", "Iop_Mul32x4", "Iop_MulHi16Ux8",
   "Iop_MulHi32Ux4", "Iop_MulHi16Sx8", "Iop_MulHi32Sx4",
   "Iop_MullEven8Ux16", "Iop_MullEven16Ux8", "Iop_MullEven8Sx16",
   "Iop_MullEven16Sx8", "Iop_Mull8Ux8", "Iop_Mull8Sx8", "Iop_Mull16Ux4",
   "Iop_Mull16Sx4", "Iop_Mull32Ux2", "Iop_Mull32Sx2", "Iop_QDMulHi16Sx8",
   "Iop_QDMulHi32Sx4", "Iop_QRDMulHi16Sx8", "Iop_QRDMulHi32Sx4",
   "Iop_QDMulLong16Sx4", "Iop_QDMulLong32Sx2", "Iop_PolynomialMul8x16",
   "Iop_PolynomialMull8x8", "Iop_PwAdd8x16", "Iop_PwAdd16x8",
   "Iop_PwAdd32x4", "Iop_PwAdd32Fx2", "Iop_PwAddL8Ux16", "Iop_PwAddL16Ux8",
   "Iop_PwAddL32Ux4", "Iop_PwAddL8Sx16", "Iop_PwAddL16Sx8",
   "Iop_PwAddL32Sx4", "Iop_Abs8x16", "Iop_Abs16x8", "Iop_Abs32x4",
   "Iop_Avg8Ux16", "Iop_Avg16Ux8", "Iop_Avg32Ux4", "Iop_Avg8Sx16",
   "Iop_Avg16Sx8", "Iop_Avg32Sx4", "Iop_Max8Sx16", "Iop_Max16Sx8",
   "Iop_Max32Sx4", "Iop_Max8Ux16", "Iop_Max16Ux8", "Iop_Max32Ux4",
   "Iop_Min8Sx16", "Iop_Min16Sx8", "Iop_Min32Sx4", "Iop_Min8Ux16",
   "Iop_Min16Ux8", "Iop_Min32Ux4", "Iop_CmpEQ8x16", "Iop_CmpEQ16x8",
   "Iop_CmpEQ32x4", "Iop_CmpEQ64x2", "Iop_CmpGT8Sx16", "Iop_CmpGT16Sx8",
   "Iop_CmpGT32Sx4", "Iop_CmpGT64Sx2", "Iop_CmpGT8Ux16", "Iop_CmpGT16Ux8",
   "Iop_CmpGT32Ux4", "Iop_Cnt8x16", "Iop_Clz8Sx16", "Iop_Clz16Sx8",
   "Iop_Clz32Sx4", "Iop_Cls8Sx16", "Iop_Cls16Sx8", "Iop_Cls32Sx4",
   "Iop_ShlN8x16", "Iop_ShlN16x8", "Iop_ShlN32x4", "Iop_ShlN64x2",
   "Iop_ShrN8x16", "Iop_ShrN16x8", "Iop_ShrN32x4", "Iop_ShrN64x2",
   "Iop_SarN8x16", "Iop_SarN16x8", "Iop_SarN32x4", "Iop_SarN64x2",
   "Iop_Shl8x16", "Iop_Shl16x8", "Iop_Shl32x4", "Iop_Shl64x2",
   "Iop_Shr8x16", "Iop_Shr16x8", "Iop_Shr32x4", "Iop_Shr64x2",
   "Iop_Sar8x16", "Iop_Sar16x8", "Iop_Sar32x4", "Iop_Sar64x2",
   "Iop_Sal8x16", "Iop_Sal16x8", "Iop_Sal32x4", "Iop_Sal64x2",
   "Iop_Rol8x16", "Iop_Rol16x8", "Iop_Rol32x4", "Iop_QShl8x16",
   "Iop_QShl16x8", "Iop_QShl32x4", "Iop_QShl64x2", "Iop_QSal8x16",
   "Iop_QSal16x8", "Iop_QSal32x4", "Iop_QSal64x2", "Iop_QShlN8Sx16",
   "Iop_QShlN16Sx8", "Iop_QShlN32Sx4", "Iop_QShlN64Sx2", "Iop_QShlN8x16",
   "Iop_QShlN16x8", "Iop_QShlN32x4", "Iop_QShlN64x2", "Iop_QSalN8x16",
   "Iop_QSalN16x8", "Iop_QSalN32x4", "Iop_QSalN64x2",
   "Iop_QNarrowBin16Sto8Ux16", "Iop_QNarrowBin32Sto16Ux8",
   "Iop_QNarrowBin16Sto8Sx16", "Iop_QNarrowBin32Sto16Sx8",
   "Iop_QNarrowBin16Uto8Ux16", "Iop_QNarrowBin32Uto16Ux8",
   "Iop_NarrowBin16to8x16", "Iop_NarrowBin32to16x8", "Iop_NarrowUn16to8x8",
   "Iop_NarrowUn32to16x4", "Iop_NarrowUn64to32x2", "Iop_QNarrowUn16Sto8Sx8",
   "Iop_QNarrowUn32Sto16Sx4", "Iop_QNarrowUn64Sto32Sx2",
   "Iop_QNarrowUn16Sto8Ux8", "Iop_QNarrowUn32Sto16Ux4",
   "Iop_QNarrowUn64Sto32Ux2", "Iop_QNarrowUn16Uto8Ux8",
   "Iop_QNarrowUn32Uto16Ux4", "Iop_QNarrowUn64Uto32Ux2",
   "Iop_Widen8Uto16x8", "Iop_Widen16Uto32x4", "Iop_Widen32Uto64x2",
   "Iop_Widen8Sto16x8", "Iop_Widen16Sto32x4", "Iop_Widen32Sto64x2",
   "Iop_InterleaveHI8x16", "Iop_InterleaveHI16x8", "Iop_InterleaveHI32x4",
   "Iop_InterleaveHI64x2", "Iop_InterleaveLO8x16", "Iop_InterleaveLO16x8",
   "Iop_InterleaveLO32x4", "Iop_InterleaveLO64x2",
   "Iop_InterleaveOddLanes8x16", "Iop_InterleaveEvenLanes8x16",
   "Iop_InterleaveOddLanes16x8", "Iop_InterleaveEvenLanes16x8",
   "Iop_InterleaveOddLanes32x4", "Iop_InterleaveEvenLanes32x4",
   "Iop_CatOddLanes8x16", "Iop_CatOddLanes16x8", "Iop_CatOddLanes32x4",
   "Iop_CatEvenLanes8x16", "Iop_CatEvenLanes16x8", "Iop_CatEvenLanes32x4",
   "Iop_GetElem8x16", "Iop_GetElem16x8", "Iop_GetElem32x4",
   "Iop_GetElem64x2", "Iop_Dup8x16", "Iop_Dup16x8", "Iop_Dup32x4",
   "Iop_ExtractV128", "Iop_Reverse16_8x16", "Iop_Reverse32_8x16",
   "Iop_Reverse32_16x8", "Iop_Reverse64_8x16", "Iop_Reverse64_16x8",
   "Iop_Reverse64_32x4", "Iop_Perm8x16", "Iop_Perm32x4", "Iop_Recip32x4",
   "Iop_Rsqrte32x4", "Iop_V256to64_0", "Iop_V256to64_1", "Iop_V256to64_2",
   "Iop_V256to64_3", "Iop_64x4toV256", "Iop_V256toV128_0",
   "Iop_V256toV128_1", "Iop_V128HLtoV256", "Iop_AndV256", "Iop_OrV256",
   "Iop_XorV256", "Iop_NotV256", "Iop_CmpNEZ32x8", "Iop_CmpNEZ64x4",
   "Iop_Add64Fx4", "Iop_Sub64Fx4", "Iop_Mul64Fx4", "Iop_Div64Fx4",
   "Iop_Add32Fx8", "Iop_Sub32Fx8", "Iop_Mul32Fx8", "Iop_Div32Fx8",
   "Iop_Sqrt32Fx8", "Iop_Sqrt64Fx4", "Iop_RSqrt32Fx8", "Iop_Recip32Fx8",
   "Iop_Max32Fx8", "Iop_Min32Fx8", "Iop_Max64Fx4", "Iop_Min64Fx4",
};

static UShort cu_op_counts[] = {
   0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 2, 3, 3, 4, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 19, 19, 19,
   19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 2, 2, 2, 31, 1, 1, 1, 17, 2, 2, 3, 31, 1, 1, 1, 1, 3, 3, 3, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 4, 4, 8, 64, 4, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
   2, 31, 2, 2, 2, 31, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 17, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 31, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 31, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 2, 31, 1, 1, 1, 17, 4, 8, 1, 1, 1, 1, 1, 1,
};

#define NUM_LD 15
static const HChar *cu_ld_names[] = {
   "Ity_INVALID", "Ity_I1", "Ity_I8", "Ity_I16", "Ity_I32", "Ity_I64",
   "Ity_I128", "Ity_F32", "Ity_F64", "Ity_D32", "Ity_D64", "Ity_D128",
   "Ity_F128", "Ity_V128", "Ity_V256",
};

static UShort cu_ld_counts[] = {
   0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};
/* count_tables: end */


/* This routine is called during instrumentation to count up clock cycles
 * for a particular instruction within a superblock.
 */
static Int cu_expr_cnt(IRExpr *expr)
{
   IROp op;
   IRType ty;

   switch (expr->tag) {
      case Iex_Binder: return 0;
      case Iex_Get: return 0;
      case Iex_GetI: return 0;
      case Iex_RdTmp: return 0;
      case Iex_Qop: op = expr->Iex.Qop.details->op; break;
      case Iex_Triop: op = expr->Iex.Triop.details->op; break;
      case Iex_Binop: op = expr->Iex.Binop.op; break;
      case Iex_Unop: op = expr->Iex.Unop.op; break;
      case Iex_Load:
         ty = expr->Iex.Load.ty;
         if (ty - Ity_INVALID < NUM_LD) {
            return cu_ld_counts[ty - Ity_INVALID];
         } else {
            return 0;
         }
      case Iex_Const: return 0;
      case Iex_ITE: return 1;
      case Iex_CCall: return 0;
      case Iex_VECRET: return 0;
      case Iex_BBPTR: return 0;
   }
   if (op - Iop_INVALID < NUM_OP) {
      return cu_op_counts[op - Iop_INVALID];
   } else {
      return 0;
   }
}

/* There was a concern on the valgrind-develop list (later rescinded) that
 * using method 1 (see updating of m1_count) below is broken.  The argument
 * was that the client program may require two instructions to read the
 * counter.  After the first instruction, the VEX may increment the counter
 * variable, resulting in a bogus value read by the client's second
 * instruction.  The claim was rescinded once I explained that the updates
 * were only happening at the exit of the superblock.  (I now also update
 * just before a dirty call.)
 * Note: I don't think volatile is needed for m2_clkcntptr, because the 
 * scope of the call will not allow changes.
 */
static Bool m1_used = False;
static unsigned long m1_clkcnt = 0;
static Bool m2_used = False;
static /*volatile*/ unsigned long * /*volatile*/ m2_clkcntptr = 0;
static Bool counter_exhausted = False;

static void update_clkcnt(UInt lclkcnt)
{
   if (m1_clkcnt > LONG_MAX/2) {
      if (m1_used) counter_exhausted = True;
   } else {
      m1_clkcnt += lclkcnt;
   }
   if (m2_clkcntptr == 0) return;	/* no counter specified */
   if (*m2_clkcntptr > LONG_MAX/2) {
      if (m2_used) counter_exhausted = True;
   } else {
      *m2_clkcntptr += lclkcnt;
   }
}

#if 0
/* for development, so we can see counts via --trace-flags */
static void mark_count(UInt clkcntinc) { }
#define MARK_COUNT(C)			do {	 \
	    expr = IRExpr_Const(IRConst_U64(C)); \
	    di = unsafeIRDirty_0_N( 0, "mark_count", \
				    VG_(fnptr_to_fnentry)( &mark_count ), \
				    mkIRExprVec_1(expr)); \
	    addStmtToIRSB( sbOut, IRStmt_Dirty(di) ); } while (0)
#else
#define MARK_COUNT(C) /* */
#endif

/* We will count expressions and statements.  We assume that, since the code
 * is flattened, no statement will have more than one expression.
 */
static
IRSB* cu_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn, 
                      const VexGuestLayout* layout, 
                      const VexGuestExtents* vge,
                      const VexArchInfo* vai,
                      IRType gWordTy,
                      IRType hWordTy )
{
   IRDirty*   di;
   Int        i;
   IRSB*      sbOut;
   Int        lastst;			/* last statement instrumented */
   Int        lclkcnt;			/* localized clock count */
   IRExpr*    expr;

   if (gWordTy != hWordTy) {
      /* We don't currently support this case. ??? */
      VG_(tool_panic)("host/guest word size mismatch");
   }

   /* Set up SB. */
   sbOut = deepCopyIRSBExceptStmts(sbIn);

   /* Copy verbatim any IR preamble preceding the first IMark. */
   i = 0;
   while (i < sbIn->stmts_used && sbIn->stmts[i]->tag != Ist_IMark) {
      addStmtToIRSB( sbOut, sbIn->stmts[i] );
      i++;
   }

   /* Go through the statements adding code count for statements and their
    * member expressions.  Update user variables at exits.  This does not
    * remove the small number of clock cycles to implement clientrequest.
    */
   lclkcnt = 1;				/* Count the clocks to branch here. */
   for (/*use current i*/; i < sbIn->stmts_used; i++) {
      IRStmt* st = sbIn->stmts[i];
      if (!st || st->tag == Ist_NoOp) continue;

      switch (st->tag) {
         case Ist_NoOp:			/* no op */
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_IMark:		/* guest instruction marker */
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_AbiHint:		/* mark addr space undefined  */
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_Put:			/* put to register */
	    expr = st->Ist.Put.data;
	    lclkcnt += cu_expr_cnt(expr);
            MARK_COUNT(cu_expr_cnt(expr));
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_PutI:			/* put to register, indirect */
	    expr = st->Ist.PutI.details->data;
	    lclkcnt += cu_expr_cnt(expr);
            MARK_COUNT(cu_expr_cnt(expr));
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_WrTmp:		/* write to temp */
	    expr = st->Ist.WrTmp.data;
	    lclkcnt += cu_expr_cnt(expr);
	    MARK_COUNT(cu_expr_cnt(expr));
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_Store:		/* store to memory */
	    expr  = st->Ist.Store.data;
	    lclkcnt += cu_expr_cnt(expr);
	    lclkcnt += 1; 		/* Add 1 clock for the store. */
	    MARK_COUNT(cu_expr_cnt(expr)+1);
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_CAS:			/* compare and swap */
	    lclkcnt += 1;		
	    MARK_COUNT(1);
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_LLSC:			/* load from memory */
	    expr = st->Ist.LLSC.storedata;
	    lclkcnt += cu_expr_cnt(expr);
	    MARK_COUNT(cu_expr_cnt(expr));
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_Dirty:		/* instrumentation call  */
	    /* This dirty call could be our call to get the count so make 
	     * sure we are covering the current block.
	     */
	    expr = IRExpr_Const(IRConst_U64(lclkcnt));
	    di = unsafeIRDirty_0_N( 0, "update_clkcnt", 
				    VG_(fnptr_to_fnentry)( &update_clkcnt ),
				    mkIRExprVec_1(expr)); 
	    addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
	    lclkcnt = 0;	      /* Reset local count to zero. */
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_MBE:			/* memory bus event */
	    lclkcnt += 1;
	    MARK_COUNT(1);
            addStmtToIRSB( sbOut, st );
            break;
         case Ist_Exit:			/* exit superblock, add clocks? */
	    /* This is the hook to add counts to user's counter. */
            /* \todo: Add counts for branching. ??? */
	    expr = IRExpr_Const(IRConst_U64(lclkcnt));
	    di = unsafeIRDirty_0_N( 0, "update_clkcnt", 
				    VG_(fnptr_to_fnentry)( &update_clkcnt ),
				    mkIRExprVec_1(expr)); 
	    addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
	    lclkcnt = 0;	      /* Reset local count to zero. */
            addStmtToIRSB( sbOut, st );
            break;
         default:
            tl_assert(0);
      }
      lastst = st->tag;
   }

   if (lastst != Ist_Exit) {
      expr = IRExpr_Const(IRConst_U64(lclkcnt)); // IRConst_U64 == ULong
      di = unsafeIRDirty_0_N( 0, "update_clkcnt", 
			      VG_(fnptr_to_fnentry)( &update_clkcnt ),
			      mkIRExprVec_1(expr) );
      addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
   }

   return sbOut;
}


/*------------------------------------------------------------*/
/*--- Client Requests                                      ---*/
/*------------------------------------------------------------*/

static Bool cu_handle_client_request ( ThreadId tid, UWord* arg, UWord* ret )
{
   //VG_(printf)("cu_handle_client_request called\n");
   if (!VG_IS_TOOL_USERREQ('X','P',arg[0])
       && VG_USERREQ__CU_CLRCTR1 != arg[0]
       && VG_USERREQ__CU_GETCTR1 != arg[0]
       && VG_USERREQ__CU_SETCTR2PTR != arg[0])
      return False;

   switch (arg[0]) {
      case VG_USERREQ__CU_CLRCTR1:
         //VG_(printf)("  CU_CLRCTR1()\n");
         m1_clkcnt = 0;
         m1_used = True;
         break;

      case VG_USERREQ__CU_GETCTR1:
         //VG_(printf)("  CU_GETCTR1(): %lu\n", m1_clkctr);
         *ret = m1_clkcnt;
         m1_used = True;
         break;

      case VG_USERREQ__CU_SETCTR2PTR:
         //VG_(printf)("  CU_SETCTR2PTR(%p)\n", m2_clkcntptr);
	 m2_clkcntptr = (unsigned long*) arg[1];
         m2_used = True;
         break;

      default:
         VG_(message)(
            Vg_UserMsg, 
            "Warning: unknown cputil client request code %llx\n",
            (ULong)arg[0]
         );
         return False;
   }

   return True;
}


/*------------------------------------------------------------*/
/*--- Command line options                                 ---*/
/*------------------------------------------------------------*/

/* Read one line.  Return len, or -1 if problem. */
static Int cu_readline(Int fd, HChar *buf, Int bufsiz)
{
   Int r, n;
   HChar c;
   
   n = 0;
   do {
      r = VG_(read)(fd, &c, 1);
      if (r == -1) {
	 return -1;
      }
      if (r == 0 || c == '\n') {
	 buf[n] = 0;
	 return n;
      }
      buf[n++] = c;
      if (n >= bufsiz -1) {
	 buf[n] = 0;
	 return -1;
      }
   } while (1);
   return 0;
}

/* Parse line of "<unsigned int><ws><string>".  Guessing scanf is taboo. */
static Int cu_parse_line(HChar *buf, Int *ival, HChar *sval, Int slen)
{
   Int ix, sx, iv;

   ix = 0;
   /* Parse unsigned integer. */
   iv = 0;
   while ('0' <= buf[ix] && buf[ix] <= '9') {
      iv = (10*iv) + (buf[ix] - '0');
      ix = ix + 1;
      if (buf[ix] == 0) {
	 /* end of buffer */
	 return -1;
      }
   }
   if (ix == 0 ) {
      /* expecting unsigned integer */
      return -1;
   }
   *ival = iv;
   /* Skip whitespace. */
   if (buf[ix] != ' ' && buf[ix] != '\t') {
      /* expecting space */
      return -1;
   }
   while (buf[ix] == ' ' || buf[ix] == '\t') {
      ix++;
      if (buf[ix] == 0) {
	 /* end of buffer */
	 return -1;
      }
   }
   /* Parse string. */
   sx = 0;
   while (buf[ix] != ' ' && buf[ix] != '\t' && buf[ix] != 0) {
      if (sx >= slen - 1) {
	 /* String buffer not long enough. */
	 return -1;
      }
      sval[sx++] = buf[ix++];
   }
   sval[sx] = '\0';
   return 0;
}

static Bool cu_dump_op_table(const HChar *filename)
{
   Int fd, i;
   HChar buf[80];
   SysRes sres;

   sres = VG_(open)(filename,
		    VKI_O_CREAT|VKI_O_TRUNC|VKI_O_WRONLY,
		    VKI_S_IRUSR|VKI_S_IWUSR);
   if (sr_isError(sres)) {
      VG_(umsg)("error: can't open output file '%s'\n", filename);
      return False;
   } else {
      fd = sr_Res(sres);
   }
   VG_(sprintf)(buf, "# valgrind/cputil count table\n#\n");
   VG_(write)(fd, buf, VG_(strlen)(buf));
   VG_(sprintf)(buf, "# load types\n");
   VG_(write)(fd, buf, VG_(strlen)(buf));
   for (i = 1; i < NUM_LD; i++) {
      VG_(sprintf)(buf, "%d\t%s\n", cu_ld_counts[i], cu_ld_names[i]);
      VG_(write)(fd, buf, VG_(strlen)(buf));
   }
   VG_(sprintf)(buf, "# other ops\n");
   VG_(write)(fd, buf, VG_(strlen)(buf));
   for (i = 1; i < NUM_OP; i++) {
      VG_(sprintf)(buf, "%d\t%s\n", cu_op_counts[i], cu_op_names[i]);
      VG_(write)(fd, buf, VG_(strlen)(buf));
   }
   VG_(close)(fd);
   return True;
}

static Bool cu_load_op_table(const HChar *filename)
{
   Int fd, ln, ix, cval, res;
   HChar buf[80], sval[40];
   SysRes sres;

   sres = VG_(open)(filename, VKI_O_RDONLY, VKI_S_IRUSR);
   if (sr_isError(sres)) {
      VG_(umsg)("error: can't open output file '%s'\n", filename);
      return False;
   } else {
      fd = sr_Res(sres);
   }
   ln = 0;
   ix = 1;
   while (ix < NUM_LD) {
      res = cu_readline(fd, buf, 80); ln++;
      if (res < 0) {
	 /* error */
	 return False;
      }
      if (buf[0] == '#') continue;	/* Allow #-comments. */
      sval[39] = 0;
      res = cu_parse_line(buf, &cval, sval, 40);
      if (res < 0) {
	 VG_(umsg)("parse error: line %d\n", ln);
	 return False;
      }
      if (VG_(strcmp)(cu_ld_names[ix], sval) != 0) {
	 VG_(umsg)("bad opname: line %d\n", ln);
	 return False;
      }
      if (0 <= cval && cval <= 1999) {	/* arbitrary max clock count */
         cu_ld_counts[ix] = cval;
      } else {
	 VG_(umsg)("invalid clock count: %d, line %d\n", cval, ln);
	 VG_(umsg)("          expect in: [0,1999]\n");
	 return False;
      }
      ix++;
   }
   ix = 1;
   while (ix < NUM_OP) {
      res = cu_readline(fd, buf, 80); ln++;
      if (res < 0) {
	 /* error */
	 return False;
      }
      if (buf[0] == '#') continue;	/* Allow #-comments. */
      sval[39] = 0;
      res = cu_parse_line(buf, &cval, sval, 40);
      if (res < 0) {
	 VG_(umsg)("parse error: line %d\n", ln);
	 return False;
      }
      if (VG_(strcmp)(cu_op_names[ix], sval) != 0) {
	 VG_(umsg)("bad opname: line %d\n", ln);
	 return False;
      }
      if (0 <= cval && cval <= 1999) {	/* arbitrary max clock count */
         cu_op_counts[ix] = cval;
      } else {
	 VG_(umsg)("invalid clock count: %d, line %d\n", cval, ln);
	 VG_(umsg)("          expect in: [0,1999]\n");
	 return False;
      }
      ix++;
   }
   VG_(close)(fd);
   return True;
}

static void cu_print_usage(void)
{  
   VG_(printf)(
      "    --help                    help me\n"
      "    --dump-op-table=file      print op-count table to <file>\n"
      "    --load-op-table=file      load op-count table from <file>\n"
   );
}

static void cu_print_debug_usage(void)
{  
   VG_(printf)(
      "    (none)\n"
   );
}

static Bool cu_process_cmd_line_option(const HChar* arg)
{
   const HChar *xarg;

   if VG_STR_CLO(arg, "--dump-op-table", xarg) {
	 return cu_dump_op_table(xarg);
   }
   else if VG_STR_CLO(arg, "--load-op-table", xarg) {
         return cu_load_op_table(xarg);
   }
   else
      return False;
   return True;
}


/*------------------------------------------------------------*/
/*--- Other hooks                                          ---*/
/*------------------------------------------------------------*/

static void cu_fini(Int exitcode)
{
   if (counter_exhausted) {
      VG_(message)(Vg_UserMsg, "cputil: counter exhausted\n");
   }
}

static void cu_post_clo_init(void)
{
}

static void cu_pre_clo_init(void)
{
   VG_(details_name)            ("cputil");
   VG_(details_version)         (cu_version);
   VG_(details_description)     ("an CPU utilization profiler");
   VG_(details_copyright_author)(
      "Copyright (C) 2013-2013, and GNU GPL'd, by Matt Wette.");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);
   //VG_(details_avg_translation_sizeB) ( 200 );

   VG_(basic_tool_funcs)        (cu_post_clo_init,
                                 cu_instrument,
                                 cu_fini);
   VG_(needs_command_line_options)(cu_process_cmd_line_option,
                                   cu_print_usage,
                                   cu_print_debug_usage);
   VG_(needs_client_requests)   (cu_handle_client_request);

   m1_clkcnt = 0;
   m1_used = False;
   m2_clkcntptr = 0;
   m2_used = False;
   counter_exhausted = False;
}

VG_DETERMINE_INTERFACE_VERSION(cu_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                cu_main.c ---*/
/*--------------------------------------------------------------------*/
/* Local Variables: */
/* c-basic-offset: 3 */
/* eval: (c-set-offset 'case-label '+) */
/* End: */