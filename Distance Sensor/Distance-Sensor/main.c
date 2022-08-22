/************************************************************************************
 *
 * File Name: main.c
 *
 * Description: main file
 *
 * Author: Omar Hossam ElDin
 *
 ***********************************************************************************/

#include "lcd.h"
#include "icu.h"
#include "ultrasonic.h"
#include <avr/io.h>

int main(void)
{

	uint16 distance;
	/* Initializing the LCD and Ultra-sonic */
	LCD_init();
	Ultrasonic_init();
	SREG |= (1<<7);
	while(1)
	{
		distance = Ultrasonic_readDistance();


		//LCD_moveCursor(0,10);
		if(distance<10)
		{LCD_displayStringRowColumn(0,0,"Distance= ");
			LCD_intgerToString(distance);
			LCD_displayString("  ");
			LCD_displayStringRowColumn(0,14,"cm");
		}
		else if(distance<100)
		{LCD_displayStringRowColumn(0,0,"Distance= ");
			LCD_intgerToString(distance);
			LCD_displayString(" ");
			LCD_displayStringRowColumn(0,14,"cm");
		}
		else
		{LCD_displayStringRowColumn(0,0,"Distance= ");
			LCD_intgerToString(distance);
			LCD_displayStringRowColumn(0,14,"cm");
		}


	}

}
