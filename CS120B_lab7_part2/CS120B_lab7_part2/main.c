/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 7 Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
//unsigned char tmpA = 0x00;
//unsigned char cntr = 0x00;
unsigned char nextState = 0x00;
unsigned char cnt = 0x05;
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

enum States{Init, Wait, Reset, LED0, LED1, LED2, UpdateScore}State;

void Tick_FCT(){
	switch(State){
		case Init:
		State = LED0;
		break;
		case LED0:
		if((~PINA & 0x01) == 0x01){
			State = Wait;
			break;
		}
		else{
			State = LED1;
			nextState = LED2;
			break;
		}
		case LED1:
		if((~PINA & 0x01) == 0x01){
			State = Wait;
			break;
		}
		else{
			State = nextState;
			break;
		}
		case LED2:
		if((~PINA & 0x01) == 0x01){
			State = Wait;
			break;
		}
		else{
			nextState = LED0;
			State = LED1;
			break;
		}
		case Wait:
		if((~PINA & 0x01) == 0x01){
			State = UpdateScore;
			break;
		}
		else{
			State = Wait;
			break;
		}
		//State = Wait;
		break;
		
		case UpdateScore:
		State = Reset;
		break;
		case Reset:
		if((PINA & 0x01) == 0x01){
			State = LED0;
			break;
		}
		else{
			State = Reset;
			break;
		}
		//State = Init;
		//break;
		default:
		LCD_ClearScreen();
		LCD_WriteData(cnt + '0');
		State = Init;
		break;
	}
	switch(State){
		case Init:
		break;
		
		case LED0:
		PORTB = 0x01;
		break;
		
		case LED1:
		PORTB = 0x02;
		break;
		
		case LED2:
		PORTB = 0x04;
		break;
		
		case Wait:
		case Reset:
		break;
		
		case UpdateScore:
		if((PINB == 0x02 && cnt < 9)){
			cnt = cnt + 1;
		}
		else if((PINB == 0x01 && cnt > 0)){
			cnt = cnt - 1;
		}
		else if((PINB == 0x04 && cnt > 0)){
			cnt = cnt - 1;
		}
		
		LCD_ClearScreen();
		LCD_WriteData(cnt + '0');
		State = Reset;
		if(cnt >= 9){
			LCD_ClearScreen();
			LCD_DisplayString(1, "Victory!");
			
		}
		break;
		default:
		PORTB = 0x01;
		break;
	}
}


// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; // Set port B to output
	PORTB = 0x00; // Init port B to 0s
	
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0xFF;
	
	TimerSet(60);
	TimerOn();
	LCD_init();
	LCD_ClearScreen();
	
	State = -1;
	//unsigned char tmpA = 0x00;
	//unsigned char cntr = 0x00;
	//tmpA = ~(PINA & 0x01);
	//unsigned char tmpB = 0x00;
	while(1) {
		//tmpA = ~(PINA & 0x01);
		// User code (i.e. synchSM calls)
		Tick_FCT();
		//tmpB = ~tmpB;	// Toggle PORTB; Temporary, bad programming style
		//PORTB = tmpB;
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}





