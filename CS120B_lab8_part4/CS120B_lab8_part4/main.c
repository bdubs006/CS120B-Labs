/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 8 Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

unsigned short MAX = 0x0C7;
unsigned short ADCCop = 0x00;
unsigned short tempVal = 0x00;

void ADC_Init(){
	ADCSRA |= (1<< ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	tempVal = MAX / 8;
	ADC_Init();
	/* Replace with your application code */
	while (1)
	{
		ADCCop = ADC;
		if(ADCCop <= tempVal){
			PORTB = 0x01;
		}
		else if(ADCCop <= (2*tempVal)){
			PORTB = 0x03;
		}
		else if(ADCCop <= (3*tempVal)){
			PORTB = 0x07;
		}
		else if(ADCCop <= (4*tempVal)){
			PORTB = 0x0F;
		}
		else if(ADCCop <= (5*tempVal)){
			PORTB = 0x1F;
		}
		else if(ADCCop <= (6*tempVal)){
			PORTB = 0x3F;
		}
		else if(ADCCop <= (7*tempVal)){
			PORTB = 0x7F;
		}
		else{
			PORTB = 0xFF;
		}
		
	}
}


