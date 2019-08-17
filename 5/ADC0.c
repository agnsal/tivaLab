#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

int main(void)
{
    uint32_t ui32ADC0Value[4]; // Create an array for storing the data read from the ADC FIFO (sequencer 1 has a FIFO depth of 4).

    volatile uint32_t ui32TempAvg; // For storing the average of the temperature. Volatile variables are available at runtime.
    volatile uint32_t ui32TempValueC; // For storing Celsius temperature value.
    volatile uint32_t ui32TempValueF; // For storing Fahrenheit temperature value.

    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); // Set up the system clock to run at 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // Enable the ADC0 peripheral.

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0); // Use ADC0, sample sequencer 1, with processor triggering the sequence, and use the highest priority (0).

    /*
     * In the following 3 lines:
     * Configure steps 0 - 2 on sequencer 1 to sample the temperature sensor (ADC_CTL_TS).
     * Our code will average all four samples of temperature sensor data on sequencer 1 to calculate the temperature,
     * so all four sequencer steps will measure the temperature sensor.
    */
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);

    /*
     * In the following line, the final sequencer step requires a couple of extra settings.
     * Sample the temperature sensor (ADC_CTL_TS) and configure the interrupt flag (ADC_CTL_IE) to be set when the sample is done.
     * Tell the ADC logic that this is the last conversion on sequencer 1 (ADC_CTL_END).
    */
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 1); // Enable ADC sequencer 1.

    while(1)
    {
        ADCIntClear(ADC0_BASE, 1); // Make sure that the flag is cleared before writing code that depends on it.
        ADCProcessorTrigger(ADC0_BASE, 1); // Trigger the ADC conversion with software.
        while(!ADCIntStatus(ADC0_BASE, 1, false)) // Wait for the conversion to complete. A better way to do this would be to use an interrupt.
        {
        } // Now data from temperature sensor are available in ADC0.
        ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value); // Read the ADC value from the ADC Sample Sequencer 1 FIFO and put it into ui32ADC0Value.

        ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2) / 4; // The addition of 2 is for rounding.

        /*
         * In the following line:
         * Division is performed last to avoid truncation due to integer math rules (a later lab will cover floating point operations).
         * TEMP = 147.5 – ((75 * (VREFP – VREFN) * ADCVALUE) / 4096)
         * We need to multiply everything by 10 to stay within the precision needed. The divide by 10 at the end is needed to get the right answer.
         * VREFP – VREFN is Vdd or 3.3 volts. We’ll multiply it by 10, and then 75 to get 2475.
        */
        ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;

        /*
         * In the following line:
         * The conversion from Celsius to Fahrenheit is F = ( C * 9)/5 +32.
         * Adjusting that a little gives: F = ((C * 9) + 160) / 5.
        */
        ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
    }
}
