#ifndef COVERFLOAT_H_INCLUDED
#define COVERFLOAT_H_INCLUDED

#include <stdint.h>
#include "../../riscv-isa-sim/softfloat/softfloat.h" // TODO: yuck fix paths

#ifdef __cplusplus
extern "C" {
#endif


// arbitary encoding of IBM paper operations
// numbering scheme: bits 31:4 are major operation (pulled directly form aharoni paper)
//                   bits 3:0 are variant operations (taylored towards riscv instrs)
//                   coverpoint are written such that any variant covers the superset
#define OP_ADD    0x10
#define OP_SUB    0x20
#define OP_MUL    0x30
#define OP_DIV    0x40
#define OP_FMA    0x50
#define OP_FMADD  0x51 /* mult-add            */
#define OP_FMSUB  0x52 /* mult-subtract       */
#define OP_FNMADD 0x53 /* negated (mult-add)  */
#define OP_FNMSUB 0x54 /* negated (mult-sub)  */
#define OP_SQRT   0x60
#define OP_REM    0x70
#define OP_CFI    0x80
#define OP_FCVTW  0x81 /* fp to int           */
#define OP_FCVTWU 0x82 /* fp to uint          */
#define OP_FCVTL  0x83 /* fp to long          */
#define OP_FCVTLU 0x84 /* fp to ulong         */
#define OP_CFF    0x90
#define OP_CIF    0xA0
#define OP_QC     0xB0
#define OP_FEQ    0xB1 /* quiet equal         */
#define OP_SC     0xC0
#define OP_FLT    0xC1 /* signaling less than */
#define OP_FLE    0xC2 /* signaling LT or eq  */
#define OP_CLASS  0xD0
#define OP_MIN    0xE0
#define OP_MAX    0xF0
#define OP_CSN    0x100 /* copy sign / negate */
#define OP_FSGNJ  0x101
#define OP_FSGNJN 0x102
#define OP_FSGNJX 0x103

// format encodings
//  {(int = 1, float = 0), (unsigned int), others => format encoding}
#define FMT_INVAL  0x11111111 /* source unused / invalid */
#define FMT_HALF   0x00000000
#define FMT_SINGLE 0x00000001
#define FMT_DOUBLE 0x00000010
#define FMT_QUAD   0x00000011
#define FMT_BF16   0x00000100
#define FMT_INT    0x10000001
#define FMT_UINT   0x11000001
#define FMT_LONG   0x10000010
#define FMT_ULONG  0x11000010

void softFloat_clearFlags( uint_fast8_t );

uint_fast8_t softFloat_getFlags ();

void softFloat_setRoundingMode ( uint_fast8_t );

void softfloat_getIntermResults ( intermResult_t * );


#ifdef __cplusplus
}
#endif

#endif