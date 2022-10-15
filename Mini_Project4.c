/******************************************************************************
 *
 * Name: Mini_Project4.c.c
 *
 * Description: Design a system to measure the distance using ultrasonic sensor\
 * HC-SR04 using ATmega32 microcontroller
 *
 * Author: Nada Sayed
 *
 * Date: 14/10/2022
 *******************************************************************************/

#include "lcd.h"
#include "icu.h"
#include "ultrasonic.h"
#include <avr/io.h> /* To use the SREG register */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
extern uint8 g_edgeCount;
extern uint16 g_timeHigh;

int main(){
	/* Variable to hold the distance measured by the ultrasonic */
	uint16 measuredDistance;

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Initialize both the LCD and ULTRASONIC driver */
	LCD_init();
	Ultrasonic_init();

	/* Triggering the ultrasonic to get its first measurement */
	Ultrasonic_Trigger();

	while(1){
		LCD_moveCursor(0,0);
		LCD_displayString("Distance= ");

		if(g_edgeCount == 2)
		{
			/* Store the High time value */
			g_timeHigh = Icu_getInputCaptureValue();

			/* Clearing the number edges to measure again */
			g_edgeCount=0;
			/* Clear the timer counter register to start measurements again */
			Icu_clearTimerValue();
			/* Getting the measured distance in cms */
			measuredDistance= Ultrasonic_readDistance();

			if(measuredDistance>=100){
				/* display the measuredDistance on LCD screen */
				LCD_intgerToString(measuredDistance);
			}
			else{
				/* display the measuredDistance on LCD screen */
				LCD_intgerToString(measuredDistance);
				LCD_displayCharacter(' ');
			}
			LCD_moveCursor(0,13);
			LCD_displayString("cm");
		}

	}
}
