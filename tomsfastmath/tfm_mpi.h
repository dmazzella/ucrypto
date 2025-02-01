/*
 *
 * Porting to Micropython
 * Copyright (c) 2016-2025 Damiano Mazzella
 *
 */

/* TomsFastMath, a fast ISO C bignum library.
 *
 * This project is meant to fill in where LibTomMath
 * falls short.  That is speed ;-)
 *
 * This project is public domain and free for all purposes.
 *
 * Tom St Denis, tomstdenis@gmail.com
 */
#ifndef TFM_PRIVATE_H_
#define TFM_PRIVATE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/* 0xMaMiPaDe
 * Major
 * Minor
 * Patch
 * Development - 00=release, 01=in-development
 */
#define TFM_VERSION_MAJ 0
#define TFM_VERSION_MIN 13
#define TFM_VERSION_PAT 1
#define TFM_VERSION_DEV 1

#define TFM_VERSION PRIVATE__TFM_VERSION_4(TFM_VERSION_MAJ, \
                                           TFM_VERSION_MIN, \
                                           TFM_VERSION_PAT, \
                                           TFM_VERSION_DEV)

#define TFM_VERSION_S PRIVATE__TFM_CONC(TFM_VERSION_MAJ, \
                                        TFM_VERSION_MIN, \
                                        TFM_VERSION_PAT, \
                                        TFM_VERSION_DEV)

/* Please use the `TFM_VERSION_3()` macro if you want to compile-time check
 * for a specific TFM version.
 * Your code could look as follows:

#if TFM_VERSION <= TFM_VERSION_3(0, 13, 1)
// do stuff to work with old TFM
#else
// do stuff to work with new TFM
#endif

 */

#define TFM_VERSION_3(maj, min, pat) PRIVATE__TFM_VERSION_4(maj, min, pat, 0)

/* Private stuff from here on.
 * As said by Stanley Kirk Burrell in 1989 "You can't touch this" */
#define PRIVATE__TFM_VERSION_4(maj, min, pat, dev) ((maj) << 24 | (min) << 16 | (pat) << 8 | (dev))

#define PRIVATE__TFM_VERSION_DEV_STR_0
#define PRIVATE__TFM_VERSION_DEV_STR_1 "-next"

#define PRIVATE__TFM_VERSION_PASTE(v) PRIVATE__TFM_VERSION_DEV_STR_##v
#define PRIVATE__TFM_VERSION_DEV_STR(v) PRIVATE__TFM_VERSION_PASTE(v)

#define PRIVATE__TFM_STR(s) #s
#define PRIVATE__TFM_CONC(maj, min, pat, dev) "v" PRIVATE__TFM_STR(maj) "." PRIVATE__TFM_STR(min) "." PRIVATE__TFM_STR(pat) \
    PRIVATE__TFM_VERSION_DEV_STR(dev)
/* End of private stuff */

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

/* externally define this symbol to ignore the default settings, useful for changing the build from the make process */
#ifndef TFM_ALREADY_SET

/* do we want the large set of small multiplications ?
   Enable these if you are going to be doing a lot of small (<= 16 digit) multiplications say in ECC
   Or if you're on a 64-bit machine doing RSA as a 1024-bit integer == 16 digits ;-)
 */
/* #define TFM_SMALL_SET */

/* do we want huge code
   Enable these if you are doing 20, 24, 28, 32, 48, 64 digit multiplications (useful for RSA)
   Less important on 64-bit machines as 32 digits == 2048 bits
 */
#if 0
#define TFM_MUL3
#define TFM_MUL4
#define TFM_MUL6
#define TFM_MUL7
#define TFM_MUL8
#define TFM_MUL9
#define TFM_MUL12
#define TFM_MUL17
#define TFM_MUL20
#define TFM_MUL24
#define TFM_MUL28
#define TFM_MUL32
#define TFM_MUL48
#define TFM_MUL64
#endif

