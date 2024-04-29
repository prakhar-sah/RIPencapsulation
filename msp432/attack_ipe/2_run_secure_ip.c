/******************************************************************************
*
* Copyright (C) 2012 - 2018 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/
//*****************************************************************************
// Flash Mailbox example running secure IP
//
// Program makes a function call to the IP located at 0x00004000. The flash
// mailbox is not interacted with in this example because the secure IP should
// have been programmed in using the secure_ip example. This example shows
// how an application developer can work with secure IP that they do not have
// the source code to as long as the IP has been programmed into the device.
//
// If the secure IP has been succesfully programmed then this example should
// blink the Green LED at a slower rate than the secure_ip application.
//
//*****************************************************************************

#include <msp.h>
#include <stdint.h>
#include "driverlib.h"

// Secure IP must be programmed to 0x00004000
#define softwareIP1 ((void (*)(void))0x00004001)

void UART0_init(void);
void UART0_putchar(char c);
void UART0_puts(char* s);

/* UART0 is connected to virtual COM port through the USB debug connection */
void UART0_init(void) {
    EUSCI_A0->CTLW0 |= 1;             /* put in reset mode for config */
    EUSCI_A0->MCTLW = 0;              /* disable oversampling */
    EUSCI_A0->CTLW0 = 0x0081;         /* 1 stop bit, no parity, SMCLK, 8-bit data */
    EUSCI_A0->BRW = 4;               /* 3000000 / 115200 = 26 */
    P1->SEL0 |= 0x0C;                 /* P1.3, P1.2 for UART */
    P1->SEL1 &= ~0x0C;
    EUSCI_A0->CTLW0 &= ~1;            /* take UART out of reset mode */
}
void UART0_putchar(char c) {
    while(!(EUSCI_A0->IFG&0x02)) { }  /* wait for transmit buffer empty */
    EUSCI_A0->TXBUF = c;              /* send a char */
}

void UART0_puts(char* s) {
    while (*s != 0)             /* if not end of string */
    UART0_putchar(*s++);        /* send the character through UART0 */
}

volatile unsigned int COMPARE_VALUE = 7;
volatile uint16_t counter_val = 0;
volatile int j = 0;
volatile uint32_t pend_status = 0;
volatile uint32_t active_status = 0;
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;               // Stop watchdog timer

    //NVIC_DisableIRQ(TA0_0_IRQn);
    //MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    //MAP_Timer_A_clearTimer(TIMER_A0_BASE);
    //MAP_Timer_A_stopTimer(TIMER_A0_BASE);
    //MAP_Interrupt_disableInterrupt(INT_TA0_0);
    //MAP_Interrupt_unpendInterrupt(INT_TA0_0);

    ///pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    ///active_status = NVIC_GetActive(TA0_0_IRQn);
    ///counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    MAP_CS_setDCOFrequency(1000000);

    //int_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    //Set MCLK = SMCLK = DCO Frequency
    //CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    //COMPARE_VALUE++;

    //UART0_init();

    // Configure UART pin
    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
             GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Configure UART
    eUSCI_UART_Config param = {0};
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    param.clockPrescalar = 8;
    param.firstModReg = 0;
    param.secondModReg = 0xD6;
    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &param);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    /* Configuring Continuous Mode */
    Timer_A_ContinuousModeConfig continuousModeConfig = {0};
    continuousModeConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                         // SMCLK Clock Source
    continuousModeConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;              // SMCLK/1 = Default DCO = 3 MHz = MCLK
    continuousModeConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;      // Disable Overflow ISR
    continuousModeConfig.timerClear = TIMER_A_DO_CLEAR;                                   // Clear Counter
    MAP_Timer_A_configureContinuousMode(TIMER_A0_BASE, &continuousModeConfig);
    //MAP_Timer_A_stopTimer(TIMER_A0_BASE);
    //MAP_Timer_A_clearTimer(TIMER_A0_BASE);

    /* Initialize compare registers */
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_CompareModeConfig compareConfig = {0};
    compareConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;                    // Use CCR0
    compareConfig.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;       // Enable CCR interrupt
    compareConfig.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    compareConfig.compareValue = COMPARE_VALUE;
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

    //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    ///pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    ///active_status = NVIC_GetActive(TA0_0_IRQn);
    ///counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    //MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

    /* Enabling interrupts and NOT going to sleep */
    //MAP_Interrupt_enableSleepOnIsrExit();
    //MAP_Interrupt_disableInterrupt(INT_TA0_0);
    MAP_Interrupt_enableInterrupt(INT_TA0_0);

    /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    /* Starting the Timer_A0 in continuous mode */
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

    //int j;

    //for(j=10000;j>0;j--)
    //{
        //softwareIP1();
        //j = j--;
        //pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
        //active_status = NVIC_GetActive(TA0_0_IRQn);
        //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);
        //__asm(" nop");
    //}

    //while(1)
    //{
        //for(j=10000;j>0;j--);
        //softwareIP1();
    //}

    /*** Read IOP ***/
    __asm(" mov r3,#0x695a");
    __asm(" mov r1,#0x3040");
    __asm(" movt r1,#0xe004");
    __asm(" mov r2,#0x4FF9");
    __asm(" mov r14,#0x4025");
    __asm(" mov.w r11,#0x4031");
    __asm(" bx r11");

    /*** Write IOP ***
    __asm(" mov r0,#0x6969");
    __asm(" mov r1,#0x3040");
    __asm(" movt r1,#0xe004");
    __asm(" mov r2,#0x4001");
    __asm(" mov r3,#0x695a");
    __asm(" mov r14,#0x402d");
    __asm(" mov.w r11,#0x4031");
    __asm(" bx r11");***/

    while(1);
}

