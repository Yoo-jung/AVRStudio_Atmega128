/*
 * GccApplication1.c
 *
 * Created: 2022-04-11 오후 6:26:42
 * Author : myj04
 */ 

#define F_CPU 16000000L
#include <avr/io.h>

int main(void)
{
	DDRA = 0xFF; // 0b11111111
	DDRD = 0x00; // 0b00000000
	while (1)
	{
		PORTA = PIND;
	}
}