#if 0
#define TFM_SQR3
#define TFM_SQR4
#define TFM_SQR6
#define TFM_SQR7
#define TFM_SQR8
#define TFM_SQR9
#define TFM_SQR12
#define TFM_SQR17
#define TFM_SQR20
#define TFM_SQR24
#define TFM_SQR28
#define TFM_SQR32
#define TFM_SQR48
#define TFM_SQR64
#endif

/* do we want some overflow checks
   Not required if you make sure your numbers are within range (e.g. by default a modulus for fp_exptmod() can only be upto 2048 bits long)
 */
#define TFM_CHECK

/* Is the target a P4 Prescott
 */
/* #define TFM_PRESCOTT */

/* Do we want timing resistant fp_exptmod() ?
 * This makes it slower but also timing invariant with respect to the exponent
 */
#define TFM_TIMING_RESISTANT

#define USE_MEMSET

#define TFM_NO_ASM

// #define TFM_ECC192
// #define TFM_ECC224
// #define TFM_ECC256
// #define TFM_ECC384
// #define TFM_ECC512
// #define TFM_RSA512
// #define TFM_RSA1024
// #define TFM_RSA2048

#if defined(__thumb2__) || defined(__thumb__) || defined(__arm__)
#define TFM_ARM
#endif

#endif

/* Max size of any number in bits.  Basically the largest size you will be multiplying
 * should be half [or smaller] of FP_MAX_SIZE-four_digit
 *
 * You can externally define this or it defaults to 4096-bits [allowing multiplications upto 2048x2048 bits ]
 */
#ifndef FP_MAX_SIZE
#define FP_MAX_SIZE ((2048 * 2) + (8 * DIGIT_BIT))
#endif

/* will this lib work? */
#if (CHAR_BIT & 7)
#error CHAR_BIT must be a multiple of eight.
#endif
#if FP_MAX_SIZE % CHAR_BIT
#error FP_MAX_SIZE must be a multiple of CHAR_BIT
#endif

#if __SIZEOF_LONG__ == 8
#define FP_64BIT
#endif

/* autodetect x86-64 and make sure we are using 64-bit digits with x86-64 asm */
#if defined(__x86_64__)
#if defined(TFM_X86) || defined(TFM_SSE2) || defined(TFM_ARM)
#error x86-64 detected, x86-32/SSE2/ARM optimizations are not valid!
#endif
#if !defined(TFM_X86_64) && !defined(TFM_NO_ASM)
#define TFM_X86_64
#endif
#endif
#if defined(TFM_X86_64)
#if !defined(FP_64BIT)
#define FP_64BIT
#endif
#endif

/* try to detect x86-32 */
#if defined(__i386__) && !defined(TFM_SSE2)
#if defined(TFM_X86_64) || defined(TFM_ARM)
#error x86-32 detected, x86-64/ARM optimizations are not valid!
#endif
#if !defined(TFM_X86) && !defined(TFM_NO_ASM)
#define TFM_X86
#endif
#endif

/* make sure we're 32-bit for x86-32/sse/arm/ppc32 */
#if (defined(TFM_X86) || defined(TFM_SSE2) || defined(TFM_ARM) || defined(TFM_PPC32)) && defined(FP_64BIT)
#warning x86-32, SSE2 and ARM, PPC32 optimizations require 32-bit digits (undefining)
#undef FP_64BIT
#endif

/* multi asms? */
#ifdef TFM_X86
#define TFM_ASM
#endif
#ifdef TFM_X86_64
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif
#ifdef TFM_SSE2
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif
#ifdef TFM_ARM
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif
#ifdef TFM_PPC32
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif
#ifdef TFM_PPC64
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif
#ifdef TFM_AVR32
#ifdef TFM_ASM
#error TFM_ASM already defined!
#endif
#define TFM_ASM
#endif

/* we want no asm? */
#ifdef TFM_NO_ASM
#undef TFM_X86
#undef TFM_X86_64
#undef TFM_SSE2
#undef TFM_ARM
#undef TFM_PPC32
#undef TFM_PPC64
#undef TFM_AVR32
#undef TFM_ASM
#endif

