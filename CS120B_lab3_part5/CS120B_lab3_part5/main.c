/*
 * CS120B_lab3_part5.c
 *
 * Created: 4/9/2019 12:25:42 PM
 * Author : Bon Won Koo
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
	return 0;
}

