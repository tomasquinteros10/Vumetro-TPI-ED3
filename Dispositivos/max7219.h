/*
 * MAX7219.h
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef MAX7219_H_
#define MAX7219_H_

/*====================================================
 * max7219.h
 *===================================================*/
#include "GPIO.h"
#include <stdint.h>
#include <cr_section_macros.h>

#define MAX7219_REG_NOP            0x00  // No realiza ninguna operación (usado en daisy-chain)

#define MAX7219_REG_DIGIT0         0x01  // Registro asociado a la fila 0
#define MAX7219_REG_DIGIT1         0x02  // Registro asociado a la fila 1
#define MAX7219_REG_DIGIT2         0x03  // Registro asociado a la fila 2
#define MAX7219_REG_DIGIT3         0x04  // Registro asociado a la fila 3
#define MAX7219_REG_DIGIT4         0x05  // Registro asociado a la fila 4
#define MAX7219_REG_DIGIT5         0x06  // Registro asociado a la fila 5
#define MAX7219_REG_DIGIT6         0x07  // Registro asociado a la fila 6
#define MAX7219_REG_DIGIT7         0x08  // Registro asociado a la fila 7

#define MAX7219_REG_DECODE_MODE    0x09  // Configura el modo de decodificación
#define MAX7219_REG_INTENSITY      0x0A  // Configura el brillo de la matriz
#define MAX7219_REG_SCAN_LIMIT     0x0B  // Configura cuántas filas se escanean
#define MAX7219_REG_SHUTDOWN       0x0C  // Control de encendido/apagado
#define MAX7219_REG_DISPLAY_TEST   0x0F  // Enciende todos los LEDs para prueba

#define MAX7219_DECODE_NONE        0x00  // Sin decodificación BCD (modo matriz LED)

#define MAX7219_SHUTDOWN_OFF       0x00  // Display apagado
#define MAX7219_SHUTDOWN_ON        0x01  // Funcionamiento normal

#define MAX7219_SCAN_ALL           0x07  // Habilita las 8 filas de la matriz

#define MAX7219_INTENSITY_MIN      0x00  // Brillo mínimo
#define MAX7219_INTENSITY_HALF     0x07  // Brillo medio
#define MAX7219_INTENSITY_MAX      0x0F  // Brillo máximo

#define MAX7219_NUM_DEVICES        2     // Dos matrices 8x8 encadenadas (16x8)

/*====================================================
 * PROTOTIPOS
 *===================================================*/

void cfgMAX7219(void);                   // Inicializa ambos MAX7219

void MAX7219_SendToDevice(uint8_t device,// Envía un comando a un MAX7219 específico
                          uint8_t reg,
                          uint8_t data);

void MAX7219_SendToAll(uint8_t reg,      // Envía un comando a todos los MAX7219
                       uint8_t data);

void MAX7219_SetRow(uint8_t device,      // Actualiza una fila completa
                    uint8_t row,
                    uint8_t data);

void MAX7219_Clear(void);                // Apaga todos los LEDs

void MAX7219_SetIntensity(uint8_t intensity); // Modifica el brillo

void MAX7219_Display(uint8_t display[MAX7219_NUM_DEVICES][8]); // Muestra una imagen completa




#endif /* MAX7219_H_ */
