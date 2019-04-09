/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 3  Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFE; PORTB = 0x01;
	unsigned short tmpD = 0x00;
	unsigned short tmpB = 0x00;
	
    while (1) 
    {
		tmpD = PIND;
		tmpB = PINB & 0x01;
		
		if (tmpB == 1) {
			tmpD = tmpD << 1;
			tmpD = tmpD + 1; 
		}
		if (tmpD >= 70) {
			tmpB = 0x02;
		}
		else if (tmpD > 5 && tmpD < 70) {
			tmpB = 0x04;
		}
		PORTB = tmpB;
    }
	
	return 1;
}

