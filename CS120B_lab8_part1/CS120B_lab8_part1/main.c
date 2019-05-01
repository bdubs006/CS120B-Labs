/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 8  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void Convert_Init(){
	//ADEN is to enable the analog->digital converting
	//ADSC is to start the first converting
	//ADATE sets up auto triggering, for the potentiometer
	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE);
}

unsigned short my_short = 0xADC;

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	Convert_Init();
	
	/* Replace with your application code */
	while (1)
	{
		my_short = ADC;
		PORTB = (char)(my_short & 0x00FF);
		PORTD = (char)(my_short >> 8);

	}
}