/* ECC helpers */
#ifdef TFM_ECC192
#ifdef FP_64BIT
#define TFM_MUL3
#define TFM_SQR3
#else
#define TFM_MUL6
#define TFM_SQR6
#endif
#endif

#ifdef TFM_ECC224
#ifdef FP_64BIT
#define TFM_MUL4
#define TFM_SQR4
#else
#define TFM_MUL7
#define TFM_SQR7
#endif
#endif

#ifdef TFM_ECC256
#ifdef FP_64BIT
#define TFM_MUL4
#define TFM_SQR4
#else
#define TFM_MUL8
#define TFM_SQR8
#endif
#endif

#ifdef TFM_ECC384
#ifdef FP_64BIT
#define TFM_MUL6
#define TFM_SQR6
#else
#define TFM_MUL12
#define TFM_SQR12
#endif
#endif

#ifdef TFM_ECC521
#ifdef FP_64BIT
#define TFM_MUL9
#define TFM_SQR9
#else
#define TFM_SMALL_SET
#define TFM_MUL17
#define TFM_SQR17
#endif
#endif

/* RSA helpers */
#ifdef TFM_RSA512
#ifdef FP_64BIT
#define TFM_MUL9
#define TFM_SQR9
#else
#define TFM_SMALL_SET
#define TFM_MUL17
#define TFM_SQR17
#endif
#endif

#ifdef TFM_RSA1024
#ifdef FP_64BIT
#define TFM_MUL17
#define TFM_SQR17
#else
#define TFM_MUL32
#define TFM_SQR32
#endif
#endif

#ifdef TFM_RSA2048
#ifdef FP_64BIT
#define TFM_MUL32
#define TFM_SQR32
#else
#define TFM_MUL64
#define TFM_SQR64
#endif
#endif

#if 0
#define TFM_MUL3
#define TFM_SQR3
#define TFM_MUL4
#define TFM_SQR4
#define TFM_MUL6
#define TFM_SQR6
#define TFM_MUL7
#define TFM_SQR7
#define TFM_MUL8
#define TFM_SQR8
#define TFM_MUL9
#define TFM_SQR9
#define TFM_MUL12
#define TFM_SQR12
#define TFM_MUL17
#define TFM_SQR17
#define TFM_SMALL_SET
#define TFM_MUL20
#define TFM_SQR20
#define TFM_MUL24
#define TFM_SQR24
#define TFM_MUL28
#define TFM_SQR28
#define TFM_MUL32
#define TFM_SQR32
#define TFM_MUL48
#define TFM_SQR48
#define TFM_MUL64
#define TFM_SQR64
#endif

/* use arc4random on platforms that support it */
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define FP_GEN_RANDOM() arc4random()
#define FP_GEN_RANDOM_MAX 0xffffffff
#endif

/* use rng_get() as rand function */
#if defined(__thumb2__) || defined(__thumb__) || defined(__arm__)
#if MICROPY_HW_ENABLE_RNG
#include "rng.h"
#ifndef FP_GEN_RANDOM
#define FP_GEN_RANDOM() rng_get()
#define FP_GEN_RANDOM_MAX RAND_MAX
#endif
#endif
#endif

/* use rand() as fall-back if there's no better rand function */
#ifndef FP_GEN_RANDOM
#define FP_GEN_RANDOM() rand()
#define FP_GEN_RANDOM_MAX RAND_MAX
#endif

/* some default configurations.
 */
#if defined(FP_64BIT)
/* for GCC only on supported platforms */
#ifndef CRYPT
typedef unsigned long long ulong64;
#endif /* CRYPT */

typedef ulong64 fp_digit;
#define SIZEOF_FP_DIGIT 8
#define DIGIT_SHIFT 6
typedef unsigned long fp_word __attribute__((mode(TI)));

#else

/* this is to make porting into LibTomCrypt easier :-) */
#ifndef CRYPT
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 ulong64;
typedef signed __int64 long64;
#else
typedef unsigned long long ulong64;
typedef signed long long long64;
#endif /* defined(_MSC_VER) ... */
#endif /* CRYPT */

