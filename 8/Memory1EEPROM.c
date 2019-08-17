#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/eeprom.h"

int main(void)
{
    uint32_t pui32Data[2];
    uint32_t pui32Read[2];
    pui32Data[0] = 0x12345678;
    pui32Data[1] = 0x56789abc;

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // 40 MHz clock.

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
    SysCtlDelay(20000000);

    FlashErase(0x10000); // Erase the block of flash we identified earlier.
    FlashProgram(pui32Data, 0x10000, sizeof(pui32Data)); // Program the data array we created, to the start of the block, of the length of the array.

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02);
    SysCtlDelay(20000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // Turn on the EEPROM peripheral.
    EEPROMInit(); // Perform a recovery if power failed during a previous write operation.
    EEPROMMassErase(); // Erase the entire EEPROM. This isn’t necessary because (unlike flash) EEPROM does not need to be erased before it is programmed.
    EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); // Read the erased values into pulRead (offset address).
    EEPROMProgram(pui32Read, 0x0, sizeof(pui32Read)); // Program the data array, to the beginning of EEPROM, of the length of the array.
    EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); // Read that data into array pulRead.
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x04);

    while(1)
    {
    }
}
