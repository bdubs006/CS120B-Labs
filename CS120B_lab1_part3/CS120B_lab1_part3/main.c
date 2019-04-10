/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 2  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char cntavail = 0x00;
	
	while(1)
	{
	tmpA = PINA & 0x01; //sets tmpA to bit position 0, 1, 2, 3
	tmpB = PINA & 0x02;
	tmpC = PINA & 0x04;
	tmpD = PINA & 0x08;
		
	    
	tmpB = tmpB >> 1;
	tmpC = tmpC >> 2;
	tmpD = tmpD >> 3;
		
	cntavail = 0x04 - (tmpA+tmpB+tmpC+tmpD);
	    
		if (cntavail == 0) {
			PORTC = 0x80;
		}
		else {
			tmpB = tmpB << 1;
			tmpC = tmpC << 2;
			tmpD = tmpD << 3;
			cntavail = tmpA + tmpB + tmpC + tmpD;
			cntavail = ~cntavail;		//invert to find which spaces are available in what position
			cntavail = cntavail & 0x0F;	//clear the other 4-bits that are part of 8 bit space
			PORTC = cntavail;			//output the inversion (which are the spots open), which is what we want
		}
			
	}
	return 0;
}

