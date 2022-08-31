/*
 * LCD.c
 *
 * Created: 2022-07-28 오후 4:38:10
 * Author : myj04
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

int main(void)
{
	DDRC = 0xFF;
	DDRA = 0xFF;

	LCD_Init();
	LCD_wBCommand(0x80 | 0x00);
	LCD_wString("MOON");
	LCD_wBCommand(0x80 | 0x40);
	LCD_wString("Hello!");
	while (1)
	{
		
	}
	return 0;
}
