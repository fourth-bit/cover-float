#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coverfloat.h"

void softFloat_clearFlags( uint_fast8_t clearMask) {
    softfloat_exceptionFlags &= ~clearMask;
}

uint_fast8_t softFloat_getFlags () {
    return softfloat_exceptionFlags;
}

void softFloat_setRoundingMode (uint_fast8_t rm) {
    softfloat_roundingMode = rm;
}

void softfloat_getIntermResults (intermResult_t * result) {

    result->sign     = softfloat_intermediateResult.sign;
    result->exp      = softfloat_intermediateResult.exp;
    result->sig64    = softfloat_intermediateResult.sig64;
    result->sig0     = softfloat_intermediateResult.sig0;
    result->sigExtra = softfloat_intermediateResult.sigExtra;

}

void softfloat_clearIntermResults () {

    softfloat_intermediateResult.sign     = 0;
    softfloat_intermediateResult.exp      = 0;
    softfloat_intermediateResult.sig64    = 0;
    softfloat_intermediateResult.sig0     = 0;
    softfloat_intermediateResult.sigExtra = 0;

}

/*
AI CODE NEEDS MODIFICATION
*/

/*
uint128_t parse_hex_128(const char *hex) {
    uint128_t value = {0, 0};
    
    while (*hex) {
        char c = *hex++;
        uint8_t digit;
        
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F') digit = 10 + (c - 'A');
        else continue; // skip non-hex chars
        
        // Shift value left by 4 bits (multiply by 16)
        uint64_t new_upper = (value.upper << 4) | (value.lower >> 60);
        uint64_t new_lower = (value.lower << 4) | digit;
        
        value.upper = new_upper;
        value.lower = new_lower;
    }
    
    return value;
}
*/

uint128_t parse_hex_128(const char *hex)
{
    uint128_t value = {(uint64_t)0ULL, (uint64_t)0ULL};
    int count = 0;

    while (*hex && count < 32) {
        char c = *hex++;
        uint8_t digit;

        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F') digit = 10 + (c - 'A');
        else continue;

        uint64_t upper = (uint64_t)value.upper;
        uint64_t lower = (uint64_t)value.lower;

        value.upper = (upper << 4) | (lower >> 60);
        value.lower = (lower << 4) | (uint64_t)digit;

        // printf("VALUE AT STEP %d: %016x%016x\n", count, value.upper, value.lower);

        count++;
    }

    return value;
}



