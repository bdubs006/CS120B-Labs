/*
 * CS120B_lab1_part1.c
 *
 * Created: 4/3/2019 5:23:37 PM
 * Author : Bon Won Koo
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	while(1)
    {
	// 1) Read input
	tmpA = PINA & 0x01;
	tmpB = PINA & 0x02;
	// 2) Perform computation
	// if PA0 is 1, set PB1PB0 = 01, else = 10
		if (tmpA == 0x01 && tmpB == 0x00) { // True if PA0 is 1
			PORTB = 0x01;
		}
		else {
			PORTB = 0x00;
		}
	    
    }
    return 0;
}

