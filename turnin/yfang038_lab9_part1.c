/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
	    if (!frequency) { TCCR3B &= 0x08; }
     	    else { TCCR3B |= 0x03; }
     	    if (frequency < 0.954) { OCR3A = 0xFFFF; }
     	    else if (frequency > 31250) { OCR3A = 0x0000; }
     	    else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
     	    TCNT3 = 0;
     	    current_frequency = frequency;
   }
 }

 void PWM_on() {
	TCCR3A = (1 << COM3A0);
   	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
   	set_PWM(0);
 }

 void PWM_off() {
   	TCCR3A = 0x00;
   	TCCR3B = 0x00;
 }

enum States{Start, Init, C4, D4, E4} state;
unsigned char press = ~PINA & 0x07;
void Tick(){
	switch(state){
	    case Start:
		    state = Init;
		    break;
	    case Init:
		    if (press == 0x01){
           		    state = C4;
         	    }else if(press == 0x02){
           		    state = D4;
         	    }else if(press == 0x04){
           		    state = E4;
         	    }else{
           	  	    state = Init;
         	    }
         	    break;
	    case C4:
		    if (press == 0x01) {
             		    state = C4;
         	    }else{
             		    state = Init;
         	    }
         	    break;
	    case D4:
		    if (press == 0x02) {
                            state = D4;
                    }else{
                            state = Init;
                    }
                    break;
	    case E4:
		    if (press == 0x04) {
                            state = E4;
                    }else{
                            state = Init;
                    }
                    break;
	    default:
		    state = Start;
		    break;


	    switch (state) {
		    case Start:	break;
		    case Init:	set_PWM(0); break;
		    case C4:	set_PWM(261.63); break;
		    case D4:	set_PWM(293.66); break;
		    case E4:	set_PWM(329.63); break;
                    default:	break;
	}

	}


}



int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
     	DDRB = 0xFF; PORTB = 0x00;
     	PWM_on();
     	state = Start;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
