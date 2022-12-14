/******************************************************************************
 *
 * Module: ULTRASONIC
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ATmega32 ULTRASONIC driver
 *
 * Author: Nada Sayed
 *
 *******************************************************************************/
#include"ultrasonic.h"
#include"std_types.h"
#include"icu.h"
#include"gpio.h"
#include<util/delay.h> /* To use delay functions */
#include <avr/io.h> /* To use ICU/Timer1 Registers */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_edgeCount = 0; /* Variable to count the number of edges detected */
uint16 g_timeHigh = 0; /* Variable to hold the high time of the Echo */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for:
 * 1. Initialize the ICU driver as required.
 * 2. Setup the ICU call back function.
 * 3. Setup the direction for the trigger pin as output pin through the GPIO driver.
 */
void Ultrasonic_init(void){

	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	/* Setup Trigger pin as output pin */
	GPIO_setupPinDirection(ULTRASONIC_TRIGEER_PORT,ULTRASONIC_TRIGEER_PIN,PIN_OUTPUT);

	/* Initialize the ICU driver */
	Icu_init(&Icu_Config);

	/* Set the Call back function pointer in the ICU driver */
	Icu_setCallBack(Ultrasonic_edgeProcessing);
}

/*
 * Description :
 * Function responsible for sending the Trigger pulse to the ultrasonic.
 */
void Ultrasonic_Trigger(void){

	/* Giving a trigger pulse to the ultrasonic */
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	/* Waiting at least 10 usec */
	_delay_us(15);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for:
 * 1. Send the trigger pulse by using Ultrasonic_Trigger function.
 * 2. Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void){

	uint16 distance;

	/* Giving a trigger pulse to the ultrasonic to get a new distance measure */
	Ultrasonic_Trigger();

	/* Calculating the distance in Centimeters */
	distance =(uint16)(((g_timeHigh) / 58.8)+ERROR_IN_READING);
	return distance;
}

/*
 * Description :
 * Function responsible for:
 * 1.This is the call back function called by the ICU driver.
 * 2. This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void){

	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}

