#include "Buzzer.h"
#include "LCD.h"
#include "LED.h"
#include "Ultrasonic.h"
#include <util/delay.h>

void alarm(void);


void main(void){
	Buzzer_init();
	LCD_init();
	LEDS_init();
	Ultrasonic_init();

	uint8 distance;

	while(1){
		distance = Ultrasonic_readDistance();

		LCD_moveCursor(0,2);
		LCD_displayString("Distance=");
		LCD_integerToString(Ultrasonic_readDistance());
		LCD_displayString(" cm ");

		if (distance <= 5){
			alarm();
		}
		else if ((distance >= 6) && (distance <= 10)){
			LED_on(red);
			LED_on(green);
			LED_on(blue);
			//clear the stop message without clearing the whole screen to avoid screen flickering
			LCD_moveCursor(1,6);
			LCD_displayString("    ");
		}
		else if (distance >= 11 && distance <= 15){
			LED_on(red);
			LED_on(green);
			LED_off(blue);
			LCD_moveCursor(1,6);
			LCD_displayString("    ");
		}
		else if (distance >= 16 && distance <= 20){
			LED_on(red);
			LED_off(green);
			LED_off(blue);
			LCD_moveCursor(1,6);
			LCD_displayString("    ");
		}
		else {
			LED_off(red);
			LED_off(green);
			LED_off(blue);
			LCD_moveCursor(1,6);
			LCD_displayString("    ");
		}
	}
}

void alarm(void){
	LED_on(red);
	LED_on(green);
	LED_on(blue);
	Buzzer_on();
	LCD_moveCursor(1,6);
	LCD_displayString("STOP");
	_delay_ms(250);
	LED_off(red);
	LED_off(green);
	LED_off(blue);
	Buzzer_off();
	_delay_ms(150);
}