void reference_model( const uint32_t       * op,
                      const uint8_t        * rm,
                      const uint128_t      * a, 
                      const uint128_t      * b, 
                      const uint128_t      * c, 
                      const uint8_t        * operandFmt, 
                      const uint8_t        * resultFmt,

                      uint128_t            * result,
                      uint8_t              * flags ,
                      intermResult_t       * intermResult ) {
    
    
    // clear flags so we get only triggered flags
    softFloat_clearFlags(0xFF);

    // clear intermediate result to avoid reporting intermediate results for results that were not rounded
    softfloat_clearIntermResults(result);

    // set rounding mode
    softFloat_setRoundingMode(*rm);

    // nested switch statements to call softfloat functions

    switch (*op) {
        case OP_ADD: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_add(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_add(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_add(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_add(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_add(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }
            }
            break;
        }

        case OP_SUB: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_sub(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_sub(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_sub(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_sub(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_sub(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_MUL: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_mul(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);

                    // printf("performing single precision mul!!\n");
                    // printf("int operands are: %x and %x\n", *a, *b);
                    // printf("float operands are: %x and %x\n", af.v, bf.v);
                    // printf("float result is %x\n", resultf.v);
                    // printf("int result is %032x%032x\n", result->upper, result->lower);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_mul(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_mul(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_mul(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_mul(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_DIV: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_div(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_div(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_div(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_div(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_div(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_REM: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_rem(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_rem(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_rem(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_rem(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                // TODO: not currently implemented as a function through softfloat
                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    // resultf = bf16_rem(af, bf);
                    float32_t f32A = { (uint_fast32_t)af.v << 16 };
                    float32_t f32B = { (uint_fast32_t)bf.v << 16 };
                    resultf = f32_to_bf16 ( f32_div ( f32A, f32B ) );

                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_FEQ: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf.v = f32_eq(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf.v = f64_eq(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf.v[0] = 0;
                    resultf.v[1] = f128_eq(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = f16_eq(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = bf16_eq(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_FLT: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf.v = f32_lt(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf.v = f64_lt(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf.v[0] = 0;
                    resultf.v[1] = f128_lt(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = f16_lt(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = bf16_lt(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_FLE: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf.v = f32_le(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf.v = f64_le(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf.v[0] = 0;
                    resultf.v[1] = f128_le(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = f16_le(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf.v = bf16_le(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_MIN: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_min(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_min(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                // TODO: Missing softfloat function
                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_min(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_min(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_min(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_MAX: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    resultf = f32_max(af, bf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    resultf = f64_max(af, bf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                // TODO: Missing softfloat function
                case FMT_QUAD: {
                    float128_t af, bf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    resultf = f128_max(af, bf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = f16_max(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    resultf = bf16_max(af, bf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        /* TODO: Sign injection instructions */

        /* TODO: float to int instructions */

        /* TODO: float to float instructions */

        case OP_SQRT: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    resultf = f32_sqrt(af);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    resultf = f64_sqrt(af);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    resultf = f128_sqrt(af);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    resultf = f16_sqrt(af);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    resultf = bf16_sqrt(af);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_CLASS: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    resultf.v = f32_classify(af);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    resultf.v = f64_classify(af);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    resultf.v[0] = 0;
                    resultf.v[1] = f128_classify(af);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    resultf.v = f16_classify(af);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    resultf.v = bf16_classify(af);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        case OP_FMADD: {
            
            switch (*operandFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, cf, resultf;
                    UINT128_TO_FLOAT32(af, a);
                    UINT128_TO_FLOAT32(bf, b);
                    UINT128_TO_FLOAT32(cf, b);
                    resultf = f32_mulAdd(af, bf, cf);
                    FLOAT32_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, cf, resultf;
                    UINT128_TO_FLOAT64(af, a);
                    UINT128_TO_FLOAT64(bf, b);
                    UINT128_TO_FLOAT64(cf, b);
                    resultf = f64_mulAdd(af, bf, cf);
                    FLOAT64_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_QUAD: {
                    float128_t af, bf, cf, resultf;
                    UINT128_TO_FLOAT128(af, a);
                    UINT128_TO_FLOAT128(bf, b);
                    UINT128_TO_FLOAT128(bf, c);
                    resultf = f128_mulAdd(af, bf, cf);
                    FLOAT128_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_HALF: {
                    float16_t af, bf, cf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    UINT128_TO_FLOAT16(cf, b);
                    resultf = f16_mulAdd(af, bf, cf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

                case FMT_BF16: {
                    float16_t af, bf, cf, resultf;
                    UINT128_TO_FLOAT16(af, a);
                    UINT128_TO_FLOAT16(bf, b);
                    UINT128_TO_FLOAT16(bf, c);
                    resultf = bf16_mulAdd(af, bf, cf);
                    FLOAT16_TO_UINT128(result, resultf);
                    break;
                }

            }

            break;
        }

        /* TODO: other FMA style instructions */

        /* ... */

    }

    *flags = softFloat_getFlags();
    softfloat_getIntermResults(intermResult);
}

// TODO move to own file
float128_t f128_min(float128_t a, float128_t b) 
{ 
    bool less = f128_lt_quiet(a, b) || 
                (f128_eq(a, b) && signF128UI64(a.v[0])); 

    if (isNaNF128UI(a.v[0], a.v[1]) && isNaNF128UI(b.v[0], b.v[1])) { 
        union ui128_f128 ui; 
        ui.ui.v64 = defaultNaNF128UI64; 
        ui.ui.v0  = defaultNaNF128UI0; 
        return ui.f; 
    } else { 
        return (less || isNaNF128UI(b.v[0], b.v[1])) ? a : b; 
    } 
}

float128_t f128_max(float128_t a, float128_t b)
{
    bool greater = f128_lt_quiet(b, a) ||
                   (f128_eq(b, a) && signF128UI64(b.v[0]));

    if (isNaNF128UI(a.v[0], a.v[1]) && isNaNF128UI(b.v[0], b.v[1])) {
        union ui128_f128 ui;
        ui.ui.v64 = defaultNaNF128UI64;
        ui.ui.v0  = defaultNaNF128UI0;
        return ui.f;
    } else {
        return (greater || isNaNF128UI(b.v[0], b.v[1])) ? a : b;
    }
}



int main(int argc, char *argv[]) {
    bool suppress_error_check = false;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Usage: %s <input_file|- for stdin> <output_file|- for stdout> [--no-error-check]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 4 && strcmp(argv[3], "--no-error-check") == 0) {
        suppress_error_check = true;
    }


    FILE *fin = strcmp(argv[1], "-") == 0 ? stdin : fopen(argv[1], "r");
    if (!fin) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *fout = strcmp(argv[2], "-") == 0 ? stdout : fopen(argv[2], "w");
    if (!fout) {
        perror("Error opening output file");
        if (fin != stdin) fclose(fin);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fin)) {
        // Strip newline
        line[strcspn(line, "\r\n")] = '\0';

        char     op_str[MAX_TOKEN_LEN + 1]; // plus one for space for null terminator
        char     rm_str[MAX_TOKEN_LEN + 1];
        char      a_str[MAX_TOKEN_LEN + 1];
        char      b_str[MAX_TOKEN_LEN + 1];
        char      c_str[MAX_TOKEN_LEN + 1];
        char  opFmt_str[MAX_TOKEN_LEN + 1];
        char    res_str[MAX_TOKEN_LEN + 1];
        char resFmt_str[MAX_TOKEN_LEN + 1];
        char  flags_str[MAX_TOKEN_LEN + 1];

        if (line[0] == '/' && line[1] == '/') continue;

        if (sscanf(line, "%48[^_]_%48[^_]_%48[^_]_%48[^_]_%48[^_]_%48[^_]_%48[^_]_%48[^_]_%48[^_ \t\r\n]", 
            op_str, rm_str, a_str, b_str, c_str, opFmt_str, res_str, resFmt_str, flags_str) != 9) {
            fprintf(stderr, "Skipping malformed line: %s\n", line);
            continue;
        }


        // unpack test vector tokens into integers to pass to the reference model

        uint32_t       op        = parse_hex_128(op_str       ).lower;
        uint8_t        rm        = parse_hex_128(rm_str       ).lower;
        uint128_t      a         = parse_hex_128(a_str        )      ;
        uint128_t      b         = parse_hex_128(b_str        )      ;
        uint128_t      c         = parse_hex_128(c_str        )      ;
        uint8_t        opFmt     = parse_hex_128(opFmt_str    ).lower;
        uint8_t        resFmt    = parse_hex_128(resFmt_str   ).lower;
        uint128_t      res       = parse_hex_128(res_str      )      ;
        uint8_t        flags     = parse_hex_128(flags_str    ).lower;
        
        
        uint128_t      newRes;
        uint8_t        newFlags;
        intermResult_t intermRes;


        // Call reference model
                
        reference_model(&op,
                        &rm,
                        &a, 
                        &b, 
                        &c, 
                        &opFmt, 
                        &resFmt,

                        &newRes,
                        &newFlags,
                        &intermRes );

        // Write cover vector (append intermediate result to test vector)

        fprintf(fout, "%08x_%02x_%016llx%016llx_%016llx%016llx_%016llx%016llx_%02x_%016llx%016llx_%02x_%02x_%01x_%08x_%016llx%016llx%016llx\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, intermRes.sig0, intermRes.sigExtra);
        /*
        switch (resFmt) {
            // if (suppress_error_check) {
            //     line = "%08x_%02x_%016x_%016x_%016x_%016x_%016x_%016x_%02x_%016x_%016x_%02x_%02x"
            // }

            case FMT_QUAD: {
                fprintf(fout, "%08x_%02x_%016x%016x_%016x%016x_%016x%016x_%02x_%016x%016x_%02x_%02x_%01x_%08x_%016x%016x%016x\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, intermRes.sig0, intermRes.sigExtra);
                break;
            }
            case FMT_DOUBLE: {
                fprintf(fout, "%08x_%02x_%016x%016x_%016x%016x_%016x%016x_%02x_%016x%016x_%02x_%02x_%01x_%08x_%016x%016x%016x\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, intermRes.sig0, intermRes.sigExtra);
                break;
            }
            case FMT_SINGLE: {
                fprintf(fout, "%08x_%02x_%016x%016x_%016x%016x_%016x%016x_%02x_%016x%016x_%02x_%02x_%01x_%08x_%08x%08x%016x%016x\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, 0x0, intermRes.sig0, intermRes.sigExtra);
                break;
            }
            case FMT_HALF: {
                fprintf(fout, "%08x_%02x_%016x%016x_%016x%016x_%016x%016x_%02x_%016x%016x_%02x_%02x_%01x_%08x_%04x%012x%016x%016x\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, 0x0, intermRes.sig0, intermRes.sigExtra);
                break;
            }
            case FMT_BF16: {
                fprintf(fout, "%08x_%02x_%016x%016x_%016x%016x_%016x%016x_%02x_%016x%016x_%02x_%02x_%01x_%08x_%04x%012x%016x%016x\n", 
                        op, rm, a.upper, a.lower, b.upper, b.lower, c.upper, c.lower, opFmt, newRes.upper, newRes.lower, resFmt, newFlags, 
                        intermRes.sign, intermRes.exp, intermRes.sig64, 0x0, intermRes.sig0, intermRes.sigExtra);
                break;
            }
        }
            */


        // printf("INTERM SIG IS %016x\n\n", intermRes.sig64);

        // confirm softfloat output matches testvectors
        if (!suppress_error_check) {
            if (res.upper   != newRes.upper   || res.lower   != newRes.lower ||     // outputs don't match
                flags != newFlags                                              ) {  // flags   don't match
                fprintf(stderr, "Error: testvector output doesn't match expected value\nTestVector output: %016llx%016llx\nExpected output:   %016llx%016llx\nTestVector Flags: %02x\nExpected Flags: %02x\nOperation: %08x\n", 
                    res.upper, res.lower, newRes.upper, newRes.lower, flags, newFlags, op);
                fclose(fin);
                fclose(fout);
                return EXIT_FAILURE;
            }
        } 
    }

    if (fin  != stdin)  fclose(fin);
    if (fout != stdout) fclose(fout);

    return EXIT_SUCCESS;
}
