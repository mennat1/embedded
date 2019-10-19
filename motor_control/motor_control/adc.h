/*
 * adc.h
 *
 *  Created on: Oct 18, 2019
 *      Author: menna
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "std_types.h"
#include "used_macros.h"
/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(void);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */

uint16 ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */
