/*
 * PIN.c
 *
 * Created: 2022-03-23 오후 7:52:44
 * Author : myj04
 */ 

// 버튼 누르면 누르는 포트 값 켜지는 거 

/*
 * GccApplication1.c
 *
 * Created: 2022-03-23 오후 6:45:17
 * Author : hwang
 */ 

#define F_CPU 16000000L
#include <avr/io.h>

int main(void)
{
   DDRC = 0x00;
   DDRD = 0xFF;
   
   PORTB = 0x00;
   
    /* Replace with your application code */
    while (1) 
    {
      //PORTB = PINC;
	  PORTD=0xFF;
    }
}

