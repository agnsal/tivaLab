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

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Configure the system clock to 40MHz.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // LEDS: 2=red=pin1, 4=blue=pin2 and 8=green=pin3.
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0X08); // 8 = 00001000 -> only pin3 (green) on.

    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE); // Enable the hibernation module.
    HibernateEnableExpClk(SysCtlClockGet()); // Define the clock supplied to the hibernation module.
    HibernateGPIORetentionEnable(); // Enable the GPIO pin state to be maintained during hibernation and remain active even when waking from hibernation.
    SysCtlDelay(64000000); // Delay 4 seconds for you to observe the LED
    HibernateWakeSet(HIBERNATE_WAKE_PIN); // Set the wake condition to the wake pin.
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); // Turn off the green LED before the device goes to sleep.

    HibernateRequest();
    while(1)
    {
    }
}