//******************************************************************************
//
//This is the TIMERA interrupt vector service routine.
//
//******************************************************************************
//volatile uint32_t int_status = 0;
void TA0_0_IRQHandler(void)
{
    ///pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    ///active_status = NVIC_GetActive(TA0_0_IRQn);
    ///counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);
    //MAP_Timer_A_stopTimer(TIMER_A0_BASE);
    __asm(" mov R11, #0x0000");
    __asm(" movt R11, #0x4000");
    __asm(" ldrh R10, [R11]");
    __asm(" bic R10, R10, #0x30");
    __asm(" strh R10, [R11]");
    //MAP_Timer_A_clearTimer(TIMER_A0_BASE);
    __asm(" mov R11, #0x0008");
    __asm(" movt R11, #0x4200");
    __asm(" mov R10, #0x0001");
    __asm(" strb R10, [R11]");
    //MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    __asm(" mov R11, #0x0000");
    __asm(" movt R11, #0x4200");
    __asm(" mov R10, #0x0000");
    __asm(" strb R10, [R11]");
    //MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    __asm(" mov R11, #0x0040");
    __asm(" movt R11, #0x4200");
    __asm(" mov R10, #0x0000");
    __asm(" strb R10, [R11]");

    //__asm(" mov R11, #0x100");
    //__asm(" cmp R11, #0");
    //__asm(" ble #0x8");
    //__asm(" sub R11, R11, #1");
    //__asm(" b #-0x10");
    //__asm(" ldrh R11, [R11]");
    //__asm(" lsrs R11, R11, #2");
    //__asm(" blo #-0x14");
    //UART0_puts("inside");

    __asm(" mov R11, #0x100E");
    __asm(" movt R11, #0x4000");

    //__asm(" mov R10, #0x5000");
    //__asm(" movt R10, #0x2000");
    //__asm(" str R0, [R10]");
    //__asm(" ldr R0, [R10]");

    __asm(" strh R0, [R11]");

    __asm(" mov R11, #0x101C");
    __asm(" movt R11, #0x4000");
    __asm(" ldrh R11, [R11]");
    __asm(" and R11, R11, #2");
    __asm(" cmp R11, #2");
    __asm(" bne #-0x18");

    //__asm(" mov R11, #0x100");
    //__asm(" loop_start: cmp R11, #0");
    //__asm(" ble loop_end");
    //__asm(" sub R11, R11, #1");
    //__asm(" b loop_start");
    //__asm(" mov R11, #0x100");
    //__asm(" cmp R11, #0");
    //__asm(" ble #0x8");
    //__asm(" sub R11, R11, #1");
    //__asm(" b #-0x10");

    __asm(" mov R11, #0x100E");
    __asm(" movt R11, #0x4000");

    //__asm(" mov R10, #0x5000");
    //__asm(" movt R10, #0x2000");
    //__asm(" str R0, [R10]");
    //__asm(" ldr R0, [R10]");

    __asm(" asr R0, R0, #8");
    __asm(" strh R0, [R11]");

    __asm(" mov R11, #0x101C");
    __asm(" movt R11, #0x4000");
    __asm(" ldrh R11, [R11]");
    __asm(" and R11, R11, #2");
    __asm(" cmp R11, #2");
    __asm(" bne #-0x18");

    //__asm(" mrs r0, PRIMASK");
    //__asm(" cpsid i");
    ///pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    ///active_status = NVIC_GetActive(TA0_0_IRQn);
    ///counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    ///__asm(" mov R13, #0xFFB0");
    ///__asm(" movt R13, #0x2000");
    ///__asm(" nop");

    ///__asm(" mov R11, #0xFF60");
    ///__asm(" movt R11, #0x2000");
    ///__asm(" mov R10, #0x2632");
    ///__asm(" strh R10, [R11]");
    ///__asm(" mov R11, #0xFF66");
    ///__asm(" movt R11, #0x2000");
    ///__asm(" mov R10, #0x6100");
    ///__asm(" strh R10, [R11]");
    ///__asm(" mov R11, #0xFF5C");
    ///__asm(" movt R11, #0x2000");
    ///__asm(" mov R10, #0x2BF1");
    ///__asm(" strh R10, [R11]");

    /*Routine to clear all bits starting from SP to SP+1F*/

    __asm(" mov R9, #0x20");
    __asm(" cmp R9, #0");
    __asm(" ble #0x14");

    __asm(" add R11, R13, R9");
    //__asm(" movt R11, #0x2000");
    __asm(" mov R10, #0x0000");
    __asm(" strh R10, [R11]");

    __asm(" sub R9, R9, #2");
    __asm(" b #-0x1c");

    __asm(" add R11, R13, #0x00");
    //__asm(" movt R11, #0x2000");
    __asm(" mov R10, #0x0000");
    __asm(" strh R10, [R11]");

    /*Routine ends*/


    __asm(" add R11, R13, #0x18");
    //__asm(" mov R11, #0xFF58");
    //__asm(" movt R11, #0x2000");
    __asm(" mov R10, #0x2632");
    __asm(" strh R10, [R11]");
    __asm(" add R11, R13, #0x1e");
    //__asm(" mov R11, #0xFF5e");
    //__asm(" movt R11, #0x2000");
    __asm(" mov R10, #0x6100");
    __asm(" strh R10, [R11]");
    __asm(" add R11, R13, #0x14");
    //__asm(" mov R11, #0xFF54");
    //__asm(" movt R11, #0x2000");
    __asm(" mov R10, #0x2C11");
    __asm(" strh R10, [R11]");

    ///__asm(" mov R13, #0xFFB0");
    ///__asm(" movt R13, #0x2000");
    ///__asm(" nop");
    ///__asm(" mov R14, #0x2cb1");
    ///__asm(" movt R14, #0x0000");
    ///__asm(" nop");
    ///__asm(" mov R10, #0x0000");
    ///__asm(" movt R10, #0x6100");
    ///__asm(" msr APSR_nzcvq, R10");
    ///__asm(" nop");
    //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);
    ///pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    ///active_status = NVIC_GetActive(TA0_0_IRQn);
    ///counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);
    //MAP_Timer_A_stopTimer(TIMER_A0_BASE);
    ///__asm(" mov R11, #0x0000");
    ///__asm(" movt R11, #0x4000");
    ///__asm(" ldrh R10, [R11]");
    ///__asm(" bic R10, R10, #0x30");
    ///__asm(" strh R10, [R11]");
    //MAP_Timer_A_clearTimer(TIMER_A0_BASE);
    ///__asm(" mov R11, #0x0008");
    ///__asm(" movt R11, #0x4200");
    ///__asm(" mov R10, #0x0001");
    ///__asm(" strb R10, [R11]");
    //MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    ///__asm(" mov R11, #0x0000");
    ///__asm(" movt R11, #0x4200");
    ///__asm(" mov R10, #0x0000");
    ///__asm(" strb R10, [R11]");
    //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);
    //int_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    //MAP_Interrupt_disableInterrupt(INT_TA0_0);
    //__asm(" mov R11, #0xE180");
    //__asm(" movt R11, #0xE000");
    //__asm(" mov R10, #0x0100");
    //__asm(" str R10, [R11]");
    //MAP_Interrupt_unpendInterrupt(INT_TA0_0);
    //__asm(" mov R11, #0xE280");
    //__asm(" movt R11, #0xE000");
    //__asm(" mov R10, #0x0100");
    //__asm(" str R10, [R11]");

    //pend_status = MAP_Timer_A_getInterruptStatus(TIMER_A0_BASE);
    //active_status = NVIC_GetActive(TA0_0_IRQn);
    //counter_val = MAP_Timer_A_getCounterValue(TIMER_A0_BASE);

    __asm(" nop");
    //__asm(" b #-0x1b4");   //jump to start of main after epilogue
    __asm(" b #-4");         //for call site verification test
}

