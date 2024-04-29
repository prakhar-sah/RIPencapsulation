//*****************************************************************************
//
// Copyright (C) 2012 - 2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
// This example simply erases the flash mailbox 
//
// Afterwards the code in flashmailbox.c erases the mailbox content.
// After successfully erasing the mailbox the application switches on
// the RED LED.
//
// This might be helpful should you have misconfigured the flash mailbox by
// writing a not valid command to it.
//
//*****************************************************************************

#include <msp.h>
#include "flashmailbox.h"

#include "driverlib.h"

#define MAX_FLASH_ERASE_CYCLES 50

uint32_t EraseFlashMailbox()
{
    uint32_t result = BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS;
    uint32_t eraseCycles = 0, eraseSuccess;

    eraseSuccess = 0;
    MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK0, FLASH_SECTOR0);

    while((eraseCycles++ < MAX_FLASH_ERASE_CYCLES) && (!eraseSuccess))
    {
        eraseSuccess = MAP_FlashCtl_eraseSector(FLASH_MAILBOX_START);
    }
    if(!eraseSuccess)
    {
        result = MAILBOX_ERASE_FAIL;
    }

    MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK0, FLASH_SECTOR0);

    return(result);
}


int main(void)
{
    volatile uint32_t i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;               // Stop watchdog timer
    P1->DIR |= BIT0;
    if(EraseFlashMailbox() !=
       BOOT_OVERRIDE_AND_MAILBOX_OPERATIONS_SUCCESS)
    {
        while(1)
        {
            for(i = 0; i < 10000; i++);

            P1->OUT ^= BIT0;
        }
    }
    else
    {
        P1->OUT |= BIT0;
    }

    while(1);
}
