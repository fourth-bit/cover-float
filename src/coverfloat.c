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

    result->sign     = softfloat_intermediateResult->sign;
    result->exp      = softfloat_intermediateResult->exp;
    result->sig64    = softfloat_intermediateResult->sig64;
    result->sig0     = softfloat_intermediateResult->sig0;
    result->sigExtra = softfloat_intermediateResult->sigExtra;

}

/*

AI CODE NEEDS MODIFICATION
*/

#define TEST_VECTOR_SIZE // TODO
#define COVER_VECTOR_SIZE // TODO

#define MAX_LINE_LEN 256
#define MAX_TOKEN_LEN 128


// void reference_model( const char * op,
//                       const char * rm,
//                       const char * a, 
//                       const char * b, 
//                       const char * c, 
//                       const char * aFmt, 
//                       const char * bFmt, 
//                       const char * cFmt, 
//                             char * result,
//                       const char * resultFmt,
//                             char * flags 
//                         // need intermediate results outputs for cover vectors
//                         ) {
    
//     uint128_t aInt;
//     uint128_t bInt;
//     uint128_t cInt;
//     uint128_t resultInt;

//     // convert inputs to integers

//     // nested switch statements to call softfloat functions

//     // convert back to strings

// }

void reference_model( const uint32_t       * op,
                      const uint8_t        * rm,
                      const uint128_t      * a, 
                      const uint128_t      * b, 
                      const uint128_t      * c, 
                      const uint8_t        * aFmt, 
                      const uint8_t        * bFmt, 
                      const uint8_t        * cFmt, 

                      uint128_t            * result,
                      uint8_t              * resultFmt,
                      uint8_t              * flags ,

                      intermResult_t       * intermResult ) {
    
    
    // clear flags so we ger only triggered flags
    // TODO


    // nested switch statements to call softfloat functions

    switch (*op) {
        case OP_ADD: {
            
            switch (*aFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    af.val = (*a << 96) >> 96;
                    bf.val = (*b << 96) >> 96;
                    resultf = f32_add(af, bf);
                    * result = resultf.val;
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    af.val = (*a << 64) >> 64;
                    bf.val = (*b << 64) >> 64;
                    resultf = f64_add(af, bf);
                    * result = resultf.val;
                    break;
                }

                /* ... */

            }
            break;
        }

                case OP_ADD: {
            
            switch (*aFmt) {
                case FMT_SINGLE: {
                    float32_t af, bf, resultf;
                    af.val = (a << 96) >> 96;
                    bf.val = (b << 96) >> 96;
                    resultf = f32_sub(af, bf);
                    * result = resultf.val;
                    break;
                }

                case FMT_DOUBLE: {
                    float64_t af, bf, resultf;
                    af.val = (a << 64) >> 64;
                    bf.val = (b << 64) >> 64;
                    resultf = f64_sub(af, bf);
                    * result = resultf.val;
                    break;
                }

                /* ... */

            }
            break;
        }

        /* ... */

    }

    *flags = softFloat_getFlags();
    softfloat_getIntermResults(intermResult);
}

// TODO needs complete refactor...
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fin = fopen(argv[1], "r");
    if (!fin) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *fout = fopen(argv[2], "w");
    if (!fout) {
        perror("Error opening output file");
        fclose(fin);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fin)) {
        // Strip newline
        line[strcspn(line, "\r\n")] = '\0';

        char input1[MAX_TOKEN_LEN], input2[MAX_TOKEN_LEN], output[MAX_TOKEN_LEN];
        if (sscanf(line, "%127[^_]_%127[^_]_%127s", input1, input2, output) != 3) {
            fprintf(stderr, "Skipping malformed line: %s\n", line);
            continue;
        }

        char internal_state[MAX_TOKEN_LEN];
        char new_output[MAX_TOKEN_LEN];

        // Call reference model
        reference_model(input1, input2, internal_state, sizeof(internal_state),
                        new_output, sizeof(new_output));

        // Write new vector: input1_input2_internalstate_output
        fprintf(fout, "%s_%s_%s_%s\n", input1, input2, internal_state, new_output);

        // confirm softfloat output matches testvectors
        if (strcmp(output, new_output) != 0 ||  // outputs don't match
            strcmp(flags,  new_flags)  != 0) {  // flags don't match
            perror("Error: testvector output doesn't match expected value\nTestVector output: %s\nExpected output: %s", output, new_output);
            fclose(fin);
            fclose(fout);
            return EXIT_FAILURE;
        }
    }

    fclose(fin);
    fclose(fout);

    return EXIT_SUCCESS;
}
