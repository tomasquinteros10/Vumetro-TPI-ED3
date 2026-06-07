#ifndef MAX7219_H
#define MAX7219_H

// =============================================================
// max7219.h - Capa de Dispositivos: MAX7219
//
// Maneja el chip controlador de matrices LED MAX7219.
// Comunicacion por bit-banging SPI usando los pines GPIO.
//
// PROTOCOLO MAX7219:
// - Cada transferencia es de 16 bits: [7:0] dato, [15:8] direccion
// - CS debe estar en bajo durante toda la transferencia
// - Los datos se envian MSB primero
// - El MAX7219 lee el dato en el flanco ascendente del CLK
//
// DAISY-CHAIN (dos matrices encadenadas):
// - Para escribir en ambas: mandar 32 bits (16 para cada una)
//   primero los datos de la segunda, luego los de la primera
// - Para escribir en una sola: mandar 16 bits reales +
//   16 bits de NOP para la otra
//
// REGISTROS DEL MAX7219:
//   0x00 -> NOP (no hace nada, usado en daisy-chain)
//   0x01 -> Digito 0 (fila 1 de la matriz)
//   ...
//   0x08 -> Digito 7 (fila 8 de la matriz)
//   0x09 -> Modo decodificacion (0x00 = sin decodificacion para matriz)
//   0x0A -> Intensidad (0x00 minimo ... 0x0F maximo)
//   0x0B -> Scan limit (0x07 = mostrar los 8 digitos/filas)
//   0x0C -> Shutdown (0x00 = apagado, 0x01 = normal)
//   0x0F -> Display test (0x01 = todos los LEDs encendidos)
// =============================================================

#include <stdint.h>

// =============================================================
// REGISTROS DEL MAX7219
// =============================================================
#define MAX7219_REG_NOP          0x00  // no operacion (daisy-chain)
#define MAX7219_REG_DIGIT0       0x01  // fila 1
#define MAX7219_REG_DIGIT1       0x02  // fila 2
#define MAX7219_REG_DIGIT2       0x03  // fila 3
#define MAX7219_REG_DIGIT3       0x04  // fila 4
#define MAX7219_REG_DIGIT4       0x05  // fila 5
#define MAX7219_REG_DIGIT5       0x06  // fila 6
#define MAX7219_REG_DIGIT6       0x07  // fila 7
#define MAX7219_REG_DIGIT7       0x08  // fila 8
#define MAX7219_REG_DECODE_MODE  0x09  // modo decodificacion
#define MAX7219_REG_INTENSITY    0x0A  // brillo (0x00 a 0x0F)
#define MAX7219_REG_SCAN_LIMIT   0x0B  // cuantas filas mostrar
#define MAX7219_REG_SHUTDOWN     0x0C  // encendido/apagado
#define MAX7219_REG_DISPLAY_TEST 0x0F  // test (enciende todo)

// =============================================================
// VALORES UTILES
// =============================================================
#define MAX7219_SHUTDOWN_OFF     0x00  // apagar display
#define MAX7219_SHUTDOWN_ON      0x01  // encender display (normal)
#define MAX7219_DECODE_NONE      0x00  // sin decodificacion BCD
#define MAX7219_SCAN_ALL         0x07  // mostrar las 8 filas
#define MAX7219_INTENSITY_MAX    0x0F  // brillo maximo
#define MAX7219_INTENSITY_HALF   0x07  // brillo medio
#define MAX7219_INTENSITY_MIN    0x00  // brillo minimo

// Numero de matrices encadenadas
#define MAX7219_NUM_DEVICES      2

// =============================================================
// PROTOTIPOS
// =============================================================

/**
 * @brief Inicializa los dos MAX7219 encadenados.
 *        Configura los GPIO, apaga el modo test, desactiva
 *        decodificacion BCD, habilita las 8 filas, configura
 *        el brillo y activa el display.
 *        Limpia la matriz (todos los LEDs apagados).
 */
void MAX7219_Init(void);

/**
 * @brief Envia un comando de 16 bits a UN MAX7219 especifico.
 *        Manda NOP al otro para no afectarlo.
 *
 * @param device  Numero de dispositivo (0 = primero, 1 = segundo).
 * @param reg     Registro del MAX7219 (MAX7219_REG_xxx).
 * @param data    Dato a escribir en el registro.
 */
void MAX7219_SendToDevice(uint8_t device, uint8_t reg, uint8_t data);

/**
 * @brief Envia el mismo comando a AMBOS MAX7219 simultaneamente.
 *        Util para configuracion inicial (brillo, scan limit, etc).
 *
 * @param reg   Registro del MAX7219 (MAX7219_REG_xxx).
 * @param data  Dato a escribir en el registro.
 */
void MAX7219_SendToAll(uint8_t reg, uint8_t data);

/**
 * @brief Actualiza una fila completa de una matriz especifica.
 *
 * @param device  Numero de dispositivo (0 o 1).
 * @param row     Fila a actualizar (0 a 7).
 * @param data    Patron de bits para la fila (1 = LED encendido).
 */
void MAX7219_SetRow(uint8_t device, uint8_t row, uint8_t data);

/**
 * @brief Limpia todas las filas de ambas matrices (apaga todos los LEDs).
 */
void MAX7219_Clear(void);

/**
 * @brief Establece el brillo de ambas matrices.
 *
 * @param intensity Valor de 0x00 (minimo) a 0x0F (maximo).
 */
void MAX7219_SetIntensity(uint8_t intensity);

/**
 * @brief Muestra un patron completo en las dos matrices.
 *        Cada elemento del array es una fila de la matriz.
 *        display[0][0..7] = filas de la primera matriz
 *        display[1][0..7] = filas de la segunda matriz
 *
 * @param display Array [2][8] con el patron a mostrar.
 */
void MAX7219_Display(uint8_t display[MAX7219_NUM_DEVICES][8]);

#endif // MAX7219_H