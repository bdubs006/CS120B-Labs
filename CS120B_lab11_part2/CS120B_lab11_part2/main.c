/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab  11 Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "io.c"
#include "io.h"
#include "timer.h"
#include <stdio.h>

//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char input = 0x00;


//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.

#include <avr/io.h>
#include "bit.h"
//#include <ucr/bit.h>

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
unsigned char GetKeypadKey() {

	PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	// Check keys in col 2
	PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }
	// ... ****FINISH****

	// Check keys in col 3
	PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	// ... ****FINISH****
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	// Check keys in col 4	
	// ... ****FINISH****
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }

	return('\0'); // default value

}

unsigned char stringArray[16];
char * msg = "CS120B is Legend... wait for it DARY!";

/*void arrayMover(char arrayMsg[], unsigned char index, unsigned char outputSize, unsigned char msgSize) {
	for (unsigned char i = 0; i < outputSize; i++) {
		stringArray[i] = 0;
	}
	unsigned char currentPosition = outputSize - 1;
	if (index > outputSize) {
		for (unsigned char i = 0; i < index; i++) {
			stringArray[currentPosition - (index - 1 - i)] = arrayMsg[i];
		}
	}
	else {
		for (unsigned char i = 0; i < outputSize; i++) {
			stringArray[outputSize - i - 1] = arrayMsg[index - i];
		}
	}
}*/


enum string_states {init, inputString} stringState;
unsigned char cnt = 0; 

int tick_fct(int state) {
	for (int i = 0; i < 16; i++) {
		stringArray[i] = msg[(cnt+i) % 38];
	}
	cnt = (cnt+1) % 38;
	LCD_DisplayString(1, stringArray);
	/*switch(state) {
		case init:
			cnt = 0;
			state = inputString;
			break;
		case inputString:
			cnt++;
			state = inputString;
		default:
			state = inputString;
			break;
	}
	switch(state) {
		case init:
			break;
		case inputString:
			cnt++;
			arrayMover("CS120B is Legend... wait for it DARY!", cnt, 16, 37);
			LCD_DisplayString(1, stringArray);
			break;
	}*/
	return state;
}

/*enum output_states {LCD_display} outputState;

int tick_fct1(int state) {

	switch(state) {
		case LCD_display:
			state = LCD_display;
			break;
		default:
			state = LCD_display;
			break;
	}
	switch(state) {
		case LCD_display:
			switch(input) {
				case '\0': PORTB = 0x1F; break; // All 5 LEDs on
				case '1': PORTB = 0x01; break; // hex equivalent
				case '2': PORTB = 0x02; break;
				case '3': PORTB = 0x03; break;
				case '4': PORTB = 0x04; break;
				case '5': PORTB = 0x05; break;
				case '6': PORTB = 0x06; break;
				case '7': PORTB = 0x07; break;
				case '8': PORTB = 0x08; break;
				case '9': PORTB = 0x09; break;
				case 'A': PORTB = 0x0A; break;
				case 'B': PORTB = 0x0B; break;
				case 'C': PORTB = 0x0C; break;
				case 'D': PORTB = 0x0D; break;
				case '*': PORTB = 0x0E; break;
				case '0': PORTB = 0x00; break;
				case '#': PORTB = 0x0F; break;
				default: PORTB = 0x1B; break;
			}
			break;
	}
	return state;
}*/



// Monitors button connected to PA0. 
// When button is pressed, shared variable "pause" is toggled.


// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRA = 0xFF; PORTA = 0x00;
DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xF0; PORTC = 0x0F;
DDRD = 0xFF; PORTD = 0x00;
// . . . etc

// Period for the tasks
unsigned long int input_calc = 45;
unsigned long int output_calc = 45;


//Calculating GCD
unsigned long int tmpGCD = 1;
tmpGCD = findGCD(input_calc, output_calc);


//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int inputPeriod = input_calc/GCD;
unsigned long int outputPeriod = output_calc/GCD;


//Declare an array of tasks 
static task task1;
task *tasks[] = { &task1 };
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = inputPeriod;//Task Period.
task1.elapsedTime = inputPeriod;//Task current elapsed time.
task1.TickFct = &tick_fct;//Function pointer for the tick.

/*// Task 2
task2.state = -1;//Task initial state.
task2.period = outputPeriod;//Task Period.
task2.elapsedTime = outputPeriod;//Task current elapsed time.
task2.TickFct = &tick_fct1;//Function pointer for the tick.*/


// Set the timer and turn it on
TimerSet(GCD);
TimerOn();
LCD_init();
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

// Error: Program should not exit!
return 0;
}

