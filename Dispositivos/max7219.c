/*
 * MAX7219.c
 *
 *  Created on: 10 jun. 2026
 *      Author: VICTUS
 */


/*====================================================
 * max7219.c
 * PUNTO C
 *===================================================*/
#include "LPC17xx.h"
#include "max7219.h"
#include "GPIO.h"

static void delay_us(uint32_t us)
{
    volatile uint32_t i;                 // Evita que el compilador optimice el bucle

    for(i = 0 ; i < us * 27 ; i++);      // Aproximadamente 1us para CCLK=80MHz
}

static void MAX7219_SendByte(uint8_t byte)
{
    int8_t i;                            // Índice para recorrer los bits

    for(i = 7 ; i >= 0 ; i--)            // Recorre desde el MSB al LSB
    {
        if(byte & (1 << i))              // Verifica si el bit actual vale 1
        {
            GPIO_MAX7219_DataAlto();     // Coloca DATA en nivel alto
        }
        else
        {
            GPIO_MAX7219_DataBajo();     // Coloca DATA en nivel bajo
        }

        delay_us(1);                     // Tiempo de setup para DATA

        GPIO_MAX7219_ClockAlto();        // Flanco ascendente: MAX7219 captura DATA

        delay_us(1);                     // Mantiene CLOCK alto

        GPIO_MAX7219_ClockBajo();        // Finaliza el pulso de reloj

        delay_us(1);                     // Tiempo mínimo antes del próximo bit
    }
}

static void MAX7219_SendPacket(uint8_t reg,
                               uint8_t data)
{
    MAX7219_SendByte(reg);               // Envía dirección del registro

    MAX7219_SendByte(data);              // Envía dato asociado al registro
}

/*====================================================
 * PUNTO H
 *===================================================*/

void cfgMAX7219(void)
{
    MAX7219_SendToAll(MAX7219_REG_DISPLAY_TEST,
                      0x00);             // Desactiva el modo test

    MAX7219_SendToAll(MAX7219_REG_SHUTDOWN,
                      MAX7219_SHUTDOWN_OFF); // Apaga temporalmente el display

    MAX7219_SendToAll(MAX7219_REG_DECODE_MODE,
                      MAX7219_DECODE_NONE);  // Modo matriz LED (sin BCD)

    MAX7219_SendToAll(MAX7219_REG_SCAN_LIMIT,
                      MAX7219_SCAN_ALL);     // Habilita las 8 filas

    MAX7219_SendToAll(MAX7219_REG_INTENSITY,
                      MAX7219_INTENSITY_HALF); // Brillo medio

    MAX7219_Clear();                     // Borra todas las filas de ambas matrices

    MAX7219_SendToAll(MAX7219_REG_SHUTDOWN,
                      MAX7219_SHUTDOWN_ON);   // Activa el funcionamiento normal
}

void MAX7219_SendToDevice(uint8_t device,
                          uint8_t reg,
                          uint8_t data)
{
    GPIO_MAX7219_LoadBajo();             // Comienza la transferencia

    if(device == 0)                      // Si se quiere escribir en el MAX7219 #0
    {
        MAX7219_SendPacket(MAX7219_REG_NOP,
                           0x00);        // NOP para el MAX7219 #1

        MAX7219_SendPacket(reg,
                           data);        // Comando real para el MAX7219 #0
    }
    else                                 // Si se quiere escribir en el MAX7219 #1
    {
        MAX7219_SendPacket(reg,
                           data);        // Comando real para el MAX7219 #1

        MAX7219_SendPacket(MAX7219_REG_NOP,
                           0x00);        // NOP para el MAX7219 #0
    }

    GPIO_MAX7219_LoadAlto();             // Aplica los datos enviados
}

void MAX7219_SendToAll(uint8_t reg,
                       uint8_t data)
{
    GPIO_MAX7219_LoadBajo();             // Comienza la transferencia

    MAX7219_SendPacket(reg,
                       data);            // Datos destinados al MAX7219 #1

    MAX7219_SendPacket(reg,
                       data);            // Datos destinados al MAX7219 #0

    GPIO_MAX7219_LoadAlto();             // Ambos MAX7219 actualizan simultáneamente
}

void MAX7219_SetRow(uint8_t device,
                    uint8_t row,
                    uint8_t data)
{
    uint8_t reg;

    reg = MAX7219_REG_DIGIT0 + row;      // Convierte fila 0..7 en registro DIGIT0..DIGIT7

    MAX7219_SendToDevice(device,
                         reg,
                         data);          // Actualiza la fila seleccionada
}

void MAX7219_Clear(void)
{
    uint8_t row;

    for(row = 0 ; row < 8 ; row++)       // Recorre las 8 filas
    {
        GPIO_MAX7219_LoadBajo();         // Comienza transferencia

        MAX7219_SendPacket(MAX7219_REG_DIGIT0 + row,
                           0x00);        // Borra fila del MAX7219 #1

        MAX7219_SendPacket(MAX7219_REG_DIGIT0 + row,
                           0x00);        // Borra fila del MAX7219 #0

        GPIO_MAX7219_LoadAlto();         // Aplica cambios
    }
}

void MAX7219_SetIntensity(uint8_t intensity)
{
    MAX7219_SendToAll(MAX7219_REG_INTENSITY,
                      intensity);        // Actualiza brillo de ambas matrices
}

void MAX7219_Display(uint8_t display[MAX7219_NUM_DEVICES][8])
{
    uint8_t row;
    uint8_t reg;

    for(row = 0 ; row < 8 ; row++)       // Recorre las 8 filas de la imagen
    {
        reg = MAX7219_REG_DIGIT0 + row;  // Obtiene el registro correspondiente

        GPIO_MAX7219_LoadBajo();         // Comienza transferencia

        MAX7219_SendPacket(reg,
                           display[1][row]); // Datos para el MAX7219 #1

        MAX7219_SendPacket(reg,
                           display[0][row]); // Datos para el MAX7219 #0

        GPIO_MAX7219_LoadAlto();         // Actualiza ambas matrices simultáneamente
    }
}
