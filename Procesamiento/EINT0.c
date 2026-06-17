/*
 * EINT0.c
 *
 *  Created on: 11 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "EINT0.h"
#include "EINT1.h"
#include "max7219.h"


void cfgEINT0(void)
{
    /* P2.10 -> EINT0 */

    LPC_PINCON->PINSEL4 &= ~(3 << 20);
    LPC_PINCON->PINSEL4 |=  (1 << 20);

    /* Flanco */
    LPC_SC->EXTMODE |= (1 << 0);

    /* Ascendente */
    LPC_SC->EXTPOLAR |= (1 << 0);

    /* Limpia flag */
    LPC_SC->EXTINT |= (1 << 0);

    NVIC_EnableIRQ(EINT0_IRQn);

    return;
}

void EINT0_IRQHandler(void)
{
	if(flag_iniciar == 1)  // solo actúa si el sistema ya arrancó
	    {
	LPC_ADC->ADCR &= ~(1<<21);

	LPC_TIM0->TCR = 0x00;

	MAX7219_Clear();
	    }
    LPC_SC->EXTINT |= (1 << 0);
    return;
}
