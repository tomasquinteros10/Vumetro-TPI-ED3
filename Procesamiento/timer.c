/*
 * timer.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */


#include "lpc17xx.h"
#include "timer.h"
#include "../../CMSISv2p00_LPC17xx/Drivers/inc/lpc17xx_timer.h"


void cfgTMR0(void)
{
    // Configuracion de Timer en Modo Temporizador
    TIM_TIMERCFG_Type cfgTimerMode;
    // Configuracion del Tiempo de Desborde de MATCH
    TIM_MATCHCFG_Type cfgTimerMatch;

    // Base de Tiempo del Prescaler a partir de valor en μs
    cfgTimerMode.PrescaleOption = TIM_PRESCALE_USVAL;
    // Base de Tiempo de Desborde de Prescaler [μs] (Time_Overflow_PC)
    cfgTimerMode.PrescaleValue = 1;     // 1 µs

    // Habilitacion de Canal de MATCH0: (MR1)
    cfgTimerMatch.MatchChannel = 1;
    // Valor Máximo para TC (MR)
    cfgTimerMatch.MatchValue = 1000;     // 1 ms => 1 kHz
    // Habilitacion de Interrupcion por MATCH: (MR1I=0)
    cfgTimerMatch.IntOnMatch = DISABLE;
    // Habilitacion de Reset por MATCH: (MR1R=1)
    cfgTimerMatch.ResetOnMatch = ENABLE;
    // Deshabilitacion de Stop por MATCH: (MR1S=0)
    cfgTimerMatch.StopOnMatch = DISABLE;
    // Modalidad TOGGLE en P1.29
    cfgTimerMatch.ExtMatchOutputType = TIM_EXTMATCH_TOGGLE;

    // Configuracion de la Modalidad del TIMER0
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &cfgTimerMode);
    // Configuracion del TIMER0 como Temporizador
    TIM_ConfigMatch(LPC_TIM0, &cfgTimerMatch);
    // Habilitacion del TIMER0
    TIM_Cmd(LPC_TIM0, ENABLE);

    // Deshabilitacion de Interrupcion por TIMER0
    NVIC_DisableIRQ(TIMER0_IRQn);
    return;
}