typedef unsigned int fp_digit;
#define SIZEOF_FP_DIGIT 4
#define DIGIT_SHIFT 5
typedef ulong64 fp_word;
#endif /* FP_64BIT */

/* # of digits this is */
#define DIGIT_BIT ((CHAR_BIT) * SIZEOF_FP_DIGIT)
#define FP_MASK (fp_digit)(-1)
#define FP_SIZE (FP_MAX_SIZE / DIGIT_BIT)

/* signs */
#define FP_ZPOS 0
#define FP_NEG 1

/* return codes */
#define FP_OKAY 0
#define FP_VAL 1
#define FP_MEM 2

/* equalities */
#define FP_LT -1 /* less than */
#define FP_EQ 0  /* equal to */
#define FP_GT 1  /* greater than */

/* replies */
#define FP_YES 1 /* yes response */
#define FP_NO 0  /* no response */

/* a FP type */
typedef struct
{
   fp_digit dp[FP_SIZE];
   int used,
       sign;
} fp_int;

/* functions */

char fp_to_upper(char str);

/* returns a TFM ident string useful for debugging... */
const char *fp_ident(void);

/* initialize [or zero] an fp int */
#define fp_init(a) (void)memset((a), 0, sizeof(fp_int))
#define fp_zero(a) fp_init(a)

/* zero/even/odd ? */
#define fp_iszero(a) (((a)->used == 0) ? FP_YES : FP_NO)
#define fp_iseven(a) (((a)->used == 0 || (((a)->dp[0] & 1) == 0)) ? FP_YES : FP_NO)
#define fp_isodd(a) (((a)->used > 0 && (((a)->dp[0] & 1) == 1)) ? FP_YES : FP_NO)

/* set to a small digit */
void fp_set(fp_int *a, fp_digit b);

/* makes a pseudo-random int of a given size */
void fp_rand(fp_int *a, int digits);

/* copy from a to b */
void fp_copy(const fp_int *a, fp_int *b);

#define fp_init_copy(a, b) fp_copy(b, a)

/* clamp digits */
#define fp_clamp(a)                                    \
   {                                                   \
      while ((a)->used && (a)->dp[(a)->used - 1] == 0) \
         --((a)->used);                                \
      (a)->sign = (a)->used ? (a)->sign : FP_ZPOS;     \
   }

/* negate and absolute */
#define fp_neg(a, b)  \
   {                  \
      fp_copy(a, b);  \
      (b)->sign ^= 1; \
      fp_clamp(b);    \
   }
#define fp_abs(a, b) \
   {                 \
      fp_copy(a, b); \
      (b)->sign = 0; \
   }

/* right shift x digits */
void fp_rshd(fp_int *a, int x);

/* left shift x digits */
void fp_lshd(fp_int *a, int x);

/* signed comparison */
int fp_cmp(const fp_int *a, const fp_int *b);

/* unsigned comparison */
int fp_cmp_mag(const fp_int *a, const fp_int *b);

/* power of 2 operations */
void fp_div_2d(const fp_int *a, int b, fp_int *c, fp_int *d);
void fp_mod_2d(const fp_int *a, int b, fp_int *c);
void fp_mul_2d(const fp_int *a, int b, fp_int *c);
void fp_2expt(fp_int *a, int b);
void fp_mul_2(const fp_int *a, fp_int *c);
void fp_div_2(const fp_int *a, fp_int *c);

/* Counts the number of lsbs which are zero before the first zero bit */
int fp_cnt_lsb(const fp_int *a);

/* c = a + b */
void fp_add(const fp_int *a, const fp_int *b, fp_int *c);

/* c = a - b */
void fp_sub(const fp_int *a, const fp_int *b, fp_int *c);

/* c = a * b */
void fp_mul(const fp_int *a, const fp_int *b, fp_int *c);

/* b = a*a  */
void fp_sqr(const fp_int *a, fp_int *b);

/* a/b => cb + d == a */
int fp_div(const fp_int *a, const fp_int *b, fp_int *c, fp_int *d);

/* c = a mod b, 0 <= c < b  */
int fp_mod(const fp_int *a, const fp_int *b, fp_int *c);

