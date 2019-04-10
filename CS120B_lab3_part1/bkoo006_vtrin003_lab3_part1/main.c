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
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char cntrA = 0x00;
	unsigned char cntrB = 0x00;
	unsigned char totAmt = 0x00;
	
	
	/* Replace with your application code */
	while (1)
	{
		cntrA = 0x00;
		cntrB = 0x00;
		totAmt = 0x00;
		for(unsigned int i = 0x00; i < 8; i++){
			cntrA = cntrA + GetBit(PINA, i);
			cntrB = cntrB + GetBit(PINB, i);
		}
		totAmt = cntrA + cntrB;
		PORTC = totAmt;
	}
	return 1;
}

