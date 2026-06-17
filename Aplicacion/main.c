/*
 * main.c
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif


#include "GPIO.h"
#include "timer.h"
#include "ADC.h"
#include "MAX7219.h"
#include "vumetro.h"
#include "EINT0.h"
#include "EINT1.h"
//extern void SystemInit(void);
int main(void)
{

    /*=============================
     * Inicializaciones
     *============================*/

    //SystemInit();
    cfgGPIO();


    Vumetro_Init();
    cfgEINT1();
    cfgEINT0();
    cfgMAX7219();
    flag_iniciar = 0;
    while(1)
    {

        Mostrar_Valor();
    }

    return 0;
}

