/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 10  Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char cnt1 = 0x00;
unsigned char cnt2 = 0x00;
unsigned char cnt3 = 0x00;

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks
//unsigned char tmpA = 0x00;
//unsigned char cntr = 0x00;
unsigned char nextState = 0x00;
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

enum Val_Changer {Val_Init, Val_Wait, Val_P0Press, Val_PCInc, Val_P1Press, Val_PCDec, Val_Reset, Reset_Button, FastUp, FastDown} IncDec;

void tick_LEDS()
{
	switch(IncDec){
		case(Val_Init):
		IncDec = Val_Wait;
		break;
		case(Val_Wait):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_P0Press;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_P1Press;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Reset_Button;
		}
		else{
			IncDec = Val_Wait;
			cnt1 = 0;
			
		}
		break;
		
		case(Val_P0Press):
		if((~PINA & 0x01) == 0x01 && cnt1 < 5){	//5 = 1s 15 = 3s
			IncDec = Val_P0Press; 
			cnt1++;
		}
		
		else if (cnt1 >= 5) {
			IncDec = Val_PCInc;
			cnt1 = 0;
		}
		/*else if ((~PINA & 0x01) == 0x01 && cnt3 >= 15) {
			IncDec = FastUp;
			
		}*/
		else{
			IncDec = Val_Wait;
		}
		break;
		
		case(Val_PCInc):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00) && cnt3 < 15){
			IncDec = Val_PCInc;
			cnt3++;
		}
		else if (cnt3 >= 15) {
			IncDec = FastUp;
			cnt3 = 0;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_P1Press;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Reset_Button;
		}
		else{
			IncDec = Val_Wait;
		}
		break;
		
		case(FastUp):
		if((~PINA & 0x01) == 0x01 && cnt2 < 2){	//5 = 1s 15 = 3s
			IncDec = FastUp;
			cnt2++;
		}
		else if (cnt2 >= 2) {
			IncDec = Val_PCInc;
		}
		else {
			IncDec = Val_Wait;
		}
		break;
		
		case(Val_P1Press):
			if((~PINA & 0x02) == 0x02 && cnt1 < 5){
				IncDec = Val_P1Press;
				cnt1++;
			}
			
			else if (cnt1 >= 5) {
				IncDec = Val_PCDec;
				cnt1 = 0;
			}
			else{
				IncDec = Val_Wait;
			}
			break;
	
		
		case(Val_PCDec):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00) && cnt3 < 15){
			IncDec = Val_PCDec;
			cnt3++;
		}
		else if (cnt3 >= 15) {
			IncDec = FastDown;
			cnt3 = 0;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_P1Press;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Reset_Button;
		}
		else{
			IncDec = Val_Wait;
		}
		break;
		
		case(Reset_Button):
		if((~PINA & 0xFF) != 0x00){
			IncDec = Reset_Button;
		}
		else{
			IncDec = Val_Reset;
		}
		break;
		
		case(FastDown):
		if((~PINA & 0x02) == 0x02 && cnt2 < 2){	//5 = 1s 15 = 3s
			IncDec = FastDown;
			cnt2++;
		}
		else if (cnt2 >= 2) {
			IncDec = Val_PCDec;

		}
		else {
			IncDec = Val_Wait;
		}
		break;
		
		case(Val_Reset):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_P0Press;
		}
		else if(((~PINA & 0x01) == 0x00) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_P1Press;
		}
		else if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x02)){
			IncDec = Val_Reset;
		}
		else{
			IncDec = Val_Wait;
		}
		break;
		default:
		IncDec = Val_Init;
		break;
	}
	switch(IncDec){ // actions
		case Val_Init:
		//PORTC = 0x07;
		//PORTB = (0x01 | 0x02 | 0x04);
		break;
		
		case Val_Wait:
		break;
		
		case Val_P0Press:
		break;

		
		case Val_PCInc:
		if(PORTB >= 0x09){
			PORTB = 0x09;
		}
		else{
			PORTB = PORTB + 0x01;
		}
		break;
		
		case Val_P1Press:
		break;
		
		case Val_PCDec:
		if(PORTB <= 0x00){
			PORTB = 0x00;
		}
		else{
			PORTB = PORTB - 1;
		}
		break;
		
		case FastUp:
		break;
		
		case FastDown:
		break;
		
		case Reset_Button:
		break;
		
		case Val_Reset:
		PORTB = 0x00;
		break;
		
	}
	
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(200);
	TimerOn();
	
	IncDec = Val_Init;
	/* Replace with your application code */
	while (1)
	{
		tick_LEDS();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}