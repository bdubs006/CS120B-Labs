/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 3 Exercise 2
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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char fuellvl = 0x00;
	unsigned char sensorLEDS = 0x00;
    /* Replace with your application code */
    while (1) 
    {
		fuellvl = PINA; //get the input
		switch(fuellvl){
			case 0:
				sensorLEDS = 0x40; //10 0000
				break;
			case 1:
				sensorLEDS = 0x60; //still needs pc6 to be included, 110 0000
				break;			
			case 2:
				sensorLEDS = 0x60;
				break;		
			case 3:
				sensorLEDS = 0x70; //still need pc6
				break;		
			case 4:
				sensorLEDS = 0x70;
				break;		
			case 5:
				sensorLEDS = 0x38;
				break;		
			case 6:
				sensorLEDS = 0x38;
				break;		
			case 7:
				sensorLEDS = 0x3C;
				break;		
			case 8:
				sensorLEDS = 0x3C;
				break;	
			case 9:
				sensorLEDS = 0x3C;
				break;	
			case 10:
				sensorLEDS = 0x3E;
				break;	
			case 11:
				sensorLEDS = 0x3E;
				break;
			case 12:
				sensorLEDS = 0x3E; //111 110
				break;			
			case 13:
				sensorLEDS = 0x3F;
				break;			
			case 14:
				sensorLEDS = 0x3F;
				break;	
			case 15:
				sensorLEDS = 0x3F; //111 111
				break;	
		}
		PORTC = sensorLEDS; //how you output to the display, assign the output to your variablw
		sensorLEDS = 0x00; //reset variables
		fuellvl = 0x00; //reset variables
    }
	
	return 1;
}


