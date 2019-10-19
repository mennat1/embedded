/*
 * lcd.h
 *
 *  Created on: Oct 18, 2019
 *      Author: menna
 */
#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "std_types.h"
#include "used_macros.h"

/* LCD HW Pins */
#define RS PD0
#define RW PD1
#define E  PD2
#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_PORT_DIR DDRD
#define LCD_DATA_PORT PORTC
#define LCD_DATA_PORT_DIR DDRC

/* LCD Commands */
#define CLEAR_COMMAND 0x01 //Clear Display (also clear DDRAM content)
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38 //Function Set: 8-bit, 2 Line, 5×7 Dots
#define CURSOR_OFF 0x0C // Display on, Cursor off
#define CURSOR_ON 0x0E  //Display on Cursor on
#define SET_CURSOR_LOCATION 0x80 //Set DDRAM address or cursor position on display:	0x80+add DDRAM address
#define AUTO_INCREMENT 0x06

/* Functions prototypes */
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *Str);
void LCD_init(void);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(uint16 data);

#endif /* LCD_H_ */
