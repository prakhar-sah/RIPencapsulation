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
// Flash Mailbox example to secure IP
//
// This example programs IP into address 0x0000.4000 and then secures the
// IP using the flash mailbox. This demonstrates how an software IP developer
// can develop IP and then secure it into devices at a fixed address. The
// device can then be used by an application that needs to call the secure IP.
//
// After loading the program the following will happen:
//   1. After loading, the program will run. If you haven't reset the device
//      yet the IP will be unprotected and the RED LED will blink
//   2. Reset the device by doing a reboot reset (by setting SYS_RESET_REQ.REBOOT 
//      bit or pressing S1 on the LaunchPad). The boot code will run and secure
//      the IP. The application code detects that the IP has been secured and
//      erases the keys in the mailbox and then runs the IP. The Green LED will
//      blink showing that the secure IP is running.
//
// Look at the run_secure_ip example to show how this secure IP can be called
// from an application without having the source code of the IP.
//
//*****************************************************************************

#include <msp.h>
#include "flashmailbox.h"
#include "driverlib.h"

void softwareIP1(void);

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;               // Stop watchdog timer

    volatile uint32_t i;
    
    P1->DIR |= BIT0;
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
		
    if(CheckFlashMailbox(COMMAND_SEC_ZONE0_EN) !=
       BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS)
    {
        // Blink RED LED indicating security has failed
        while(1)
        {
            for(i = 0; i < 10000; i++);

            P1->OUT ^= BIT0;
			
		    // Do reeboot reset when S1 is pressed
            //if((P1->IN & BIT1)==0)
            //{
                SysCtl_rebootDevice();
            //}
        }
    }
    else
    {
        // Run Protected IP
        while(1)
        {
            for(i = 0; i < 10000; i++);

            softwareIP1();
        }
    }
}

//*****************************************************************************
//
//! \brief Secure IP that blinks pin P2.1
//!
//! This function toggles P2.1 and is to be used as a demonstration of a
//! protected IP code. The assembly instructions at the beginning of the
//! function enable data access within the IP protected zone. On the
//! MSP-EXP432P401R LaunchPad P2.1 is a Green LED.
//!
//! \return None
//
//*****************************************************************************
#if defined (__TI_ARM__)   /* TI CGT Compiler */
#pragma CODE_SECTION(softwareIP1,".sec_zone0")

#elif defined (__ICCARM__) /* IAR Compiler */
#pragma default_function_attributes = @ "SEC_ZONE_0"

#elif defined (__CC_ARM)   /* ARM Compiler */
void softwareIP1(void) __attribute__((section(".ARM.__at_0x00004000")));
#endif

void softwareIP1(void)
{
    UNLOCK_DEVICE;          // Write the unlock key to the SYSCTL_SECDATA_UNLOCK register

    P2->DIR |= BIT1;        // Configure P2.1 as output

    uint64_t i = 10000;
    //do i--;
     while(i != 0)
    {
        i--;
    }

    P2->OUT ^= BIT1;        // Toggle P2.1

    SYSCTL->SECDATA_UNLOCK = 0x0;
}

#if defined (__ICCARM__) /* IAR Compiler */
#pragma default_function_attributes =
#endif
