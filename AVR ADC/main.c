#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#ifndef F_CPU 
#define F_CPU 8000000
#endif
#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

int Uart_PutChar (char c, FILE *stream);
FILE uart_str = FDEV_SETUP_STREAM(Uart_PutChar, NULL, _FDEV_SETUP_WRITE);

void USART_Init(void)
{	               
UBRRL=UBRRVAL;		
UBRRH=(UBRRVAL>>8);	
UCSRC|=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);			
UCSRB|=(1<<TXEN);
}

int Uart_PutChar (char c, FILE *stream)
 { 
    if (c == '\n')
    Uart_PutChar('\r', stream);
    loop_until_bit_is_set(UCSRA,UDRE); 
    UDR = c; 
    return 0; 
 }

void adcinit(void)
{
   ADMUX|=(0<<REFS1)|(1<<REFS0);
   ADCSRA|=(1<<ADEN)|(1<<ADIE);
}

void adcstartconv(void)
{
   sei();
   ADCSRA|=(1<<ADSC)|(1<<ADIE);
}

ISR(ADC_vect)
{
   uint16_t adc_value;
   USART_Init();
   stdout=&uart_str;
   adc_value = ADCL;
   adc_value += (ADCH<<8);
   printf("%d\n",adc_value);
}

int main(void)
{
    while(1)
	{
	adcinit();
   _delay_ms(10);
   adcstartconv();
   _delay_ms(10);
    }
	return 0;
}
