/*
 * EINT0.h
 *
 *  Created on: 11 jun. 2026
 *      Author: VICTUS
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef EINT0_H_
#define EINT0_H_

#include <cr_section_macros.h>


#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_gpio.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_pinsel.h"





void cfgEINT0(void);



#endif /* EINT0_H_ */
