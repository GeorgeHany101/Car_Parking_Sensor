#include "Ultrasonic.h"
#include "ICU.h"
#include "GPIO.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static uint8 g_edgeCount = 0;       /* Counter for edge detection */
static uint16 g_timeHigh = 0;       /* Time duration of the high pulse */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description: Initialize the ICU driver and configure the trigger pin as output */
void Ultrasonic_init(void)
{
    /* Configure the trigger pin (e.g., PB5) as output */
    GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, PIN_OUTPUT);    /* Configure the trigger pin (e.g., PB5) as output */
    GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, PIN_OUTPUT);

    /* Initialize ICU with F_CPU/8 prescaler and rising edge detection */
    ICU_ConfigType icuConfig = {F_CPU_8, RAISING};
    ICU_init(&icuConfig);

    /* Set the ICU callback function */
    ICU_setCallBack(Ultrasonic_edgeProcessing);
}

/* Description: Send a trigger pulse to the ultrasonic sensor */
void Ultrasonic_Trigger(void)
{
    /* Send a 10us trigger pulse to the HC-SR04 */
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, LOGIC_HIGH); /* Set trigger pin HIGH */
    _delay_us(10);                               /* Wait for 10 microseconds */
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT_ID, ULTRASONIC_TRIGGER_PIN_ID, LOGIC_LOW); /* Set trigger pin LOW */
}

/* Description: Read the distance measured by the ultrasonic sensor */
uint16 Ultrasonic_readDistance(void)
{
    /* Send a trigger pulse to start the measurement */
    Ultrasonic_Trigger();

    /* Wait for the ICU to measure the pulse width */
    while (g_edgeCount != 2);

    /* Reset edge count for the next measurement */
    g_edgeCount = 0;

    /* Calculate the distance in centimeters */
    uint16 distance = ((g_timeHigh / 117.6)+1); /* Formula: Distance = Timer Value / 117.6 */
    return distance;
}

/* Description: Callback function to process edges detected by the ICU */
void Ultrasonic_edgeProcessing(void)
{
    g_edgeCount++;
    if (g_edgeCount == 1)
    {
        /* First edge detected (rising edge), start the timer */
        ICU_clearTimerValue();
        ICU_setEdgeDetectionType(FALLING); /* Switch to detect falling edge */
    }
    else if (g_edgeCount == 2)
    {
        /* Second edge detected (falling edge), calculate the pulse width */
        g_timeHigh = ICU_getInputCaptureValue();
        ICU_setEdgeDetectionType(RAISING); /* Reset to detect rising edge */
    }
}