/* compare against a single digit */
int fp_cmp_d(const fp_int *a, fp_digit b);

/* c = a + b */
void fp_add_d(const fp_int *a, fp_digit b, fp_int *c);

/* c = a - b */
void fp_sub_d(const fp_int *a, fp_digit b, fp_int *c);

/* c = a * b */
void fp_mul_d(const fp_int *a, fp_digit b, fp_int *c);

/* a/b => cb + d == a */
int fp_div_d(const fp_int *a, fp_digit b, fp_int *c, fp_digit *d);

/* c = a mod b, 0 <= c < b  */
int fp_mod_d(const fp_int *a, fp_digit b, fp_digit *c);

/* ---> number theory <--- */
/* d = a + b (mod c) */
int fp_addmod(const fp_int *a, const fp_int *b, const fp_int *c, fp_int *d);

/* d = a - b (mod c) */
int fp_submod(const fp_int *a, const fp_int *b, const fp_int *c, fp_int *d);

/* d = a * b (mod c) */
int fp_mulmod(const fp_int *a, const fp_int *b, const fp_int *c, fp_int *d);

/* c = a * a (mod b) */
int fp_sqrmod(const fp_int *a, const fp_int *b, fp_int *c);

/* c = 1/a (mod b) */
int fp_invmod(const fp_int *a, const fp_int *b, fp_int *c);

/* c = (a, b) */
void fp_gcd(const fp_int *a, const fp_int *b, fp_int *c);

/* c = [a, b] */
void fp_lcm(const fp_int *a, const fp_int *b, fp_int *c);

/* setups the montgomery reduction */
int fp_montgomery_setup(const fp_int *a, fp_digit *mp);

/* computes a = B**n mod b without division or multiplication useful for
 * normalizing numbers in a Montgomery system.
 */
void fp_montgomery_calc_normalization(fp_int *a, const fp_int *b);

/* computes x/R == x (mod N) via Montgomery Reduction */
void fp_montgomery_reduce(fp_int *a, const fp_int *m, fp_digit mp);

/* d = a**b (mod c) */
int fp_exptmod(const fp_int *a, const fp_int *b, const fp_int *c, fp_int *d);

/* primality stuff */

/* perform a Miller-Rabin test of a to the base b and store result in "result" */
void fp_prime_miller_rabin(const fp_int *a, const fp_int *b, int *result);

#define FP_PRIME_SIZE 256
/* 256 trial divisions + 8 Miller-Rabins, returns FP_YES if probable prime  */
int fp_isprime(const fp_int *a);
/* extended version of fp_isprime, do 't' Miller-Rabins instead of only 8 */
int fp_isprime_ex(const fp_int *a, int t);

/* Primality generation flags */
#define TFM_PRIME_BBS 0x0001      /* BBS style prime */
#define TFM_PRIME_SAFE 0x0002     /* Safe prime (p-1)/2 == prime */
#define TFM_PRIME_2MSB_OFF 0x0004 /* force 2nd MSB to 0 */
#define TFM_PRIME_2MSB_ON 0x0008  /* force 2nd MSB to 1 */

/* callback for fp_prime_random, should fill dst with random bytes and return how many read [upto len] */
typedef int tfm_prime_callback(unsigned char *dst, int len, void *dat);

#define fp_prime_random(a, t, size, bbs, cb, dat) fp_prime_random_ex(a, t, ((size) * 8) + 1, (bbs == 1) ? TFM_PRIME_BBS : 0, cb, dat)

int fp_prime_random_ex(fp_int *a, int t, int size, int flags, tfm_prime_callback cb, void *dat);

/* radix conversions */
int fp_count_bits(const fp_int *a);

int fp_unsigned_bin_size(const fp_int *a);
void fp_read_unsigned_bin(fp_int *a, const unsigned char *b, int c);
void fp_to_unsigned_bin(const fp_int *a, unsigned char *b);

int fp_signed_bin_size(const fp_int *a);
void fp_read_signed_bin(fp_int *a, const unsigned char *b, int c);
void fp_to_signed_bin(const fp_int *a, unsigned char *b);

