import random
import cover_float.common as common
from cover_float.reference import run_test_vector

SRC1_OPS = [common.OP_SQRT]

SRC2_OPS = [common.OP_ADD,
            common.OP_SUB,
            common.OP_MUL,
            common.OP_DIV]
            # common.OP_REM]

SRC3_OPS = [common.OP_FMA,
            common.OP_FMADD,
            common.OP_FMSUB,
            common.OP_FNMADD,
            common.OP_FNMSUB]

# OP_QC     = "000000B0"
# OP_FEQ    = "000000B1"
# OP_SC     = "000000C0"
# OP_FLT    = "000000C1"
# OP_FLE    = "000000C2"
# OP_CLASS  = "000000D0"
# OP_MIN    = "000000E0"
# OP_MAX    = "000000F0"
# OP_CSN    = "00000100"
# OP_FSGNJ  = "00000101"
# OP_FSGNJN = "00000102"
# OP_FSGNJX = "00000103"

FMTS     = [common.FMT_SINGLE, common.FMT_DOUBLE, common.FMT_HALF, common.FMT_BF16, common.FMT_QUAD]
INT_FMTS = [common.FMT_INT, common.FMT_UINT, common.FMT_LONG, common.FMT_ULONG]


def generate_float(sign: int, exponent: int, mantissa: int, fmt: str) -> int:
    exponent += common.EXPONENT_BIASES[fmt]
    return (sign << (common.MANTISSA_BITS[fmt] + common.EXPONENT_BITS[fmt])) | (exponent << common.MANTISSA_BITS[fmt]) | mantissa

def generate_random_float(exponent: int, fmt: str) -> int:
    # sign = random.randint(0, 1)
    sign = 0
    mantissa = random.randint(0, (1 << common.MANTISSA_BITS[fmt]) - 1)
    # Add in the exponent bias for single-precision (127)
    float32 = generate_float(sign, exponent, mantissa, fmt)

    return float32

def generate_test_vector(op, in1, in2, in3, fmt1, fmt2, rnd_mode="00"):
    zero_padding = '0' * 32
    return f"{op}_{rnd_mode}_{in1:032x}_{in2:032x}_{in3:032x}_{fmt1}_{zero_padding}_{fmt2}_00\n"


def extract_rounding_info(cover_vector):
    fields = cover_vector.split('_')
    sgn = fields[-3]
    interm_significand = bin(int(fields[-1], 16))[2:]

    result_fmt = fields[-5]
    mantissa_length = common.MANTISSA_BITS[result_fmt]

    # if result_fmt == common.FMT_BF16:
    #     return { 'Sign': 0, 'LSB': 0, 'Guard': 0, 'Sticky': 0 }
    if interm_significand == '0':
        # breakpoint()
        return { 'Sign': 2, 'LSB': 0, 'Guard': 0, 'Sticky': 0 }

    lsb = interm_significand[mantissa_length]
    guard = interm_significand[mantissa_length + 1]
    sticky = interm_significand[mantissa_length + 2:]

    # if result_fmt == common.FMT_QUAD:
    #     quad_offset = 15
    #     lsb = interm_significand[mantissa_length - 1 + quad_offset]
    #     guard = interm_significand[mantissa_length + quad_offset]
    #     sticky = interm_significand[mantissa_length + 1 + quad_offset:]


    f32_mantissa = int(fields[-1][:16], 16)

    # print(bin(f32_mantissa & 0x1ff))
    # print(sticky)

    return {
        'Sign': int(sgn),
        'LSB': int(lsb),
        'Guard': int(guard),
        'Sticky': 1 if any(x == '1' for x in sticky) else 0,
    }

