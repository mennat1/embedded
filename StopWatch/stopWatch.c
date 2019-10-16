/*
 * stopWatch.c
 *
 *  Created on: Oct 16, 2019
 *      Author: menna
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define NUMBER_OF_COMPARE_MATCHES_PER_SECOND ((uint8_t)(4))
uint8_t n_of_cmp_matches = 0;
uint8_t second_counted = 0;

//initialize stopwatch segments
uint8_t secs = 0;
uint8_t mins = 0;
uint8_t hrs = 0;

//manage pushbuttons
uint8_t pause_pushed = 0;
uint8_t pause_released = 1;
uint8_t reset_released = 1;

ISR(TIMER0_COMP_vect)
{
	n_of_cmp_matches++;
	if(n_of_cmp_matches == NUMBER_OF_COMPARE_MATCHES_PER_SECOND)
	{
		second_counted = 1;
		n_of_cmp_matches = 0; //clear the tick counter again to count a new second
	}
}

void timer0_init_CTC_mode(uint8_t tick)
{
	TCNT0 = 0; //timer initial value
	OCR0  = tick; //compare value
	//TIMSK |= (1<<OCIE0); //enable compare interrupt
	/* Configure timer0 control register
	 * 1. Non PWM mode FOC0=1
	 * 2. CTC Mode WGM01=1 & WGM00=0
	 * 3. No need for OC0 so COM00=0 & COM01=0
	 * 4. clock = F_CPU/1024 CS00=1 CS01=1 CS02=0
	 */
	TCCR0 = ((1<<FOC0) | (1<<WGM01) | (1<<CS02) | (1<<CS00));

}
int main(void){
	//Pins direction
	DDRA |= 0x3F;
	DDRC |= 0x0F;
	DDRD &= (~(1<<2));
	DDRD &= (~(1<<3));

	//Activate pull up resistors
	PORTD |= (1<<2);
	PORTD |= (1<<3);

	//initialize input to decoder
	PORTC &= 0xF0;

	//initialize enables
	PORTA |= 0x3F;


	// enable global interrupts
	SREG  |= (1<<7);

	//Configure timer
	timer0_init_CTC_mode(250);


	while(1){
		//Configure PAUSE/RESUME button
		if(((PIND & (1<<2)) == 0) && (pause_released == 1)){// pause/resume pushed
			if(pause_pushed == 0){//resume
				n_of_cmp_matches = 0;
				second_counted = 0;
				TIMSK |= (1<<OCIE0); //enable compare interrupt
				pause_pushed = 1;
			}
			else if(pause_pushed == 1){// pause
				TIMSK &= (~(1<<OCIE0));//disable compare interrupt
				pause_pushed = 0;
			}

			pause_released =0;
		}
		if(((PIND & (1<<2)))){
			pause_released = 1;
		}

		//Configure RESET button
		if(((PIND & (1<<3)) == 0) &&(reset_released == 1)){//reset pushed
			TIMSK &= (~(1<<OCIE0));//disable compare interrupt
			//n_of_cmp_matches = 0;
			//second_counted = 0;
			secs = 0; mins = 0; hrs = 0;
			reset_released = 0;
			pause_pushed = 0;
		}
		if(((PIND & (1<<3)))){
			reset_released = 1;
		}

		if((second_counted == 1)){
			secs++;
			if(secs == 60){
				secs = 0;
				mins++;
				if(mins == 60){
					mins = 0;
					hrs++;
					if(hrs == 24){
						hrs = 0;
					}
				}
			}
			second_counted = 0;
		}

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x01));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(secs%10))));
		//PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(secs%10))&0x0F));
		_delay_ms(2);

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x02));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(secs/10))));
		_delay_ms(2);

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x04));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(mins%10))));
		_delay_ms(2);

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x08));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(mins/10))));
		_delay_ms(2);

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x10));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(hrs%10))));
		_delay_ms(2);

		PORTA = (uint8_t)((PORTA & 0b11000000) | (0x20));
		PORTC = (uint8_t)((PORTC & 0xF0) | (((uint8_t)(hrs/10))));
		_delay_ms(2);

	}
	return 0;
}
