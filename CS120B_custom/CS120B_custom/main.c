/*	Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Lab Section: 028
 *	Assignment: Custom Project
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include "scheduler.h"
#include "io.c"    //LCD fcts
#include <stdio.h>

static unsigned char pattern = 0x80; // LED dot
static unsigned long row = 0xFE; // Row to display pattern.
unsigned char column_val = 0x01; // pattern set on columns
unsigned char column_sel = 0x7F; // ground column
unsigned char mode = 1; //difficulty
unsigned short score = 0; //global score
unsigned long timePassed = 0; //global timer
unsigned char bell[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 }; //Custom character
unsigned char downArrow[8] = {0x04, 0x04, 0x04, 0x04, 0x01F, 0x0E, 0x04, 0x00};  //Custom character 
enum resetter{y, n}resetGame;
	
unsigned char c4 = 0x3F;	//pattern combinations within range of LED matrix
unsigned char c3 = 0xCF;
unsigned char c2 = 0xF3;
unsigned char c1 = 0xFC;
unsigned char c10 = 0x0F;
unsigned char c9 = 0x33;
unsigned char c8 = 0x3C;
unsigned char c7 = 0xC3;
unsigned char c6 = 0xCC;
unsigned char c5 = 0xF0;
unsigned char c12= 0x03;
unsigned char c11 = 0xC0;


//shift register functions to transmit data to LED matrix
void transmit_dataPORTB(unsigned char data) { 
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTB = 0x08;
		// set SER = next bit of data to be sent.
		PORTB |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTB |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTB |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTB = 0x00;
}

void transmit_dataPORTD(unsigned char data) { 
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x08;
		//PORTD &= 0xF8;
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTD |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}

//LCD custom char fct gotten from website: www.electronicwings.com/avr-atmega/lcd-custom-character-display-using-atmega-16-32-
void LCD_Custom_Char (unsigned char loc, unsigned char *msg) 
{
    unsigned char i;
    if(loc<8)
    {
     LCD_WriteCommand(0x40 + (loc*8));  /* Command 0x40 and onwards forces 
                                       the device to point CGRAM address */
       for(i=0;i<8;i++) 
	   { /* Write 8 byte for generation of 1 character */
			LCD_WriteData(msg[i]); 
	   }
    }   
}

//fct to decide which row to put char 
void LCD_Goto(char x, char y)
{
	char addr;
	switch(y)
	{
		case 0:					//top row addr
			addr = 0x00 + x; 
			break; 
		case 1:					//bot row addr
			addr = 0x40 + x; 
			break; 
	}
	char addcmd = (0x80|addr);
	LCD_WriteCommand(addcmd);
}

