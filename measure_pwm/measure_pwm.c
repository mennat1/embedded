/*
 * measure_duty_cycle.c
 *
 *  Created on: Oct 19, 2019
 *      Author: menna
 */



#include "lcd.h"
#include "icu.h"

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0; //T1
uint16 g_timePeriod = 0; //T2
uint16 g_timePeriodPlusHigh = 0; //T3

void APP_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = Icu_getInputCaptureValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}

int main()
{
	uint32 dutyCycle = 0;
	uint32 freq = 0;

	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_CLOCK,RISING};

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Set the Call back function pointer in the ICU driver */
	Icu_setCallBack(APP_edgeProcessing);

	/* Initialize both the LCD and ICU driver */
	LCD_init();
	Icu_init(&Icu_Config); // const Icu_ConfigType * Config_Ptr = &Icu_Config;

	while(1)
	{
		if(g_edgeCount == 4)
		{
			Icu_DeInit(); /* Disable ICU Driver */
			g_edgeCount = 0;
			/*
			LCD_intgerToString(g_timeHigh);
			LCD_displayString(", ");
			LCD_intgerToString(g_timePeriod);
			LCD_displayString(", ");
			LCD_intgerToString(g_timePeriodPlusHigh);
			*/

			LCD_displayString("Duty = ");
			/* calculate the dutyCycle */
			dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
			//dutyCycle = ((float)(g_timeHigh) / (g_timePeriod)) * 100; //This is wrong
			/* display the dutyCycle on LCD screen */
			LCD_intgerToString(dutyCycle);
			LCD_displayCharacter('%');
			LCD_goToRowColumn(1,0);
			if(Icu_Config.clock){
				freq = ((F_CPU/Icu_Config.clock) * (1.0/(float)(g_timePeriodPlusHigh - g_timeHigh)));
				LCD_displayString("Freq = ");
				LCD_intgerToString(freq);
				LCD_displayString(" Hz");

			}
		}
	}
}
