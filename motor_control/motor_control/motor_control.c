/*
 * motor_control.c
 *
 *  Created on: Oct 18, 2019
 *      Author: menna
 */

/*
* Read resistance value and display its ADC value on the LCD
* control motor speed by making the duty cycle of its driving pwm signal
* proportional to the read analog value:
* i.e.: max resistance -> max duty cycle -> highest speed
        min resistance -> min duty cycle -> lowest speed

* control motor direction using a pushbutton
* Note: clockwise rotation -> in1 = 0, in2 = 1
* 		anti clockwise rotation -> in1 = 1, in2 = 0
* 		Stop -> int1 = int2 = 0

*/
/* ADC vaue = 1023 * (Vin/ AREF), in which Vin is the potentiometer output voltage.
 * I used a 1k resistor and 5v supply voltage. i.e.: Volts due to potentiometer changes:
 * 10 ohm -> Vin = 0.05v -> ADC = 10
 * 20 ohm -> Vin = 0.1v -> ADC = 20
 * .
 * .
 * .
 * 1000 ohm -> Vin = 5v -> ADC = 1023
 * */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc.h"
#include "lcd.h"


void Timer0_PWM_Init(uint16 set_duty_cycle)
{
	SREG  |= (1<<7); // Enable interrupts by setting I-bit
	TCNT0 = 0; //Timer initial value
	OCR0  = set_duty_cycle; //compare value
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

int main(void){
	//pins direction
	DDRD &= ~(1<<PD3);//pushbutton
	DDRA &= ~(1<<PA0);//analog input (output of the potentiometer)
	DDRB |= 0x0B; //motor driver input pins

	//initialize motor direction to clockwise
	PORTB |= (1<<PB0);
	PORTB &= ~(1<<PB1);

	PORTD |= (1<<PD3); //activate pull up resistor

	/* Initialize LCD module */
	LCD_init();

	/* Initialize ADC driver */
	ADC_init();

	uint16 adc_value = 0;
	uint16 prev_adc_value = 0;
	char button_released = 1;

	while(1){

		LCD_goToRowColumn(0,0); //display the number every time at this position
		adc_value = ADC_readChannel(0); //read channel zero where the potentiometer is connected
		if(((adc_value < 1000) && (prev_adc_value >=1000)) ||
				((adc_value < 100) && (prev_adc_value >=100))){
			LCD_clearScreen();
		}
		LCD_intgerToString(adc_value); //display the ADC value on LCD screen, ADC resolution is 10 bits, i.e.: 0 -> 1023
		Timer0_PWM_Init(adc_value/4); // control the duty cycle of the output PWM signal according to the ADC value
		prev_adc_value = adc_value;
		if(((PIND & (1<<PD3)) == 0) && (button_released = 1)){
			//toggle motor input bits to reverse direction
			//Note: A^1 = A', A^0 = A
			PORTB ^= 0x03;
			button_released = 0;
		}
		if((PIND & (1<<PD3)) == 1){
			button_released = 1;
		}

	}


}
