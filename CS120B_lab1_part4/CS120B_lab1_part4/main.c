/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 2  Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as outputs, initialize to 0s
	DDRD = 0xFF; PORTD = 0x00;	//output
	unsigned short tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned short tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned short tmpC = 0x00; 
	unsigned short tmpT = 0x00;
	unsigned short total = 0x00;
	
    while(1)
    {
		tmpA = PINA; //sets tmpA to bit position 0, 1, 2, 3
		tmpB = PINB;
		tmpC = PINC;
		tmpT = PIND & 0xFE;
	 
		if ((tmpA + tmpB + tmpC) > 0x8C) {
			PORTD = tmpT | 0x01;
		}
		
		if (tmpA - tmpC > 0x50) {
			PORTD = (PIND & 0xFD) | 0x02;
		}
		
		total = tmpA + tmpB + tmpC;
		if (total > 0x3F) {
			total = 0xFC;
			PORTD = total | (tmpT | 0x01) | ((PIND & 0xFD) | 0x02);
		}
		else {
			total = total << 2;
			total = total | (tmpT | 0x01) | ((PIND & 0xFD) | 0x02);
			PORTD = total;
		}
		
	
    }
	return 0;
}

