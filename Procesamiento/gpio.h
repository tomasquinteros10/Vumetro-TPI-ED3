/*
 * GPIO.h
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef GPIO_H_
#define GPIO_H_


#include <cr_section_macros.h>


#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_gpio.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"
/*====================================================
 * MAX7219
 *===================================================*/

#define MAX7219_PORT        PINSEL_PORT_0

#define MAX7219_DATA_PIN       PINSEL_PIN_0
#define MAX7219_CLOCK_PIN      PINSEL_PIN_1
#define MAX7219_LOAD_PIN       PINSEL_PIN_2

#define MAX7219_DATA_MASK      (1<<0)
#define MAX7219_CLOCK_MASK     (1<<1)
#define MAX7219_LOAD_MASK      (1<<2)

/*====================================================
 * ADC
 *===================================================*/

#define AUDIO_PORT             PINSEL_PORT_0
#define AUDIO_PIN              PINSEL_PIN_23

/*====================================================
 * Prototipos
 *===================================================*/

void cfgGPIO(void);

void cfg_GPIO_MAX7219(void);

void cfgADC_Pin(void);

/*void cfgTIMER_Pin(void); 	no hace falta porque no saco una
							señal por el MAT0.0 el adc se activa
							por interrupcion del macth interna
*/

void GPIO_MAX7219_DataAlto(void);
void GPIO_MAX7219_DataBajo(void);

void GPIO_MAX7219_ClockAlto(void);
void GPIO_MAX7219_ClockBajo(void);

void GPIO_MAX7219_LoadAlto(void);
void GPIO_MAX7219_LoadBajo(void);



#endif /* GPIO_H_ */
