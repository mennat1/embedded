/*
 * mc1.c
 *
 *  Created on: Oct 20, 2019
 *      Author: menna
 */


#include "keypad.h"
#include "uart.h"
// This is a rudimentary calculator that does operations on numbers of max value = 255
//Integer division only
//Limited valid values

sint16 calculate_result(uint8 lhs, uint8 op, uint8 rhs){
	sint16 result;
	switch(op){
	case '+':
		result = (lhs + rhs);
		break;
	case '/':
		result = (lhs/rhs);
		break;
	case '-':
		result = (lhs-rhs);
		break;
	case '*':
		result = (rhs*lhs);
		break;
	}
	return result;
}
int main(void)
{
	uint8 lhs = 0; /*           0 .. 255             */
	uint8 rhs = 0; /*           0 .. 255             */
	uint8 op = 0;
	uint8 eq = 0;
	sint16 result = 0;/*      -32768 .. +32767          */
	UART_init();
    while(1)
    {
    	for(int i =0; i<4; i++){//get lhs
    		uint8 key = KeyPad_getPressedKey(); //get the pressed key
    		if(key == 13){//clear screen
    			UART_sendByte(key);
    			_delay_ms(500);
    			lhs = 0;
    			rhs = 0;
    			op = 0;
    			eq = 0;
    			result = 0;
    			break;
    		}else if((key == '*') || (key == '/') || (key == '-') || (key == '+') ){
    			op = key;
    	       	UART_sendByte(op);
    	        _delay_ms(500);
    			break;
    		}else if((key >=0) && (key <=9)){
    			lhs = (lhs*10) + key;
    		   	UART_sendByte(key);
    		    _delay_ms(500);
    		}
    	}

		for(int i =0; i<4; i++){//get lhs
			uint8 key = KeyPad_getPressedKey(); //get the pressed key
			if(key == 13){//clear screen
				UART_sendByte(key);
				_delay_ms(500);
				lhs = 0;
				rhs = 0;
				op = 0;
				eq = 0;
				result = 0;
				break;
			}else if(key == '='){
				eq = key;
				UART_sendByte(eq);
				_delay_ms(500);
				break;
			}else if((key >=0) && (key <=9)){
				rhs = (rhs*10) + key;
				UART_sendByte(key);
				_delay_ms(500);
			}
		}
    	result = calculate_result(lhs, op, rhs);
    	uint16 p_result;
    	if(result<0){
    		UART_sendByte('-');
    		_delay_ms(500);
    		p_result = (-1) * result;/*           0 .. 65535           */
    		/*
    		UART_sendByte((uint8)(p_result>>8));
    		_delay_ms(500);
    		UART_sendByte((uint8)p_result);
    		_delay_ms(500);*/
    		/*
    		while(p_result !=0){
        		UART_sendByte((uint8)(p_result%10));
        		_delay_ms(500);
        		p_result /= 10;
    		}*/
    	}else{
    		p_result = result;
    		/*
    		UART_sendByte((uint8)(p_result>>8));
    		_delay_ms(500);
    		UART_sendByte((uint8)p_result);
    		_delay_ms(500);*/

    	}

		uint16 divider = 10000;
		if(p_result>=100){
			for(int i=0; i<5; i++){//max result is 5 digits
				UART_sendByte(((p_result/(divider))% 10));
				_delay_ms(500);
				divider /= 10;
			}
		}else{

    		UART_sendByte(p_result);
    		_delay_ms(500);
		}

    	while(1){
    		uint8 key = KeyPad_getPressedKey(); //get the pressed key
    		if(key == 13){
    			UART_sendByte(key);
    			_delay_ms(500);
    			lhs = 0;
    			rhs = 0;
    			op = 0;
    			eq = 0;
    			result = 0;
    			break;
    		}
    	}

    	/*
		key = KeyPad_getPressedKey(); //get the pressed key
		UART_sendByte(key); //send the pressed key to the second MC using uart
		_delay_ms(500);
		*/
    }
}

