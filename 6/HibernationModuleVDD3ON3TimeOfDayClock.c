/*
Copyright 2019 Agnese Salutari.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on
an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and limitations under the License
*/

#include <stdint.h>
#include <stdbool.h>
#include "utils/ustdlib.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/hibernate.h"
#include "driverlib/gpio.h"

uint8_t ui8HourMin[2] = {0, 0}; // Array storing hours and minutes

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Configure the system clock to 40MHz.

    // The following 2 lines are for debugging. Comment them to safe power!
    // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // Pin3 is for minutes, pin2 for hours and pin1 for days.

    ui8HourMin[0] += 1;
    // The following 3 lines are for debugging. Comment them to safe power!
    // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02); // Only pin3 on.
    // SysCtlDelay(16000000);
    // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); // All pins off.
    if(ui8HourMin[0] > 59)
    {
        ui8HourMin[0] = 0;
        ui8HourMin[1] += 1;
        // The following 3 lines are for debugging. Comment them to safe power!
        // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x04); // Only pin2 on.
        // SysCtlDelay(16000000); // Delay 1 sec to see LED on (ui8HibernationTime = 59 for this reason).
        // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); // All pins off.
        if(ui8HourMin[1] > 23)
        {
            ui8HourMin[1] = 0;
            // The following 3 lines are for debugging. Comment them to safe power!
            // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08); // Only pin1 on.
            // SysCtlDelay(16000000); // Delay 1 sec to see LED on (ui8HibernationTime = 59 for this reason).
            // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); // All pins off.
        }
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE); // Enable the hibernation module.
    HibernateEnableExpClk(SysCtlClockGet()); // Define the clock supplied to the hibernation module.
    HibernateGPIORetentionEnable(); // Enable the GPIO pin state to be maintained during hibernation and remain active even when waking from hibernation.
    HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC); // Set the wake condition to the wake pin or RTC (Real Time Clock).
    HibernateRTCSet(0); // Reset the RTC to 0.
    HibernateRTCEnable(); // Turn the RTC.
    HibernateRTCMatchSet(0, 60); // Set the wake up time for 60 seconds in the future.
    HibernateRequest();
    while(1)
    {
    }
}