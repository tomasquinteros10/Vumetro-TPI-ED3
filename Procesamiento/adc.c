/*
 * ADC.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */


#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include "ADC.h"

#include "lpc17xx.h"

#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_adc.h"

volatile uint32_t ADC0Value_AD = 0;
volatile uint32_t ADC0Value_mV = 0;

void cfgADC(){
	ADC_Init(LPC_ADC,100000);//Frecuencia?
	ADC_BurstCmd(LPC_ADC,DISABLE); //No quiero modo burst, porque voy a usar un timer
	///*** Inicializacion de Conversión debido a MAT0.1: (START=100) ***
	ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
	///*** Inicializacion de Conversión ante Flanco Descendente debido a MAT0.1: (EDGE=1) ***
	ADC_EdgeStartConfig(LPC_ADC, ADC_START_ON_FALLING);
	///*** Funcionalidad de P0.23 como AD0.0 (Canal 0) ***
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
	///*** Habilitación de Interrupción por ADC0 ***
	NVIC_EnableIRQ(ADC_IRQn);

	    return;

}

void ADC_IRQHandler(void){
    //*** Espera Fin de Conversión de AD0.0: DONE_CH0 = 1? ***
    while(!(ADC_ChannelGetStatus(LPC_ADC, ADC_CHANNEL_0, ADC_DATA_DONE))){};
    // while(!(LPC_ADC->ADDR0 & (1<<31))){}

    //*** Guarda Dato Digitalizado de AD0.0 (12 bits) en Cuentas de AD ***
    ADC0Value_AD = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
    // ADC0Value_AD = (LPC_ADC->ADDR0)>>4 & 0xFFFF;

    //*** Acondicionamiento del Dato Digitalizado en mV ***
    ADC0Value_mV = (uint32_t)(ADC0Value_AD * 3300) / 4095;
    return;

}
