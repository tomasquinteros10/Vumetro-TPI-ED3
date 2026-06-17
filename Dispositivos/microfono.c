/*
 * microfono.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */


#include "microfono.h"
#include "ADC.h"

#define CANAL_MICROFONO 0   // AD0.0 por ejemplo

void Microfono_Init(void)
{
	cfgADC();  // inicializa el periférico ADC
}

uint16_t Microfono_Leer_mV(void)
{
    return (uint16_t)ADC0Value_mV;
}
