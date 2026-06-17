/*
 * microfono.h
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef MICROFONO_H_
#define MICROFONO_H_
#include "GPIO.h"


#include <stdint.h>
#include <cr_section_macros.h>

// Inicializa el ADC del micrófono
void Microfono_Init(void);

// Lee el valor actual del micrófono (ADC crudo)
uint16_t Microfono_Leer_mV(void);



#endif /* MICROFONO_H_ */
