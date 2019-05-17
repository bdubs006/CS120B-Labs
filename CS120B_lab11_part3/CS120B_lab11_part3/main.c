/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab  11 Exercise 3
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

unsigned char temp;

enum Keypad_states {press} keypadState;
	
int tick_fct(int state) {

	switch(state) {
		case press:
			state = press;
			break;
		default:
			state = press;
			break;
	}
	switch(state) {
		case press:
			if(GetKeypadKey() != '\0') {
				input = GetKeypadKey();
				LCD_ClearScreen();
				LCD_WriteData(temp);
				
			}
			
			break;
	}
	return state;
}


enum output_states {output_press} outputState;

int tick_fct1(int state) {

	switch(state) {
		case output_press:
			state = output_press;
			break;
		default:
			state = output_press;
			break;
	}
	switch(state) {
		case output_press:
			switch(input) {
				case '\0': temp = temp; break; // All 5 LEDs on
				case '1': temp = '1'; break; // hex equivalent
				case '2': temp = '2'; break;
				case '3': temp = '3'; break;
				case '4': temp = '4'; break;
				case '5': temp = '5'; break;
				case '6': temp = '6'; break;
				case '7': temp = '7'; break;
				case '8': temp = '8'; break;
				case '9': temp = '9'; break;
				case 'A': temp = 'A'; break;
				case 'B': temp = 'B'; break;
				case 'C': temp = 'C'; break;
				case 'D': temp = 'D'; break;
				case '*': temp = '*'; break;
				case '0': temp = '0'; break;
				case '#': temp = '#'; break;
				default: temp = ' '; break;
			}
			break;
	}
	return state;
}



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
unsigned long int input_calc = 30;
unsigned long int output_calc = 30;


//Calculating GCD
unsigned long int tmpGCD = 1;
tmpGCD = findGCD(input_calc, output_calc);


//Greatest common divisor for all tasks or smallest time unit for tasks.
unsigned long int GCD = tmpGCD;

//Recalculate GCD periods for scheduler
unsigned long int inputPeriod = input_calc/GCD;
unsigned long int outputPeriod = output_calc/GCD;


//Declare an array of tasks 
static task task1, task2;
task *tasks[] = { &task1, &task2 };
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = inputPeriod;//Task Period.
task1.elapsedTime = inputPeriod;//Task current elapsed time.
task1.TickFct = &tick_fct;//Function pointer for the tick.

// Task 2
task2.state = -1;//Task initial state.
task2.period = outputPeriod;//Task Period.
task2.elapsedTime = outputPeriod;//Task current elapsed time.
task2.TickFct = &tick_fct1;//Function pointer for the tick.


// Set the timer and turn it on
TimerSet(GCD);
TimerOn();

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