int fp_read_radix(fp_int *a, const char *str, int radix);

int fp_radix_size(const fp_int *a, int radix, int *size);
int fp_toradix(const fp_int *a, char *str, int radix);
int fp_toradix_n(const fp_int *a, char *str, int radix, int maxlen);
/*
 * Private symbols
 * ---------------
 *
 * On Unix symbols can be marked as hidden if tomsfastmath is compiled
 * as a shared object. By default, symbols are visible.
 */
#if defined(__GNUC__) && __GNUC__ >= 4 && !defined(_WIN32) && !defined(__CYGWIN__)
#define FP_PRIVATE __attribute__((visibility("hidden")))
#else
#define FP_PRIVATE
#endif

/* VARIOUS LOW LEVEL STUFFS */
FP_PRIVATE void s_fp_add(const fp_int *a, const fp_int *b, fp_int *c);
FP_PRIVATE void s_fp_sub(const fp_int *a, const fp_int *b, fp_int *c);
FP_PRIVATE void fp_reverse(unsigned char *s, int len);

FP_PRIVATE void fp_mul_comba(const fp_int *A, const fp_int *B, fp_int *C);

#ifdef TFM_SMALL_SET
FP_PRIVATE void fp_mul_comba_small(const fp_int *A, const fp_int *B, fp_int *C);
#endif

#ifdef TFM_MUL3
FP_PRIVATE void fp_mul_comba3(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL4
FP_PRIVATE void fp_mul_comba4(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL6
FP_PRIVATE void fp_mul_comba6(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL7
FP_PRIVATE void fp_mul_comba7(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL8
FP_PRIVATE void fp_mul_comba8(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL9
FP_PRIVATE void fp_mul_comba9(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL12
FP_PRIVATE void fp_mul_comba12(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL17
FP_PRIVATE void fp_mul_comba17(const fp_int *A, const fp_int *B, fp_int *C);
#endif

#ifdef TFM_MUL20
FP_PRIVATE void fp_mul_comba20(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL24
FP_PRIVATE void fp_mul_comba24(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL28
FP_PRIVATE void fp_mul_comba28(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL32
FP_PRIVATE void fp_mul_comba32(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL48
FP_PRIVATE void fp_mul_comba48(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL64
FP_PRIVATE void fp_mul_comba64(const fp_int *A, const fp_int *B, fp_int *C);
#endif

FP_PRIVATE void fp_sqr_comba(const fp_int *A, fp_int *B);

#ifdef TFM_SMALL_SET
FP_PRIVATE void fp_sqr_comba_small(const fp_int *A, fp_int *B);
#endif

#ifdef TFM_SQR3
FP_PRIVATE void fp_sqr_comba3(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR4
FP_PRIVATE void fp_sqr_comba4(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR6
FP_PRIVATE void fp_sqr_comba6(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR7
FP_PRIVATE void fp_sqr_comba7(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR8
FP_PRIVATE void fp_sqr_comba8(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR9
FP_PRIVATE void fp_sqr_comba9(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR12
FP_PRIVATE void fp_sqr_comba12(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR17
FP_PRIVATE void fp_sqr_comba17(const fp_int *A, fp_int *B);
#endif

#ifdef TFM_SQR20
FP_PRIVATE void fp_sqr_comba20(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR24
FP_PRIVATE void fp_sqr_comba24(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR28
FP_PRIVATE void fp_sqr_comba28(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR32
FP_PRIVATE void fp_sqr_comba32(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR48
FP_PRIVATE void fp_sqr_comba48(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR64
FP_PRIVATE void fp_sqr_comba64(const fp_int *A, fp_int *B);
#endif
FP_PRIVATE extern const char *fp_s_rmap;

void fp_and(const fp_int *a, const fp_int *b, fp_int *c);
void fp_or(const fp_int *a, const fp_int *b, fp_int *c);
void fp_xor(const fp_int *a, const fp_int *b, fp_int *c);
int fp_tstbit(const fp_int a, int bit_index);

#endif

/* $Source$ */
/* $Revision$ */
/* $Date$ */
