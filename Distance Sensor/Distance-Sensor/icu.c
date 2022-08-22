/************************************************************************************
 *
 * Module: ICU
 *
 * File Name: icu.c
 *
 * Description: Source file for the ICU driver
 *
 * Author: Omar Hossam ElDin
 *
 ***********************************************************************************/

#include "icu.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */
#include "gpio.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void Icu_init(const Icu_ConfigType * Config_Ptr){

	/*Configure PD6 as an input pin*/
	GPIO_setupPinDirection(PORTD_ID,PIN6_ID,PIN_INPUT);

	/* Timer1 always operates in Normal Mode */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);

	/*Configure the clock from the config-type structure in the pins cs12,cs11 and cs10*/
	TCCR1B = (TCCR1B&0xF8) | (Config_Ptr->clock);

	/*Configure the required edge type in pin ICES1*/
	TCCR1B = (TCCR1B&0xBF) | ((Config_Ptr->edge)<<6);

	/* Initial Value for Timer1 */
	TCNT1 = 0;

	/* Initial Value for the input capture register */
	ICR1 = 0;

	/*Configure the input capture interrupt enable by setting the pin TICIE1 in TIMSK register*/
	SET_BIT(TIMSK,TICIE1);

}

/*
 * Description: Function to set the Call Back function address.
 */
void Icu_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/*
 * Description: Function to set the required edge detection.
 */
void Icu_setEdgeDetectionType(const Icu_EdgeType a_edgeType)
{
	/*Configure the required edge type in pin ICES1*/
	TCCR1B = (TCCR1B&0xBF) | (a_edgeType<<6);
}

/*
 * Description: Function to get the Timer1 Value when the input is captured
 *              The value stored at Input Capture Register ICR1
 */
uint16 Icu_getInputCaptureValue(void)
{
	return ICR1;
}

/*
 * Description: Function to clear the Timer1 Value to start count from ZERO
 */
void Icu_clearTimerValue(void)
{
	TCNT1 = 0;
}

/*
 * Description: Function to disable the Timer1 to stop the ICU Driver
 */
void Icu_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	ICR1 = 0;

	/* Disable the Input Capture interrupt */
	CLEAR_BIT(TIMSK,TICIE1);
}