def write_fma_tests(test_f, cover_f, fmt):
    FMA_OPS = [
        common.OP_FMADD,
        common.OP_FMSUB,
        common.OP_FNMADD,
        common.OP_FNMSUB,
    ]

    targets = [
        {
            'Sign': (x & 1),
            'LSB': (x & 2) >> 1,
            'Guard': (x & 3) >> 2,
            'Sticky': 0,
        }
        for x in range(8)
    ]

    for op in FMA_OPS:
        for mode in common.ROUNDING_MODES:
            to_cover = targets[:]

            for _ in range(100):
                """
                    How does FMA actually work on softfloat? (and why we are not using the reference 
                    model to do our math)
                    
                    Softfloat is going to crush extra bits into one with the shiftJam logic, and in 
                    the f32 case, softfloat's rounding function takes a uint_fast32_t as input for
                    the significand. This means that it rounds based off of ~9 extra bits instead
                    of all of the generated sticky bits (so we cannot get preaddition results
                    with an OP_FMADD x, y, 0 call). 

                    The following is a calculation from s_mulAddF32.c:

                        sigC = (sigC | 0x00800000)<<6;

                        ...

                        sig64Z =
                            sigProd
                                + softfloat_shiftRightJam64(
                                    (uint_fast64_t) sigC<<32, expDiff );
                        sigZ = softfloat_shortShiftRightJam64( sig64Z, 32 );
                    
                    sig64Z is a uint_fast64_t, while sigZ is a uint_fast32_t. SigZ is the final answer
                    but what we want is sig64Z. The meaning of Jam is that bits shifted out of the integer
                    are "jammed" into a 1. Thus, we just need a faithful calculation of sigProd.

                    So, how is sigProd calculated?

                        sigA = (sigA | 0x00800000)<<7;
                        sigB = (sigB | 0x00800000)<<7;
                        sigProd = (uint_fast64_t) sigA * sigB;
                        if ( sigProd < UINT64_C( 0x2000000000000000 ) ) {
                            --expProd;
                            sigProd <<= 1;
                        }

                    And expProd?

                        expProd = expA + expB - 0x7E // 0x7e = 126
                """

                signA = random.randint(0, 1)
                signB = random.randint(0, 1)

                sigA_initial = random.randint(0, (1 << common.MANTISSA_BITS[fmt]) - 1)
                sigB_initial = random.randint(0, (1 << common.MANTISSA_BITS[fmt]) - 1)
                expA = random.randint(-10, 10) + common.EXPONENT_BIASES[fmt]
                expB = random.randint(-10, 10) + common.EXPONENT_BIASES[fmt]

                if fmt == common.FMT_HALF:
                    # Just be careful that we don't generate things that need
                    # to add a number that we don't have the exponents to add
                    expA = random.randint(-1, 6) + common.EXPONENT_BIASES[fmt]
                    expB = random.randint(-1, 6) + common.EXPONENT_BIASES[fmt]

                # Put in the leading one
                sigA = (sigA_initial | (1 << common.MANTISSA_BITS[fmt]))
                sigB = (sigB_initial | (1 << common.MANTISSA_BITS[fmt]))
                
                # Actually Multiply
                sigProd = sigA * sigB
                signProd = signA ^ signB # zero iff both are the same
                expProd = expA + expB - common.EXPONENT_BIASES[fmt] + 1
                
                # Correct for the actual operation
                if op == common.OP_FNMADD or op == common.OP_FNMSUB:
                    signProd ^= 1 # These ops induce a sign flip
                
                # Now we ensure that our leading one is in the correct bit, and the 
                # product exponent is correct
                if sigProd < (1 << (common.MANTISSA_BITS[fmt] * 2 + 1)):
                    sigProd <<= 1
                    expProd -= 1

                # The leading one should be in bit MANTISSA_BIT * 2 + 2, so
                # bits MANTISSA_BIT * 2 + 1 --> MANTISSA_BIT + 2 (inclusive) are mantissa
                # Thus, G = MANTISSA_BIT + 1, STICKY = MANTISSA_BIT --> 1
                mask = 2 ** (common.MANTISSA_BITS[fmt] + 1) - 1
                rounding_bits = sigProd & mask
                sticky_bits = rounding_bits & (mask >> 1)
                not_sticky = sigProd & (~mask)

                # Sticky bits should be aligned to already, so
                signC = signProd
                sigC_initial = 2 ** common.MANTISSA_BITS[fmt] - sticky_bits
                sigC = sigC_initial | (1 << common.MANTISSA_BITS[fmt])

                # Sign Flip if it is a subtraction op
                if op == common.OP_FMSUB or op == common.OP_FNMADD:
                    signC ^= 1

                # Figure out alignment
                expC = expProd - common.MANTISSA_BITS[fmt] - 1 
                expDiff = expProd - expC

                # Align sigC to correct bits of sigProd, the shifts are a no-op but
                # they are there for correctness
                sigZ64 = sigProd + ((sigC << (common.MANTISSA_BITS[fmt] + 1)) >> expDiff) 
                sigZ64 = sigProd + sigC

                # In some cases, especially in lower precision formats (i.e. bf16 and half), 
                # we get an "overflow" here (i.e. we move up an exponent and have to shift)
                # This means we can accidentally cause a shift of guard into the stickt bit
                # which we do not guarentee to be zero, so we check that here
                if len(bin(sigZ64)) > len(bin(sigProd)):
                    continue

                # Get new rounding info, if we want to log it
                new_rounding = sigZ64 & mask
                new_sticky = new_rounding & (mask >> 1)

                in1 = generate_float(signA, expA - common.EXPONENT_BIASES[fmt], sigA_initial, fmt)
                in2 = generate_float(signB, expB - common.EXPONENT_BIASES[fmt], sigB_initial, fmt)
                in3 = generate_float(signC, expC - common.EXPONENT_BIASES[fmt], sigC_initial, fmt)

                tv = generate_test_vector(op, in1, in2, in3, fmt, fmt, mode)
                result = run_test_vector(tv)
                rounding = extract_rounding_info(result)

                if rounding['Sticky'] != 0:
                    print("FMA Sticky Bit Generation Failed! This should not happen, please investigate")
                    print(f"\tInputs: signA={signA}, sigA={sigA:#x}, expA={expA}, signB={signB}, sigB={sigB:#x}, expB={expB}, fmt={fmt}, op={op}")

                if rounding in to_cover:
                    to_cover.remove(rounding)
                    print(result[:common.TEST_VECTOR_WIDTH_HEX_WITH_SEPARATORS], file=test_f)
                    print(result, file=cover_f)

                    # This means were done
                    if len(to_cover) == 0:
                        break
            else:
                # This catches a for loop that does not break, i.e. we don't hit every goal
                # if fmt != common.FMT_BF16: # We have no rounding info extraction on BF16
                print(fmt, mode, to_cover)
            
