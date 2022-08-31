/*
 * BIT.c
 *
 * Created: 2022-03-23 오후 7:49:52
 * Author : myj04
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF; // 0b11111111 //포트 B 출력모드로 설정
	PORTB = 0x55; // 0b01010101 //포트 B 6,4,2,0 번 ON
	while (1)
	{
		PORTB <<= 1; // 포트 B 왼쪽으로 1비트 이동
		_delay_ms(200); //0.2초 지연
		PORTB >>= 1; //포트 B 오른쪽으로 1비트 이동
		_delay_ms(200); //0.2초 지연
	}
}

