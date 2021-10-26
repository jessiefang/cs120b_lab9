/*	Author: Yunjie Fang
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #9  Exercise #2
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

enum States {Start, Up, Down, On, Off, Press} state;
unsigned char press = ~PINA & 0x07;
double array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char cnt = 0x00;
void Tick(){
	switch(state){
	    case Start:
		    state = Off;
		    break;
	    case Off:
		    if(press == 0x01){
			    state = Press;
		    }else{
			    state = Off;
		    }
		    break;
	    case Press:
		    if(press == 0x01){
			    state = Press;
		    }else{
			    state = On;
		    }
		    break;
	    case On:
		    if (press == 0x01){
			    state = Off;
         	    }else if(press == 0x02){
           		    state = Up;
         	    }else if(press == 0x04){
           		    state = Down;
         	    }else{
           	  	    state = Start;
         	    }
         	    break;
	    case Up:
		    if (press == 0x02) {
             		    state = Up;
         	    }else{
             		    state = On;
         	    }
         	    break;
	    case Down:
		    if (press == 0x04) {
                            state = Down;
                    }else{
                            state = On;
                    }
                    break;
	    default:
		    state = Start;
		    break;


	    switch (state) {
		    case Start: break;
		    case Off:	PWM_off(); break;
		    case Press: cnt=0; break;
		    case On: 	set_PWN(array[cnt]);  break;
		    case Up:	if(cnt < 7){
					cnt++;
				}	break;
		    case Down:	if(cnt>0){
					cnt--;
				}	break;
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
