/*
 * adc.h
 *
 *  Created on: 10 jun 2026
 *      Author: fabri
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>
#include <stdint.h>
#ifndef ADC_H_
#define ADC_H_


void cfgADC(void);

void ADC_IRQHandler(void);

extern volatile uint32_t ADC0Value_mV;

#endif /* ADC_H_ */
