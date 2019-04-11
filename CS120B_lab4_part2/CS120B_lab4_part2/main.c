/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 4 Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum States { start, wait, pressed_incr, incr, pressed_decr, decr, both} State;

void Setter() {
	switch(State) {
		case start: //Initial transition
		State = wait;
		break;
		
		case wait:
		if ((PINA & 0x01) == 0x00) {
			State = wait;
		}
		else if ((PINA & 0x01) == 0x01) {
			State = pressed_incr;
		}
		break;
		
		case pressed_incr:
		if ((PINA & 0x01) == 0x01) {
			State = pressed_incr;
		}
		else if ((PINA & 0x01) == 0x00) {
			State = incr;
		}
		break;
		
		case ON:
		if ((PINA & 0x01) == 0x00) {
			State = incr;
		}
		else if ((PINA & 0x01) == 0x01) {
			State = pressed_decr;
		}
		break;
		
		case pressed_decr:
		if ((PINA & 0x01) == 0x01) {
			State = pressed_decr;
		}
		else if ((PINA & 0x01) == 0x00) {
			State = decr;
		}
		break;
		
		default:
		State = start;
		break;
	}
	
	switch (State) {
		case start:
		break;

		case wait:
		PORTB = 0x01;
		break;
		
		case pressed_incr:
		PORTB = 0x02;
		break;
		
		case incr:
		PORTB = 0x02;
		break;
		
		case pressed_decr:
		PORTB = 0x01;
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	State = start;
	while (1)
	{
		Setter();
	}
	return 1;
}

