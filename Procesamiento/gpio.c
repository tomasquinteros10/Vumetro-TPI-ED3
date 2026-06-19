/*
 * GPIO.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif


#include "GPIO.h"

void cfgGPIO(void)
{
	cfg_GPIO_MAX7219();
    cfgADC_Pin();

    return;
}

/*====================================================
 * MAX7219
 *===================================================*/

void cfg_GPIO_MAX7219(void)
{
    PINSEL_CFG_Type cfgMAX;
    PINSEL_CFG_Type cfgMAXPIN2;

    cfgMAXPIN2.Portnum   = PINSEL_PORT_0;
    cfgMAXPIN2.Funcnum   = PINSEL_FUNC_0;
    cfgMAX.Portnum   = PINSEL_PORT_0;
    cfgMAX.Funcnum   = PINSEL_FUNC_0;
    cfgMAX.Pinmode   = PINSEL_PINMODE_TRISTATE;
    cfgMAXPIN2.Pinmode   = PINSEL_PINMODE_PULLUP;
    cfgMAX.OpenDrain = PINSEL_PINMODE_NORMAL;
    cfgMAXPIN2.OpenDrain = PINSEL_PINMODE_NORMAL;

    /* DATA */

    cfgMAX.Pinnum = PINSEL_PIN_0;
    PINSEL_ConfigPin(&cfgMAX);

    /* CLOCK */

    cfgMAX.Pinnum = PINSEL_PIN_1;
    PINSEL_ConfigPin(&cfgMAX);

    /* LOAD */
    cfgMAXPIN2.Pinnum = PINSEL_PIN_2;
       PINSEL_ConfigPin(&cfgMAXPIN2);

    GPIO_SetDir(
    		PINSEL_PORT_0,
        MAX7219_DATA_MASK |
        MAX7219_CLOCK_MASK |
        MAX7219_LOAD_MASK,
        1);

    return;
 }

  void GPIO_MAX7219_DataAlto(void)
    {
        GPIO_SetValue(MAX7219_PORT, MAX7219_DATA_MASK);
    }

    void GPIO_MAX7219_DataBajo(void)
    {
        GPIO_ClearValue(MAX7219_PORT, MAX7219_DATA_MASK);
    }

    void GPIO_MAX7219_ClockAlto(void)
    {
        GPIO_SetValue(MAX7219_PORT, MAX7219_CLOCK_MASK);
    }

    void GPIO_MAX7219_ClockBajo(void)
    {
        GPIO_ClearValue(MAX7219_PORT, MAX7219_CLOCK_MASK);
    }

    void GPIO_MAX7219_LoadAlto(void)
    {
        GPIO_SetValue(MAX7219_PORT, MAX7219_LOAD_MASK);
    }

    void GPIO_MAX7219_LoadBajo(void)
    {
        GPIO_ClearValue(MAX7219_PORT, MAX7219_LOAD_MASK);
    }


/*====================================================
 * Entrada de Audio
 * P0.23 -> AD0.0
 *===================================================*/

void cfgADC_Pin(void)
{
    PINSEL_CFG_Type cfgADC;

    cfgADC.Portnum   = PINSEL_PORT_0;
    cfgADC.Pinnum    = PINSEL_PIN_23;
    cfgADC.Funcnum   = PINSEL_FUNC_1;
    cfgADC.Pinmode   = PINSEL_PINMODE_TRISTATE;
    cfgADC.OpenDrain = PINSEL_PINMODE_NORMAL;

    PINSEL_ConfigPin(&cfgADC);

    return;
}
