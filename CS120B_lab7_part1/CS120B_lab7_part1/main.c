/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 7 Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"
unsigned char DisplayVal = 0x00;
enum Val_Changer {Val_Init, Val_Wait, Val_PCInc, Val_PCDec, Val_Reset} IncDec;

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



void tick_LEDS()
{
	switch(IncDec){
		case(Val_Init):
		DisplayVal = 0x00; 
		LCD_ClearScreen();
		LCD_WriteData(DisplayVal + '0');
			IncDec = Val_Wait;
			break;
		case(Val_Wait):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_PCInc;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_PCDec;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			//((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)
			IncDec = Val_Reset;
		}
		else{
			IncDec = Val_Wait;
		}
		
		break;
		/*case(Val_P0Press):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Reset_Button;
		}
		else if((~PINA & 0x01) == 0x01){
			IncDec = Val_P0Press;
		}
		else{
			IncDec = Val_PCInc;
		}
		break;*/
		case(Val_PCInc):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			//((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)
			IncDec = Val_Reset;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_PCInc;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_PCDec;
		}
		else {
			IncDec = Val_Wait;
		}
		break;
		/*case(Val_P1Press):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Reset_Button;
		}
		else if((~PINA & 0x02) == 0x02){
			IncDec = Val_P1Press;
		}
		else{
			IncDec = Val_PCDec;
		}
		break;*/
		case(Val_PCDec):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			//((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)
			IncDec = Val_Reset;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_PCInc;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_PCDec;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			//((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)
			IncDec = Val_Reset;
		}
		else{
			IncDec = Val_Wait;
		}
		break;
		/*case(Reset_Button):
		if((~PINA & 0xFF) != 0x00){
			IncDec = Reset_Button;
		}
		else{
			IncDec = Val_Reset;
		}
		break;*/
		case(Val_Reset):
			IncDec = Val_Wait;
			break;
		
		default:
			IncDec = Val_Init;
			break;
	}
	switch(IncDec){ // actions
		case Val_Init:
		LCD_ClearScreen();
		LCD_WriteData(DisplayVal + '0');
		//PORTC = 0x07;
		//PORTB = (0x01 | 0x02 | 0x04);
		break;
		
		case Val_Wait:
		break;
		
		/*case Val_P0Press:
		break;*/
		
		case Val_PCInc:
		if(DisplayVal >= 0x09){
			DisplayVal = 0x09;
		}
		else{
			DisplayVal = DisplayVal + 0x01;
		}
		LCD_ClearScreen();
		LCD_WriteData(DisplayVal + '0');
		break;
		
		/*case Val_P1Press:
		break;*/
		
		case Val_PCDec:
		if(DisplayVal <= 0x00){
			DisplayVal = 0x00;
		}
		else{
			DisplayVal = DisplayVal - 1;
		}
		LCD_ClearScreen();
		LCD_WriteData(DisplayVal + '0');
		break;
		/*
		case Reset_Button:
			DisplayVal = 0x00;
			LCD_ClearScreen();
			LCD_WriteData(DisplayVal + '0');
			break;
		*/
		case Val_Reset:
			DisplayVal = 0x00;
			LCD_ClearScreen();
			LCD_WriteData(DisplayVal + '0');
			break;
		
	}
	
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	TimerSet(100);
	TimerOn();
	// Initializes the LCD display
	LCD_init();
	
	// Starting at position 1 on the LCD screen, writes Hello World
	
	while(1) {
			while(!TimerFlag);
			TimerFlag = 0;
			tick_LEDS();
	}
}



