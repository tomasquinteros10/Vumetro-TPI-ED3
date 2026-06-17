/*
 * vumetro.h
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif


#ifndef VUMETRO_H_
#define VUMETRO_H_

#include <cr_section_macros.h>
#include <stdint.h>

/**
 * @brief Inicializa el módulo Vúmetro.
 */
void Vumetro_Init(void);

void Mostrar_Valor(void);

/**
 * @brief Devuelve el nivel del Vúmetro (0 a 16).
 */
uint8_t Vumetro_GetNivel(void);

/**
 * @brief Devuelve el valor actual en dB.
 */
float Vumetro_GetdB(void);


#endif /* VUMETRO_H_ */
