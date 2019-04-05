/*
 * CS120B_lab1_part4.c
 *
 * Created: 4/4/2019 6:43:32 PM
 * Author : Bon Won Koo
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

