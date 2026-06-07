#ifndef GPIO_H
#define GPIO_H

// =============================================================
// gpio.h - Capa de Dispositivos: GPIO
//
// Configura y controla los pines GPIO usados para comunicacion
// bit-banging con el MAX7219.
//
// PINES USADOS:
//   P0.0 -> DATA: envia los bits al MAX7219 (un bit a la vez)
//   P0.1 -> CLK:  reloj de sincronismo (MAX7219 lee en flanco alto)
//   P0.2 -> CS:   chip select (bajo = escucha, alto = aplica dato)
//
// CONFIGURACION:
//   Funcion: PINSEL_FUNC_00 = GPIO puro (sin funcion alternativa)
//   Modo:    TRISTATE (sin pull-up/down, somos salida)
//   Tipo:    salida digital normal (no open-drain)
// =============================================================

#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

// Pines fisicos usados
#define GPIO_MAX_PORT   PORT_0
#define GPIO_DATA_PIN   PIN_0
#define GPIO_DATA_MASK  (1 << 0)
#define GPIO_CLK_PIN    PIN_1
#define GPIO_CLK_MASK   (1 << 1)
#define GPIO_CS_PIN     PIN_2
#define GPIO_CS_MASK    (1 << 2)

// Prototipos
void GPIO_MAX7219_Init(void);
void GPIO_DATA_High(void);
void GPIO_DATA_Low(void);
void GPIO_CLK_High(void);
void GPIO_CLK_Low(void);
void GPIO_CS_High(void);
void GPIO_CS_Low(void);

#endif