//lab 8 ADC fct
void ADC_init() {  
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum SM1_States {r1, g1, r2, g2, r3, g3, gStart, nada};
enum started{yes, no}sent;

//fct to navigate in main menu
int SMTick1(int state) {
	// Local Variables
	static unsigned short sensorVal;
	sensorVal = ADC;	//joystick
	
	// Transitions
	switch (state)
	{
		case r1:
			if(sensorVal < 536)
			{
				state = g2;
			}
			else if((~PINA & 0x3C) != 0x00)
			{
				state = gStart;
			}
			break;
		case g1: 
			if(sensorVal >= 536 && sensorVal <= 540)
			{
				state = r1;
			}
			break;
		case r2:
			if(sensorVal < 536)
			{
				state = g3;
			}
			else if(sensorVal > 540)
			{
				state = g1;
			}
			else if((~PINA & 0x3C) != 0x00)
			{
				state = gStart;
			}
			break;
		case g2:
			if(sensorVal >= 536 && sensorVal <= 540)
			{
				state = r2;
			}
			break;
		case r3: 
			if(sensorVal > 540)
			{
				state = g2;
			}
			else if((~PINA & 0x3C) != 0x00)
			{
				state = gStart;
			}
			break;
		case g3:
			if(sensorVal >= 536 && sensorVal <= 540)
			{
				state = r3;
			}
			break;
		case gStart:
			resetGame = n;
			state = nada;
			break;
		case nada:
			if(resetGame == y)
			{
				state = r1;
			}
			else
			{
				state = nada;
			}
			break;
	}

	//State machine actions
	switch(state) {
		case g1:
			LCD_Goto(0,0);
			mode = 1; 
			break;
		case g2:
			LCD_Goto(2,0);
			mode = 2; 
			break;
		case g3:
			LCD_Goto(4,0);
			mode = 3; 
			break;
		case r1:
			LCD_Goto(0,0);
			mode = 1; 
			break;
		case r2:
			LCD_Goto(2,0);
			mode = 2; 
			break;
		case r3:
			LCD_Goto(4,0);
			mode = 3; 
			break;
		case gStart:
			sent = yes;
			LCD_ClearScreen();
			break;
		case nada:
			break;
	}

	return state;
}


enum Demo_States {init, shiftL, waitL} LEDstate;
	
//fct that uses joystick to move LED dot 
void Demo_Tick() {
	// Local Variables
	
	static unsigned short sensor;
	sensor = ADC;
	unsigned short x=536;
	unsigned short y=400;
	

	// Transitions
	switch (LEDstate) {
		case init:
		if (!GetBit(PINA, 0)) {
			LEDstate = init;
		}
		else if (!GetBit(PINA, 3)) {
			LEDstate = waitL;
		}
		else {
			LEDstate = init;
			if(!GetBit(PORTD, 1))
			row = 0xEF;
			if(!GetBit(PORTD, 0))
			row = 0xFE;
		}
		break;
		
		case waitL:
		if(!GetBit(PINA, 0)) {
			LEDstate = waitL;
		}
		else {
			LEDstate = shiftL;
		}
		break;
		
		case shiftL:
		break;
		
		default:
		LEDstate = init;
		break;
	}
	// Actions
	switch (LEDstate) {
		case init:
		break;
		
		case waitL:
		break;
		
		case shiftL:
		if( sensor > y) {
			if (row < 0x3FFC) {
			row = (row << 1) | 0x01;
			}
		}
		else if(sensor < y) {
			if (row > 0xFE) {
			row = (row >> 1) | 0x80;
			}
			else {
				pattern = 0x80;
				row = 0xFE;
			}
		}
		break;
		/*case shiftL:		//code to test LED movement, obtained online for reference
		if (row == 0xEF && pattern == 0x80) { // Reset demo
			pattern = 0x80;
			row = 0xFE;
			} else if (pattern == 0x80) { // Move LED to start of next row
			pattern = 0x80;
			row = (row << 1) | 0x01;
			} else { // Shift LED one spot to the right on current row
			pattern >>= 1;
		}
		break;*/
		default:
		break;
	}
	transmit_dataPORTD(pattern); //Use the shift registers to display onto the matrix
	transmit_dataPORTB(row); //Use the shift registers to display onto the matrix

}enum SM3_States {sm3_display, ending, reset};
enum Reset{done, nDone}gEnd;
	
//fct to reset game and display blocks falling
int SMTick3(int state) {
	// === Local Variables ===
	
	static unsigned short sensor2;
	static unsigned char resetVal = 2;
	static unsigned char1;
	static unsigned char2;
	static unsigned char3;
	static unsigned char4;
	static unsigned char5;
	static unsigned short scroll = 0;
	static unsigned char pattern1[2] = {0x03, 0xCC};
	static unsigned char pattern4[2] = {0xC0, 0x33};
	unsigned short speed;
	// === Transitions ===
	switch (state) {
		
		case sm3_display:   
			if(timePassed == 16000) 
			{
				timePassed = 0;
				state = ending; 
			}
			break;
		case ending:
			if(timePassed == 5000) //display menu for 5 seconds. 
			{
				gEnd = nDone;
				timePassed = 0;
				state = reset;
			}
			else 
			{
				state = ending;
			}
		case reset: 
			if(resetGame == y)
			{
				resetVal = 2;
				state = sm3_display;
			}
			break;
		default:
			state = sm3_display;
			break;
	}
	
	// === Actions ===
	switch (state) {
		
		case sm3_display: 
			
			if(mode == 1) // scrolling speed
			{
				speed = 500;
			}	
			else if(mode == 2)
			{
				speed = 400;
			}
			else if(mode == 3)
			{
				speed = 150;
			}
			if(sent == yes)
			{
				timePassed += 1;
				scroll += 1;
		
				if(pattern1[0] == 0x00) //pattern changes as time progresses
				{
					if(timePassed < 5000) {
						pattern1[0] = 0x03;
						pattern1[1] = c5;
					}
					else if(timePassed > 5000 && timePassed < 10000) {
						pattern1[0] = 0x03;
						pattern1[1] = c10;
					}
					else {
					pattern1[0] = 0x03;
					pattern1[1] = c8; //2 4
					}
				}
	
				
				else if(pattern4[0] == 0x00) //pattern changes as time progresses
				{
					if(timePassed < 5000) {
						pattern4[0] = 0x03;
						pattern4[1] = c4;
					}
					else if(timePassed > 5000 && timePassed < 10000) {
						pattern4[0] = 0x03;
						pattern4[1] = c6;
					}
					else {
					pattern4[0] = 0x03;
					pattern4[1] = c2; //10 5
					}
				}
				if(scroll == speed) //lights scrolling
				{
					pattern1[0] = pattern1[0] << 1;
					pattern4[0] = pattern4[0] << 1;
					scroll = 0;
				}
				if(column_val == pattern1[0]) //alt pattern
				{
					column_val = pattern4[0];
					column_sel = pattern4[1];
				}

				else if (column_val == pattern4[0])
				{
					column_sel = pattern1[1]; 
					column_val = pattern1[0];
				}
				else 
				{
					column_sel = pattern1[1]; 
					column_val = pattern1[0];
				}
				if(pattern1[0] == 0xC0)
				{
					if(pattern1[1] == c2)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
					else if(pattern1[1] == c4)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
					else if(pattern1[1] == c5)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
					else if(pattern1[1] == c6)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
					else if(pattern1[1] == c8)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
					else if(pattern1[1] == c10)
					{
						//if(pattern == pattern1){
							score += 10;
						//}
					}
				}
				
				
				else if(pattern4[0] == 0xC0)
				{
					if(pattern4[1] == c2)
					{
						//if(pattern == pattern4){
							score += 10;
						//}
					}
		
					else if(pattern4[1] == c4)
					{
						if(pattern == pattern4)
						{
							score += 10;
						}
					}
					
					else if(pattern4[1] == c5)
					{
						if(pattern == pattern4)
						{
							score += 10;
						}
					}
					
					else if(pattern4[1] == c6)
					{
						if(pattern == pattern4)
						{
							score += 10;
						}
					}
			
					else if(pattern4[1] == c8)
					{
						if(pattern == pattern4)
						{
							score += 10;
						}
					}
					
					else if(pattern4[1] == c10)
					{
						if(pattern == pattern4)
						{
							score += 10;
						}
					}
		
				}
			}
	
		break;
		case ending:
			timePassed += 1;
			if(gEnd == nDone) //display score
			{
				gEnd = done;
				char1 = score / 10000;
				if(char1 > 0)
				{
					score %= 10000;
				}
				char2 = score / 1000;
				if(char2 > 0)
				{
					score %= 1000;
				}
				char3 = score / 100;
				if(char3 > 0)
				{
					score %= 1000;
				}
				char4 = score / 10;
				char5 = 0; 
				LCD_ClearScreen();
				LCD_Goto(0,0);
				if(char1 == 6)
				{
					LCD_WriteData('6');
				}
				else if(char1 == 5)
				{
					LCD_WriteData('5');
				}
				else if(char1 == 4)
				{
					LCD_WriteData('4');
				}
				else if(char1 == 3)
				{
					LCD_WriteData('3');
				}
				else if(char1 == 2)
				{
					LCD_WriteData('2');
				}
				else if(char1 == 1)
				{
					LCD_WriteData('1');
				}
				else if(char1 == 0)
				{
					LCD_WriteData('0');
				}
				
				LCD_Goto(1,0);
				if(char2 == 9)
				{
					LCD_WriteData('9');
				}
				else if(char2 == 8)
				{
					LCD_WriteData('8');
				}
				else if(char2 == 7)
				{
					LCD_WriteData('7');
				}
				else if(char2 == 6)
				{
					LCD_WriteData('6');
				}
				else if(char2 == 5)
				{
					LCD_WriteData('5');
				}
				else if(char2 == 4)
				{
					LCD_WriteData('4');
				}
				else if(char2 == 3)
				{
					LCD_WriteData('3');
				}
				else if(char2 == 2)
				{
					LCD_WriteData('2');
				}
				else if(char2 == 1)
				{
					LCD_WriteData('1');
				}
				else if(char2 == 0)
				{
					LCD_WriteData('0');
				}
				
				LCD_Goto(2,0);
				if(char3 == 9)
				{
					LCD_WriteData('9');
				}
				else if(char3 == 8)
				{
					LCD_WriteData('8');
				}
				else if(char3 == 7)
				{
					LCD_WriteData('7');
				}
				else if(char3 == 6)
				{
					LCD_WriteData('6');
				}
				else if(char3 == 5)
				{
					LCD_WriteData('5');
				}
				else if(char3 == 4)
				{
					LCD_WriteData('4');
				}
				else if(char3 == 3)
				{
					LCD_WriteData('3');
				}
				else if(char3 == 2)
				{
					LCD_WriteData('2');
				}
				else if(char3 == 1)
				{
					LCD_WriteData('1');
				}
				else if(char3 == 0)
				{
					LCD_WriteData('0');
				}
				else 
				{
					LCD_WriteData('0');
				}
				
				LCD_Goto(3,0);
				if(char4 == 9)
				{
					LCD_WriteData('9');
				}
				else if(char4 == 8)
				{
					LCD_WriteData('8');
				}
				else if(char4 == 7)
				{
					LCD_WriteData('7');
				}
				else if(char4 == 6)
				{
					LCD_WriteData('6');
				}
				else if(char4 == 5)
				{
					LCD_WriteData('5');
				}
				else if(char4 == 4)
				{
					LCD_WriteData('4');
				}
				else if(char4 == 3)
				{
					LCD_WriteData('3');
				}
				else if(char4 == 2)
				{
					LCD_WriteData('2');
				}
				else if(char4 == 1)
				{
					LCD_WriteData('1');
				}
				else if(char4 == 0)
				{
					LCD_WriteData('0');
				}
				else
				{
					LCD_WriteData('0');
				}
				LCD_Custom_Char(5, bell);
				LCD_Goto(5,0);
				LCD_WriteData(5);
				LCD_Custom_Char(6, bell);
				LCD_Goto(6,0);
				LCD_WriteData(6);
				LCD_Custom_Char(7, bell);
				LCD_Goto(7,0);
				LCD_WriteData(7);
				LCD_Custom_Char(4, bell);
				LCD_Goto(4,0);
				LCD_WriteData(4);
				
				LCD_Goto(1,1);
				LCD_WriteData('G');
				LCD_Goto(2,1);
				LCD_WriteData('O');
				LCD_Goto(3,1);
				LCD_WriteData('O');
				LCD_Goto(4,1);
				LCD_WriteData('D');
				LCD_Goto(5,1);
				LCD_WriteData(' ');
				LCD_Goto(6,1);
				LCD_WriteData('S');
				LCD_Goto(7,1);
				LCD_WriteData('C');
				LCD_Goto(8,1);
				LCD_WriteData('O');
				LCD_Goto(9,1);
				LCD_WriteData('R');
				LCD_Goto(10,1);
				LCD_WriteData('E');
				break;
			}
		case reset:
			if(gEnd == nDone) 
			{
				ADC_init();
				gEnd = done; 
				LCD_ClearScreen();
				LCD_DisplayString(23, "Reset");
				LCD_Custom_Char(0, downArrow);
				LCD_Custom_Char(1, downArrow);
				LCD_WriteCommand(0x80);
				LCD_WriteData(0);
				LCD_WriteData(' ');
				LCD_WriteData(1);
				LCD_WriteData(' ');
			
				LCD_Goto(0,1);
				LCD_WriteData('Y');
				LCD_Goto(2,1);
				LCD_WriteData('N');
			}
			sensor2 = ADC;
			if(sensor2 < 536) //go to no reset
			{
				LCD_Goto(2,1);
				resetVal = 0;
				
			}
			else if(sensor2 > 540) //go to "yes" reset
			{
				LCD_Goto(0,1);
				resetVal = 1;
			}
			else if((~PINA & 0x3C) != 0x00)
			{
				while((~PINA & 0x3C) != 0x00)					//check
				{}
				if(resetVal == 1)
				{
					resetGame = y;
					gEnd = nDone;
					sent = no; 
					LCD_ClearScreen();
					LCD_DisplayString(26, "Mode");
					LCD_Custom_Char(0, downArrow);
					LCD_Custom_Char(1, downArrow);
					LCD_Custom_Char(2, downArrow);
					LCD_WriteCommand(0x80);
					LCD_WriteData(0);
					LCD_WriteData(' ');
					LCD_WriteData(1);
					LCD_WriteData(' ');
					LCD_WriteData(2);
					LCD_WriteData(' ');
					
					LCD_Goto(0,1);
					LCD_WriteData('E');
					LCD_Goto(2,1);
					LCD_WriteData('M');
					LCD_Goto(4,1);
					LCD_WriteData('H');
				}
				
				else if(resetVal == 0)
				{
					LCD_ClearScreen();
					LCD_DisplayString(1, "GAME OVER :[");
				}
			}
			break;
			
			default:  
			break;
	}
	
	transmit_dataPORTD(column_val); 
	transmit_dataPORTB(column_sel); 

	return state;
};


// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00; 
	// . . . etc

	// Period for the tasks
	unsigned long int SMTick1_calc = 1;
	unsigned long int SMTick2_calc = 200;
	unsigned long int SMTick3_calc = 1;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);
	tmpGCD = findGCD(tmpGCD, SMTick3_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;
	
	//Declare an array of tasks
	static task task1, task2, task3;
	task *tasks[] = { &task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = r1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.

	// Task 2
	task2.state = init;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &Demo_Tick;//Function pointer for the tick.

	task3.state = sm3_display;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period;//Task current elapsed time.
	task3.TickFct = &SMTick3;//Function pointer for the tick.

	TimerSet(GCD);
	TimerOn();
	
	ADC_init();

	LCD_init();
	
	LCD_DisplayString(1, "Light Dash"); //Main menu at startup
	LCD_Custom_Char(12, bell);
	LCD_WriteData(12);
	LCD_Custom_Char(0, bell);
	LCD_Goto(0,1);
	LCD_WriteData(0);
	LCD_Custom_Char(2, bell);
	LCD_Goto(2,1);
	LCD_WriteData(2);
	LCD_Custom_Char(4, bell);
	LCD_Goto(4,1);
	LCD_WriteData(4);
	LCD_Custom_Char(6, bell);
	LCD_Goto(6,1);
	LCD_WriteData(6);
	LCD_Custom_Char(8, bell);
	LCD_Goto(8,1);
	LCD_WriteData(8);
	LCD_Custom_Char(10, bell);
	LCD_Goto(10,1);
	LCD_WriteData(10);
	
	
	while((~PINA & 0x3C) == 0x00)
	{
		while(!TimerFlag);
		TimerFlag = 0;
	} 
	while((~PINA & 0x3C) != 0x00)
	{
		while(!TimerFlag);
		TimerFlag = 0;
	}
	LCD_ClearScreen();
	/*LCD_Goto(8,0);
	LCD_WriteData('P');
	LCD_Goto(9,0);
	LCD_WriteData('R');
	LCD_Goto(10,0);
	LCD_WriteData('A');
	LCD_Goto(11,0);
	LCD_WriteData('C');
	LCD_Goto(12,0);
	LCD_WriteData('T');
	LCD_Goto(13,0);
	LCD_WriteData('I');
	LCD_Goto(14,0);
	LCD_WriteData('C');
	LCD_Goto(15,0);
	LCD_WriteData('E');*/
	LCD_DisplayString(24, "PRACTICE"); 
	LCD_Custom_Char(0, downArrow);
	LCD_Custom_Char(1, downArrow);
	LCD_Custom_Char(2, downArrow);
	LCD_WriteCommand(0x80);
	LCD_WriteData(0);
	LCD_WriteData(' ');
	LCD_WriteData(1);
	LCD_WriteData(' ');
	LCD_WriteData(2);
	LCD_WriteData(' ');
	
	LCD_Goto(0,1);
	LCD_WriteData('E');
	LCD_Goto(2,1);
	LCD_WriteData('M');
	LCD_Goto(4,1);
	LCD_WriteData('H');

	sent = no; 
	resetGame = n; 
	
	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1; 
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}
