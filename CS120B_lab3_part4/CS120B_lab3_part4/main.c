/*
 * CS120B_lab3_part4.c
 *
 * Created: 4/9/2019 10:18:13 AM
 * Author : Bon Won Koo
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
}

