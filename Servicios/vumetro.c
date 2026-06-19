/*
 * vumetro.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include "EINT1.h"
#include "vumetro.h"
#include "microfono.h"
#include "max7219.h"

#define VU_MAX_NIVEL 16

    uint8_t nivel;
    uint8_t fila;

    /* Imagen para las dos matrices 8x8 */
    uint8_t display[2][8];


/* Funciones privadas */
static float Convertir_mV_a_dB(uint16_t mv);
static uint8_t Convertir_dB_a_Nivel(float db);

/**
 * @brief Inicialización del módulo.
 */
void Vumetro_Init(void)
{
    /* Reservado para futuras inicializaciones */
}

/**
 * @brief Obtiene el nivel actual en dB.
 */
float Vumetro_GetdB(void)
{
    uint16_t mv;

    mv = Microfono_Leer_mV();

    return Convertir_mV_a_dB(mv);
}

/**
 * @brief Obtiene el nivel de barra (0 a 16).
 */
uint8_t Vumetro_GetNivel(void)
{
    float db;

    db = Vumetro_GetdB();

    return Convertir_dB_a_Nivel(db);
}

/**
 * @brief Conversión de milivoltios a decibeles.
 *
 * COMPLETAR CON TU TABLA.
 */

static float Convertir_mV_a_dB(uint16_t mv)
{
    if(mv < 120)
        return 0.0f;
    else if(mv < 160)
        return 68.0f;
    else if(mv < 200)
        return 70.2f;
    else if(mv < 240)
        return 73.0f;
    else if(mv < 320)
        return 75.0f;
    else if(mv < 400)
        return 78.0f;
    else if(mv < 500)
        return 83.0f;
    else if(mv < 680)
        return 86.0f;
    else if(mv < 960)
        return 89.0f;
    else if(mv < 1230)
        return 93.0f;
    else if(mv < 1700)
        return 97.0f;
    else
        return 99.0f;
}

	/**
	 * @brief Conversión de dB a nivel visual (0 a 16).
	 */
	static uint8_t Convertir_dB_a_Nivel(float db)
	{
	    if(db < 59.3f)
	    {
	        return 0;
	    }
	    else if(db < 61.0f)
	    {
	        return 1;
	    }
	    else if(db < 65.0f)
	    {
	        return 2;
	    }
	    else if(db < 78.0f)
	    {
	        return 8;
	    }
	    else if(db < 81.0f)
	    {
	        return 4;
	    }
	    else
	    {
	        return 16;
	    }
	}

void Mostrar_Valor(void){

	/* Obtiene nivel entre 0 y 16 */
	        nivel = Vumetro_GetNivel();

	        /* Apaga toda la matriz */
	        for(fila = 0; fila < 8; fila++)
	        {
	            display[0][fila] = 0x00;
	            display[1][fila] = 0x00;
	        }

	        /* Enciende las filas necesarias */
	        for(fila = 0; fila < nivel; fila++)
	        {
	            if(fila < 8)
	            {
	                /* Primer MAX7219 */
	                display[0][fila] = 0xFF;
	            }
	            else
	            {
	                /* Segundo MAX7219 */
	                display[1][fila - 8] = 0xFF;
	            }
	        }

	        /* Actualiza ambas matrices */
	        MAX7219_Display(display);

	        return;
}
