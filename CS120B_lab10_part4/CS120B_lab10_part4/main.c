/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 10 Exercise 4
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

enum threeLED_States{threeInit, three0, threeMiddle, threeEnd} three_State;
enum blinker_States{blinkInit, blinkOn, blinkOff} blinkState;
enum combine_States{Init, Combine} comboMaker;
enum speaker_States{SpeakInit,SpeakOn,SpeakOff, SpeakWait} speakState;

enum Freq_States{Freq_Init, FreqDown, FreqUp, FreqWait} FreqState;
	
unsigned short outputThree = 0x00;
unsigned short outputBlink = 0x00;
unsigned short outputSpeaker = 0x00;
unsigned short cntblink = 0x00;
unsigned short cntthree = 0x00;
unsigned short cntspeak = 0x00;
unsigned short outputtemp = 0x00;

//unsigned long Inc = 0x00;
//unsigned long Dec = 0x00;
unsigned short FreqScale = 0x02;
unsigned short FreqCnt = 0x00;


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

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
/*void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}*/


void three_Tick()
{
	switch(three_State){ //transition
		case(threeInit):
			//cntthree = 0x00;
			three_State = three0;
			break;
		case(three0):
			//three_State = threeMiddle;
			
			if(cntthree < 300){
				three_State = three0;
				++cntthree;
			}
			else{
					three_State = threeMiddle;
					cntthree = 0;
			}
			break;
			
		case(threeMiddle):
			
			if(cntthree < 300){				
				three_State = threeMiddle;
				++cntthree;
			}
			else{
				three_State = threeEnd;
				cntthree = 0;
			}
			break;
			
		case(threeEnd):
			
			if(cntthree < 300){
				three_State = threeEnd;				
				++cntthree;
			}
			else{
				three_State = three0;
				cntthree = 0;
			}
			break;		
		default:
			break;
	}
	switch(three_State){//actions
		case threeInit:
			break;
		case three0:
			outputThree = 0x01;
			break;
		case threeMiddle:
			outputThree = 0x02;
			break;
		case threeEnd:
			outputThree = 0x04;
			break;
		default:
			break;
	}
}

void blinker_Tick(){
	switch(blinkState){//transitions
		case blinkInit:
			//cntblink = 0x00;
			blinkState = blinkOn;
			break;
			
		case blinkOn:
			if(cntblink < 1000){
				//++cntblink;
				blinkState = blinkOn;
				++cntblink;
			}
			else{
				blinkState = blinkOff;
				cntblink = 0;
			}
			//blinkState = blinkOn;
			break;
		case blinkOff:
			if(cntblink < 1000){
				blinkState = blinkOff;
				++cntblink;
			}
			else{
				blinkState = blinkOn;	
				cntblink = 0;
			}
			break;

	}
	switch(blinkState){//actions
		case blinkInit:
			break;
		
		case blinkOn:
			outputBlink = 0x08;
			break;
			
		case blinkOff:
			outputBlink = 0x00;
			break;
		
	}
}

void speaker_Tick(){
	switch(speakState){//transitions
		case SpeakInit:
		speakState = SpeakWait;
		cntspeak = 0;
		break;
		case SpeakOn:
		if(((~PINA & 0x04) == 0x04) && (cntspeak < FreqScale)){
		//	++cntspeak;
			speakState = SpeakOn;
			++cntspeak;
		}
		else if((~PINA & 0x04) == 0x04){
			speakState = SpeakOff;
			cntspeak = 0;
		}
		else{
			speakState = SpeakWait;
			
		}
		break;
		case SpeakOff:
		if(((~PINA & 0x04) == 0x04) && (cntspeak < FreqScale)){
			speakState = SpeakOff;
			++cntspeak;
			
		}
		else if((~PINA & 0x04) == 0x04){
			speakState = SpeakOn;
			cntspeak = 0;
		}
		else{
			speakState = SpeakWait;
		
		}
		break;
		
		case SpeakWait:
			if((~PINA & 0x04) == 0x04) {
				speakState = SpeakOn;
			}
			else {
				speakState = SpeakWait;
			}
			break;
		default:
		break;
	}
	switch(speakState){//actions
		case SpeakInit:
		break;
		case SpeakOn:
		outputSpeaker = 0x40;
		break;
		case SpeakOff:
		outputSpeaker = 0;
		break;
		case SpeakWait:
		outputSpeaker = 0x00;
		cntspeak = 0;
		break;
		
		default:
		break;
		
	}
}

/*enum Freq_States{Freq_Init, FreqDown, FreqUp, FreqWait} FreqState;
unsigned long FreqScale = 0x00;*/
void Freq_Tick(){
	switch(FreqState){//transitions
		case Freq_Init:
			FreqState = FreqWait;
			break;
		case FreqWait:
			if(((~PINA & 0x01) == 0x01) && FreqScale < 7 && FreqCnt > 1000){
			//	FreqCnt++;
				FreqState = FreqUp;
				FreqCnt = 0;
			}
			else if(((~PINA & 0x02) == 0x02) && FreqScale > 0 && FreqCnt > 1000){
				//FreqCnt++;
				FreqState = FreqDown;
				FreqCnt = 0;
			}
			else{
				FreqState = FreqWait;
				FreqCnt++;
			}
			break;
		case FreqUp:
			FreqState = FreqWait;
			break;
		case FreqDown:
			FreqState = FreqWait;
			break;
		
		default:
			break;
	}
		switch(FreqState){ //actions
			case Freq_Init:
				break;
			case FreqWait:
				break;
			case FreqUp:
				FreqScale++;
				break;
			case FreqDown:
				FreqScale--;
				break;
		}
}

void combo_Tick(){
	switch(comboMaker){
		case Init:
			comboMaker = Combine;
			break;
		case Combine:
			//comboMaker = Combine;
			break;
		default:
			comboMaker = Init;
			break;
	}
	switch(comboMaker){
			case Init:
				break;
			case Combine:
				outputtemp = outputThree | outputBlink | outputSpeaker;
				PORTB = outputtemp;
				break;
			default:
				break;

	}
}
	


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
		
	
	three_State = threeInit;
	blinkState = blinkInit;
	speakState = SpeakInit;
	comboMaker = Init;
	
	cntblink = 0;
	cntthree = 0;
	cntspeak = 0;
	
	TimerSet(1);
	TimerOn();
	

    /* Replace with your application code */
    while (1) 
    {
		
		three_Tick();
		blinker_Tick();
		Freq_Tick();
		speaker_Tick();
		combo_Tick();
		while(!TimerFlag){}
		TimerFlag = 0;

    }
	
}




