#include "LED.h"
#include "GPIO.h"
#include "common_macros.h"

void LEDS_init(void){
	GPIO_setupPinDirection(LED_PORT_ID, red, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_ID, green, PIN_OUTPUT);
	GPIO_setupPinDirection(LED_PORT_ID, blue, PIN_OUTPUT);

#if (connection == positive_logic)
	GPIO_writePin(LED_PORT_ID, red, 0);
	GPIO_writePin(LED_PORT_ID, green, 0);
	GPIO_writePin(LED_PORT_ID, blue, 0);
#elif (connection == negative_logic)
	GPIO_writePin(LED_PORT_ID, red, 1);
	GPIO_writePin(LED_PORT_ID, green, 1);
	GPIO_writePin(LED_PORT_ID, blue, 1);
#endif
}

void LED_on(LED_ID id){
#if (connection == positive_logic)
	GPIO_writePin(LED_PORT_ID, id, 1);
#elif (connection == negative_logic)
	GPIO_writePin(LED_PORT_ID, id, 0);
#endif
}

void LED_off(LED_ID id){
#if (connection == positive_logic)
	GPIO_writePin(LED_PORT_ID, id, 0);
#elif (connection == negative_logic)
	GPIO_writePin(LED_PORT_ID, id, 1);
#endif
}

