/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 4 Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum States { start, wait, pressed_A7, A7, pressed_pound, pound, pressed_y, y, pressed_wrong, wrong, pressed_x, x, unlock} State;
	
enum States step_array[] = {pressed_x, pressed_y, pressed_x};
unsigned char buttons_array[3];
unsigned char cntr = 0x00;

void Setter() {
	unsigned char tmpB = 0x00;
	buttons_array[0] = PINA & 0x01;	// X
	buttons_array[1] = PINA & 0x02;	// Y
	buttons_array[2] = PINA & 0x04; // #
	
	switch(State) {
		case start: //Initial transition
		State = wait;
		break;
		
		case wait:
		if ((PINA & 0x04) == 0x04 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = pressed_pound;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7; 
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = wait;
		}
		else {
			State = pressed_wrong;
		}
		break;
		
		case pressed_pound:
		if ((PINA & 0x04) == 0x04 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = pressed_pound;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = pound;
		}
		break;
		
		case pound:
		if (buttons_array[2] == 0x00 && buttons_array[1] == 0x00 && buttons_array[0] == 0x00 && (PINA & 0x80) == 0x00) {
			State = pound;
		}
		else if (buttons_array[2] == 0x00 && buttons_array[1] == 0x00 && buttons_array[0] == 0x01 && (PINA & 0x80) == 0x00) {
			State = step_array[cntr];
			cntr = cntr + 1;
		}
		else if (buttons_array[2] == 0x00 && buttons_array[1] == 0x00 && buttons_array[0] == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7;
		}
		else {
			State = wait;
		}
		break;
		
		case pressed_x:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x01 && (PINA & 0x80) == 0x00) {
			State = pressed_x;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = x;
		}
		break;
		
		case x:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = x;
		}
		else if (cntr == 0) {
			State = unlock;
		}
		else if (buttons_array[2] == 0x00 && buttons_array[1] == 0x02 && buttons_array[0] == 0x00 && (PINA & 0x80) == 0x00) {
			State = step_array[cntr];
			cntr = 0x00;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7;
		}
		else {
			State = wait;
		}
		break;
		
		case pressed_y:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x02 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = pressed_y;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = y;
		}
		break;
		
		case y:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = y;
		}
		else if (buttons_array[2] == 0x00 && buttons_array[1] == 0x00 && buttons_array[0] == 0x01 && (PINA & 0x80) == 0x00) {
			State = step_array[cntr];
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7;
		}
		else {
			State = wait;
		}
		break;
		
		case pressed_wrong:
		if ((PINA & 0xFF) != 0x00) {
			State = pressed_wrong;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = wrong;
		}
		break;
	
		case wrong:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = wrong;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7;
		}
		else {
			State = wait;
		}
		break;
		
		case pressed_A7:
		if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x80) {
			State = pressed_A7;
		}
		else if ((PINA & 0x04) == 0x00 && (PINA & 0x02) == 0x00 && (PINA & 0x01) == 0x00 && (PINA & 0x80) == 0x00) {
			State = A7;
		}
		break;
		
		case A7:
		State = wait;
		break;
		
		case unlock:
		State = wait;
		break;
		
		default:
		State = start;
		break;
	}
	
	switch (State) {
		case start:
		PORTB = 0x00;
		PORTC = 0x00;
		break;

		case wait:
		PORTC = 0x01;
		break;
		
		case pressed_pound:
		PORTC = 0x02;
		break;
		
		case pound:
		PORTC = 0x03;
		break;
		
		case pressed_x:
		PORTC = 0x0A;
		break;
		
		case x:
		PORTC = 0x0B;
		break;
		
		case pressed_y:
		PORTC = 0x04;
		break;
		
		case y:
		PORTC = 0x05;
		break;
		
		case pressed_wrong:
		PORTC = 0x06;
		break;
		
		case wrong:
		PORTC = 0x07;
		break;
		
		case pressed_A7:
		PORTC = 0x08;
		break;
		
		case A7:
		PORTC = 0x09;
		PORTB = 0x00;
		break;
		
		case unlock:
		tmpB = PORTB;
		if ((tmpB & 0x01) == 0x01) {
			PORTB = 0x00;
		}
		else {
			PORTB = 0x01;
		}
		PORTC = 0x0C;
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs,
	DDRC = 0xFF; PORTC = 0x00;
	
	State = start;
	while (1)
	{
		Setter();
	}
	return 1;
}