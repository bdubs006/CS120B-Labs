/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 4 Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum States { start, OFF, pressed_on, ON, pressed_off} State;

void Setter() {
	switch(State) {
		case start: //Initial transition
			State = OFF;
			break;
		
		case OFF:
			if ((PINA & 0x01) == 0x00) {
				State = OFF;
			}
			else if ((PINA & 0x01) == 0x01) {
				State = pressed_on;
			}
			break;
		
		case pressed_on:
			if ((PINA & 0x01) == 0x01) {
				State = pressed_on;
			}
			else if ((PINA & 0x01) == 0x00) {
				State = ON;
			}
			break;
		
		case ON:
			if ((PINA & 0x01) == 0x00) {
				State = ON;
			}
			else if ((PINA & 0x01) == 0x01) {
				State = pressed_off;
			}
			break;
		
		case pressed_off:
			if ((PINA & 0x01) == 0x01) {
				State = pressed_off;
			}
			else if ((PINA & 0x01) == 0x00) {
				State = OFF;
			}
			break;
		
		default:
			State = start;
			break;
	}
	
	switch (State) {
		case start:
			break;

		case OFF:
			PORTB = 0x01; 
			break;
		
		case pressed_on:
			PORTB = 0x02;
			break;
		
		case ON:
			PORTB = 0x02;
			break;
		
		case pressed_off:
			PORTB = 0x01;
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	
	State = start; 	
    while (1) 
    {
		Setter();
    }
	return 1;
}