def write_add_sub_tests(test_f, cover_f, fmt):
    ops = [
        common.OP_ADD,
        common.OP_SUB,
    ]

    targets = [
        {
            'Sign': (x & 1),
            'LSB': (x & 2) >> 1,
            'Guard': (x & 3) >> 2,
            'Sticky': 0,
        }
        for x in range(8)
    ]

    for op in ops:
        for mode in common.ROUNDING_MODES:
            for _ in range(100):
                # Generate a random float for A
                signA = random.randint(0, 1)
                sigA_initial = random.randint(0, (1 << common.MANTISSA_BITS[fmt]) - 1)
                expA = random.randint(-10, 10) + common.EXPONENT_BIASES[fmt]

                # How can we get rounding bits to be what we want? 
                # For add and sub, unfortunately, there is no way to get a lot of manipulation 

                # TBD


def main():
    test_f = open("./tests/testvectors/B3_tv.txt", "w")
    cover_f = open("./tests/covervectors/B3_cv.txt", "w")

    # These are going to be for sticky = 0
    for fmt in FMTS:
        write_fma_tests(test_f, cover_f, fmt)
    return

    targets = [
        {
            'Sign': (x & 1),
            'LSB': (x & 2) >> 1,
            'Guard': (x & 4) >> 2,
            'Sticky': (x & 8) >> 3, # Sticky is one for all of these
        }
        for x in range(8, 16)
    ]

    targets = [x for x in targets if x['Sign'] == 0]

    misses = 0
    emmitted = 0
    total = 0

    for op in [*SRC1_OPS, *SRC2_OPS, *SRC3_OPS]:
        for fmt in [common.FMT_SINGLE]:
            for mode in common.ROUNDING_MODES:
                cover_goals = targets[:]
                if op == common.OP_SQRT or op == common.OP_REM:
                    cover_goals = [x for x in cover_goals if x['Sign'] == 0]

                for _ in range(100):
                    in1 = generate_random_float(random.randint(0, 5), fmt)
                    in2 = generate_random_float(random.randint(0, 5), fmt) if op in SRC2_OPS or op in SRC3_OPS else 0
                    in3 = generate_random_float(random.randint(0, 5), fmt) if op in SRC3_OPS else 0

                    tv = generate_test_vector(op, in1, in2, in3, fmt, fmt, mode)
                    cv = run_test_vector(tv)

                    # if op == OP_REM:
                    #     breakpoint()

                    rounding_results = extract_rounding_info(cv)

                    if rounding_results in cover_goals:
                        cover_goals.remove(rounding_results)
                        print(cv[:common.TEST_VECTOR_WIDTH_HEX_WITH_SEPARATORS], file=test_f)
                        print(cv, file=cover_f)
                        emmitted += 1
                    
                    if len(cover_goals) == 0:
                        break
                else:
                    print("Miss: ", op, fmt, len(cover_goals), cover_goals)
                    misses += len(cover_goals)
                total += len(targets)

    print(f"Hit rate: {emmitted/total}, {emmitted}, {total}")

if __name__ == "__main__":
    main()

"""
in1 = generate_random_float32(1)
in2 = generate_random_float32(3)

tv = generate_test_vector(OP_ADD, in1, in2, FMT_SINGLE, FMT_SINGLE)
cv = coverfloat.reference(tv)

info = extract_rounding_info(cv)
"""