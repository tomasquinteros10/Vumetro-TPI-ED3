/*
 * EINT1.c
 *
 *  Created on: 11 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "EINT1.h"

#include "max7219.h"
#include "ADC.h"
#include "timer.h"
volatile uint8_t flag_iniciar = 0;
void cfgEINT1(void)
{
    /* P2.11 -> EINT1 */

    LPC_PINCON->PINSEL4 &= ~(3 << 22);
    LPC_PINCON->PINSEL4 |=  (1 << 22);

    /* Flanco */
    LPC_SC->EXTMODE |= (1 << 1);

    /* Descendente */
    LPC_SC->EXTPOLAR |= (1 << 1);

    /* Limpia flag */
    LPC_SC->EXTINT |= (1 << 1);

    NVIC_EnableIRQ(EINT1_IRQn);
    return;
}

void EINT1_IRQHandler(void)
{
	cfgTMR0();
    cfgADC();

	flag_iniciar = 1;  // solo setea el flag


    LPC_SC->EXTINT |= (1 << 1);
    return;
}
