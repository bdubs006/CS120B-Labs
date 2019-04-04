/*
 * CS120B_lab_part2.c
 *
 * Created: 4/3/2019 6:18:48 PM
 * Author : Bon Won Koo
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
	    // 1) Read input
	    tmpA = PINA & 0x01; //sets tmpA to bit position 0, 1, 2, 3
		tmpB = PINA & 0x02;
		tmpC = PINA & 0x04;
		tmpD = PINA & 0x08;
		
		tmpB = tmpB >> 1;
		tmpC = tmpC >> 2;
		tmpD = tmpD >> 3;
		
		cntavail = 0x04 - (tmpA+tmpB+tmpC+tmpD);
		PORTC = cntavail;
	    
	
    }
    return 0;
}

