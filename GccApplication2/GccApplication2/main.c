/*
 * Kim Seong Won_1.c
 *
 * Created: 2022-04-05 오후 6:42:08
 * Author : myj04
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
   DDRD=0x00;
   DDRE=0xFF;
 //  PORTE=0b01010101;
   
   while(1){
	   PORTE = PIND;
	   z
	   PORTE <<=1;
	   _delay_ms(200);
	   PORTE >>=1;
	   _delay_ms(200);
	   */
   }   
   return 0;
}

