/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 3  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
    while (1) 
    {
		tmpB = PINA & 0xF0;
		tmpC = PINA & 0x0F;
		
		tmpB = tmpB >> 4;
		tmpC = tmpC << 4;
		
		PORTB = tmpB;
		PORTC = tmpC;
    }
	
	return 1;
}

