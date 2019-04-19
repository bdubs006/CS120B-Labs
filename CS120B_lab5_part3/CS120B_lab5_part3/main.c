/*	Partner 1 Name & E-mail: Bon Won Koo bkoo006@ucr.edu
 *	Partner 2 Name & E-mail: Vinh-Trung Trinh vtrin003@ucr.edu
 *	Lab Section: 028
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

enum Val_Changer {Val_Init, Val_Wait, Val_P0Press, Val_PCInc, Val_P1Press, Val_PCDec, Val_Reset, Reset_Button} IncDec;

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
		}
		
		break;
		case(Val_P0Press):
		if((~PINA & 0x01) == 0x01){
			IncDec = Val_P0Press;
		}
		else{
			IncDec = Val_PCInc;
		}
		break;
		case(Val_PCInc):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_PCInc;
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
		case(Val_P1Press):
		if((~PINA & 0x02) == 0x02){
			IncDec = Val_P1Press;
		}
		else{
			IncDec = Val_PCDec;
		}
		break;
		case(Val_PCDec):
		if(((~PINA & 0x01) == 0x01) && ((~PINA & 0x02) == 0x00)){
			IncDec = Val_PCDec;
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
		
		case Reset_Button:
		break;
		
		case Val_Reset:
		PORTB = 0x00;
		break;
		
	}
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	IncDec = Val_Init;
	/* Replace with your application code */
	while (1)
	{
		tick_LEDS();
	}
	return 1;
}

