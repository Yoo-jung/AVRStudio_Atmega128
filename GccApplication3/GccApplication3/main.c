
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define PRESCALER 1024​

void Timer_init(void)
{
	TCCR1B |= (1<<CS12) | (1<<CS10);
}

uint8_t masure_distance(void)
{
	PORTA &= ~(1<<PA0);
	_delay_us(10);
	PORTA &= (1<<PA0);
	_delay_us(10);
	PORTA &= ~(1<<PA0);
	
	TCNT1 = 0;
	while (!(PINA & 0x02))
	{
		if(TCNT1>65000)	return 0;
	}
	TCNT1 =0;
	while (PINA & 0x02)
	{
		if(TCNT1>65000){
			TCNT1 = 0;
			break;
		}
	}
	
	double pulse_width = 1000000.0 * TCNT1 * PRESCALER / F_CPU;
	return pulse_width / 58;
}

int main(void)
{
	uint8_t distance;
	
	DDRB = 0xFF;
	DDRA |= (1 << PA0);
	DDRA &= ~(1 << PA1);
	Timer_init();
	
	while (1)
	{
		distance =masure_distance();
		
		if(distance > 40) PORTB = 0b00000000;
		if(distance <= 40 && distance >35) PORTB = 0b00000001;
		if(distance <= 35 && distance >30) PORTB = 0b00000011;
		if(distance <= 30 && distance >25) PORTB = 0b00000111;
		if(distance <= 25 && distance >20) PORTB = 0b00001111;
		if(distance <= 20 && distance >15) PORTB = 0b00011111;
		if(distance <= 15 && distance >10) PORTB = 0b00111111;
		if(distance <= 10 && distance >5) PORTB = 0b01111111;
		if(distance <= 5) PORTB = 0b11111111;
		
		_delay_ms(100);
	}
	
	return 0;
}