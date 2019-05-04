/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 9 Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

enum{Init, Inc, Dec, Begin, Wait, Press}var;

double array[9] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25, 0};
double tempVal = 0;

unsigned char low = 0x00;
unsigned char high = 0x07;
unsigned char place = 0x00;
unsigned char daclick = 0x00;
unsigned char substitute = 0x00;


void Tick_FCT(){
	switch(var){
		case Init:
			var = Begin;
			break;
		
		case Begin:
		if((~PINA & 0x07) == 0x01){
			var = Inc;
			break;
		}
		else if((~PINA & 0x07) == 0x02){
			var = Dec;
			break;
		}
		else if((~PINA & 0x07) == 0x04){
			var = Press;
			break;
		}
		else{
			var = Begin;
			break;
		}
		
		case Inc:
			tempVal = array[place];
			var = Wait;
			break;
		
		case Dec:
			tempVal = array[place];
			var = Wait;
			break;
		
		case Wait:
		if((~PINA & 0x07) == 0x00){
			var = Init;
			break;
		}
		else{
			var = Wait;
			break;
		}
		
		case Press:
			var = Wait;
			break;
		
		default:
		break;
	}
	switch(var){ //actions
		//Init, Inc, Dec, Begin, Wait, Press
		case Init:
		break;
		case Begin:
		break;
		
		case Inc:
		if((place + 1) > high){
			place = high;
			break;
		}
		else{
			++place;
			break;
		}
		
		case Dec:
		if((place-1) < low){
			place = low;
			break;
		}
		else{
			--place;
			break;
		}
		case Wait:
			tempVal = array[place];
			set_PWM(tempVal);
			break;
		
		case Press:
		if(daclick){
			PWM_off();
			substitute = place;
			place = 8;
			daclick = 0;
			break;
		}
		else{
			PWM_on();
			place = substitute;
			daclick = 1;
			break;
		}
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	//C4 = 261.63
	//D = 293.66
	//E = 329.63
	//F = 349.23
	//G = 392
	//A = 440
	//B = 493.88
	// i extended the code to have the 500 note and a 0 note to allow it to set off
	//wasnt working because i never had a state to actually set and play the tone
	/* Replace with your application code */
	var = Init;
	PWM_on();
	daclick = 1;
	while (1)
	{
		Tick_FCT();
	}
}

