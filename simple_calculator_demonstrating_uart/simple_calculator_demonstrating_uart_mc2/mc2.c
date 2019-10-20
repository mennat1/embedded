/*
 * mc2.c
 *
 *  Created on: Oct 20, 2019
 *      Author: menna
 */


#include "uart.h"
#include "lcd.h"

int main(void)
{
	uint8 key;
	UART_init();
	LCD_init();
    while(1)
    {
		key = UART_recieveByte(); //receive the pressed key from uart
		if(key == 13){
			LCD_clearScreen();
		}else if((key == '*') || (key == '/') || (key == '+') || (key == '-')||(key == '=') ){
			LCD_displayCharacter(key); //display the pressed keypad switch
		}else{
			LCD_intgerToString(key);
		}
    }
}

