/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 10 Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum threeLED_States{threeInit, three0, threeMiddle, threeEnd} three_State;
enum blinker_States{blinkInit, blinkOff, blinkOn} blinkState;
enum combine_States{Init, Combine} comboMaker;
unsigned char outputThree;
unsigned char outputBlink;
unsigned char cntthree;
unsigned char cntblink;
void three_Tick(){
	switch(three_State){ //transition
		case(threeInit):
		cntthree = 0x00;
		three_State = three0;
		break;
		case(three0):
		//three_State = threeMiddle;
		++cntthree;
		if(cntthree > 3){
			cntthree = 0x00;
			three_State = three0;
		}
		else{
			three_State = threeMiddle;
		}
		break;
		case(threeMiddle):
		++cntthree;
		if(cntthree >3){
			cntthree = 0x00;
			three_State = three0;
		}
		else{
			three_State = threeEnd;
		}
		break;
		case(threeEnd):
		++cntthree;
		if(cntthree > 3){
			cntthree = 0x00;
			three_State = three0;
		}
		else{
			three_State = three0;
		}
		break;
		default:
		break;
	}
	switch(three_State){//actions
		case threeInit:
		break;
		case three0:
		outputThree = 0x02;
		break;
		case threeMiddle:
		outputThree = 0x04;
		break;
		case threeEnd:
		outputThree = 0x08;
		break;
		default:
		break;
	}
}

void blinker_Tick(){
	switch(blinkState){//transitions
		case blinkInit:
		blinkState = blinkOff;
		break;
		case blinkOff:
		cntblink++;
		if(cntblink > 10){
			cntblink = 0x00;
			blinkState = blinkOff;
		}
		else{
			blinkState = blinkOn;
		}
		//blinkState = blinkOn;
		break;
		case blinkOn:
		cntblink++;
		if(cntblink > 10){
			cntblink = 0x00;
			blinkState = blinkOff;
		}
		else{
			blinkState = blinkOff;
		}
		break;
		default:
		break;
	}
	switch(blinkState){//actions
		case blinkInit:
		break;
		case blinkOff:
		outputBlink = 0x00;
		break;
		case blinkOn:
		outputBlink = 0x01;
		break;
		default:
		break;
	}
}
void combo_Tick(){
	switch(comboMaker){
		case Init:
		comboMaker = Combine;
		break;
		case Combine:
		comboMaker = Combine;
		break;
		default:
		break;
	}
	switch(comboMaker){
		case Init:
		break;
		case Combine:
		PORTB = outputBlink | outputThree;
		break;
		default:
		break;
	}
}



int main(void)
{
	blinkState = blinkInit;
	three_State = threeInit;
	comboMaker = Init;
	TimerSet(100);
	TimerOn();
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	/* Replace with your application code */
	while (1)
	{
		blinker_Tick();
		three_Tick();
		combo_Tick();
		//blinker_Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}



