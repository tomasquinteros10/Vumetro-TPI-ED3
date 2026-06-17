/*
 * EINT1.h
 *
 *  Created on: 11 jun. 2026
 *      Author: VICTUS
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif


#ifndef EINT1_H_
#define EINT1_H_

#include <cr_section_macros.h>


#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_gpio.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"



void cfgEINT1(void);
extern volatile uint8_t flag_iniciar;


#endif /* EINT1_H_ */
