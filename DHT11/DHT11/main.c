
#define F_CPU 16000000L
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

#define DHT 6
#define DHT_DDR DDRD
#define DHT_PORT PORTD
#define DHT_PIN PIND

#define ROTATION_DELAY   1000
#define PULSE_MIN   1000
#define PULSE_MID   3000
#define PULSE_MAX   6000


void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1<<RXCIE0);
} // 8bit, no parity, 1 stop bit, TX enable, RX ISR enable

void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	// wait for sending
	UDR0 = data; // send
}

void uart_string(char* str)
{
	while (*str)
	uart_write(*str++);
}

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	uart_write(buf - 'a' + 'A');
}

void Request()
{
	DHT_DDR |= (1<<DHT);
	DHT_PORT &= ~(1<<DHT);
	_delay_ms(20);
	DHT_PORT |= (1<<DHT);
	_delay_us(40);
	
	DHT_DDR &= ~(1<<DHT);
	while(DHT_PIN & (1<<DHT));
	while((DHT_PIN & (1<<DHT))==0);
	while(DHT_PIN & (1<<DHT));
}

void InitializeTimer1(void)
{
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

	TCCR1A |= (1 << COM1A1);

	TCCR1B |= (1 << CS11);

	ICR1 = 39999;
}

uint8_t Receive_data()
{
	uint8_t c = 0;
	for (int i = 0; i < 8; i++)
	{
		while((DHT_PIN & (1<<DHT)) == 0);     /* check received bit 0 or 1 */
		_delay_us(30);
		if(DHT_PIN & (1<<DHT))             /* if high pulse is greater than 30ms */
		{
			c = (c<<1)|(0x01);               /* then its logic HIGH */
		}
		else                           /* otherwise its logic LOW */
		{
			c = (c<<1);
		}

		while(DHT_PIN & (1<<DHT));
	}
	return c;
}

int main(void)
{
	uint16_t I_Humi, D_Humi, I_Temp, D_Temp, check;
	DDRA = 0xFF;
	DDRC = 0xFF;
	uart_init(BAUDRATE(9600));
	LCD_Init();
	sei();
	LCD_wString("hello");
	

	while (1)
	{
		
		Request();             // 데이터 전송 요청
		I_Humi = Receive_data();      // 습도 상위 8비트 전송
		D_Humi = Receive_data();      // 습도 하위 8비트 전송
		I_Temp = Receive_data();      // 온도 상위 8비트 전송
		D_Temp = Receive_data();      // 온도 하위 8비트 전송
		check = Receive_data();   // CheckSum 비트 = 습도 16비트 + 온도 16비트
	
		
		if ((I_Humi + D_Humi + I_Temp + D_Temp) != check)
		{
			LCD_wString("Error");
		}
		char str[16] = {0,};
		LCD_wBCommand(0x80|0x00);
		LCD_wString("Temp: ");
		LCD_wString(itoa(I_Temp, str, 10));
		LCD_wString(".");
		LCD_wString(itoa(D_Temp, str, 10));
		LCD_wData(0b11011111);
		LCD_wData('C');
		LCD_wBCommand(0x80|0x40);
		LCD_wString("Humi: ");
		LCD_wString(itoa(I_Humi, str, 10));
		LCD_wString(".");
		LCD_wString(itoa(D_Humi, str, 10));
		LCD_wString(" %");
		sprintf(str, "%3d%3d", I_Temp, I_Humi);
		uart_string(str);

		
		if(I_Temp>26)
		{    DDRB |= (1 << PB5);

			InitializeTimer1();

			while(1)
			{
				OCR1A = PULSE_MIN;
				_delay_ms(ROTATION_DELAY);

				OCR1A = PULSE_MID;
				_delay_ms(ROTATION_DELAY);

				OCR1A = PULSE_MAX;
				_delay_ms(ROTATION_DELAY);
				
			}

		}
		
		if(I_Temp==26)
		
		{
			LCD_wBCommand(0x80|0x00);
			LCD_wString("temp proper  ");
			_delay_ms(5000);
		}
		
		
		
		if(I_Humi<=50)
		
		{
			LCD_wBCommand(0x80|0x40);
			LCD_wString("humi proper   ");
		_delay_ms(5000);
		}

		
		

		
		
		
		_delay_ms(3000);
	}
}
