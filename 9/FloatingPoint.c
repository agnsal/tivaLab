#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SERIES_LENGTH 100 // The depth of our data buffer.

float gSeriesData[SERIES_LENGTH];
int32_t i32DataCount = 0; // The counter for our computation loop.

int main(void)
{
    float fRadians; // Sine function.

    ROM_FPULazyStackingEnable(); // Turn on Lazy Stacking.
    ROM_FPUEnable(); // Turn on the FPU.

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); // Set up the system clock for 50MHz.

    fRadians = ((2 * M_PI) / SERIES_LENGTH); // A full sine wave cycle is 2pi radians. Divide 2pi by the depth of the array.

    while(i32DataCount < SERIES_LENGTH) // Calculate the sine value for each of the 100 values of the angle and place them in our data array.
    {
        gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount);
        i32DataCount ++;
    }

    while(1){
    }
}
