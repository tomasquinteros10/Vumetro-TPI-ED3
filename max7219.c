// =============================================================
// max7219.c - Capa de Dispositivos: MAX7219
//
// PROTOCOLO SPI DEL MAX7219 (bit-banging):
//
// Para mandar 1 paquete de 16 bits:
//
//   CS bajo          -> MAX7219 empieza a escuchar
//
//   Para cada bit del 15 al 0:
//     DATA = ese bit (alto o bajo)
//     esperar 1us    -> DATA estable antes del CLK (setup time)
//     CLK alto       -> MAX7219 LEE el bit en este momento
//     esperar 1us    -> CLK alto minimo 50ns (datasheet)
//     CLK bajo       -> preparar proximo bit
//     esperar 1us    -> CLK bajo minimo 50ns
//
//   CS alto          -> MAX7219 APLICA el dato recibido (latch)
//
// Visualmente:
//
//   CS   __|__________________________|‾‾
//          ↑ baja                    ↑ sube = MAX7219 aplica
//
//   CLK  __|‾|_|‾|_|‾|_  ...  _|‾|_|
//            ↑ lee DATA en cada flanco ascendente
//
//   DATA [b15][b14][b13]...    [b1][b0]
//         dir(8 bits)          dato(8 bits)
//
// DAISY-CHAIN (dos MAX7219 encadenados):
//
//   LPC1769 -> MAX7219 #0 -> MAX7219 #1
//               DIN  DOUT    DIN
//
//   Los bits entran por el #0 y se desplazan hacia el #1.
//   Al subir CS, cada MAX7219 aplica los 16 bits que tiene.
//
//   Para escribir en device=0:
//     mandar primero: NOP  (termina en el #1, no hace nada)
//     mandar despues: dato (termina en el #0, se aplica)
//
//   Para escribir en device=1:
//     mandar primero: dato (termina en el #1, se aplica)
//     mandar despues: NOP  (termina en el #0, no hace nada)
// =============================================================

#include "max7219.h"
#include "gpio.h"

// =============================================================
// DELAY
// Con CCLK=80MHz cada iteracion del loop toma ~3 ciclos = ~37ns
// Para 1us necesitamos ~27 iteraciones
// Usamos volatile para que el compilador no lo optimice
// =============================================================
static void delay_us(uint32_t us)
{
    volatile uint32_t i;
    for (i = 0; i < (us * 27); i++);
}

// =============================================================
// MAX7219_SendByte - envia 8 bits MSB primero
//
// MSB primero significa: bit 7, bit 6, ... bit 1, bit 0
// El MAX7219 espera recibir los bits en este orden.
//
// Para cada bit:
//   1. Ponemos DATA segun el valor del bit
//   2. Esperamos que DATA este estable (setup time)
//   3. CLK alto -> MAX7219 lee el bit
//   4. Esperamos (CLK alto minimo 50ns segun datasheet)
//   5. CLK bajo -> preparar proximo bit
//   6. Esperamos (CLK bajo minimo 50ns)
// =============================================================
static void MAX7219_SendByte(uint8_t byte)
{
    int8_t i;
    for (i = 7; i >= 0; i--)
    {
        // Poner DATA segun el bit actual
        if (byte & (1 << i))
            GPIO_DATA_High();
        else
            GPIO_DATA_Low();

        delay_us(1);     // DATA debe estar estable antes del CLK

        GPIO_CLK_High(); // flanco ascendente -> MAX7219 lee DATA
        delay_us(1);     // CLK alto minimo (datasheet: 50ns, usamos 1us)

        GPIO_CLK_Low();  // bajar CLK, preparar proximo bit
        delay_us(1);     // CLK bajo minimo (datasheet: 50ns, usamos 1us)
    }
}

// =============================================================
// MAX7219_SendPacket - envia 16 bits (registro + dato)
// NO maneja CS, lo hace el llamador para poder encadenar varios
// paquetes en una sola transferencia
// =============================================================
static void MAX7219_SendPacket(uint8_t reg, uint8_t data)
{ //reg= 0x01-0x07 data= 0xFF 10000000 y se te prende la fila 
    MAX7219_SendByte(reg);  // primero el registro (direccion)
    MAX7219_SendByte(data); // luego el dato
}

// =============================================================
// MAX7219_Init
// Secuencia de inicializacion para ambos MAX7219.
// =============================================================
void MAX7219_Init(void)
{
    GPIO_MAX7219_Init(); // inicializar pines GPIO

    // Apagar modo test (enciende todos los LEDs, util para debug)
    MAX7219_SendToAll(MAX7219_REG_DISPLAY_TEST, 0x00);

    // Shutdown mode: apagar mientras configuramos
    MAX7219_SendToAll(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_OFF);

    // Sin decodificacion BCD: cada bit controla 1 LED directamente
    // Si activaramos decodificacion, el chip interpretaria los datos
    // como digitos 0-9 para display de 7 segmentos
    MAX7219_SendToAll(MAX7219_REG_DECODE_MODE, MAX7219_DECODE_NONE);

    // Scan limit: mostrar las 8 filas (0x07 = filas 0 a 7)
    MAX7219_SendToAll(MAX7219_REG_SCAN_LIMIT, MAX7219_SCAN_ALL);

    // Brillo inicial: medio para no encandilarse
    MAX7219_SendToAll(MAX7219_REG_INTENSITY, MAX7219_INTENSITY_HALF);

    // Limpiar la matriz antes de encender
    MAX7219_Clear();

    // Salir de shutdown: encender display
    MAX7219_SendToAll(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_ON);
}

// =============================================================
// MAX7219_SendToDevice
// Escribe en UN dispositivo especifico, NOP al otro.
//
// Recordar el flujo de bits en daisy-chain:
//   Lo que mandamos PRIMERO termina en el dispositivo #1
//   Lo que mandamos DESPUES termina en el dispositivo #0
// =============================================================
void MAX7219_SendToDevice(uint8_t device, uint8_t reg, uint8_t data)
{
    GPIO_CS_Low(); // CS bajo: ambos empiezan a escuchar

    if (device == 0)
    {
        // Escribir en #0: NOP al #1 (primero), dato al #0 (despues)
        MAX7219_SendPacket(MAX7219_REG_NOP, 0x00); // va al #1, no hace nada
        MAX7219_SendPacket(reg, data);              // va al #0, se aplica
    }
    else
    {
        // Escribir en #1: dato al #1 (primero), NOP al #0 (despues)
        MAX7219_SendPacket(reg, data);              // va al #1, se aplica
        MAX7219_SendPacket(MAX7219_REG_NOP, 0x00); // va al #0, no hace nada
    }

    GPIO_CS_High(); // CS alto: ambos aplican sus datos
}

// =============================================================
// MAX7219_SendToAll
// Manda el mismo comando a ambos en una sola transferencia.
// 32 bits en total: 16 para el #1 y 16 para el #0.
// =============================================================
void MAX7219_SendToAll(uint8_t reg, uint8_t data)
{
    GPIO_CS_Low();
    MAX7219_SendPacket(reg, data); // estos 16 bits terminan en el #1
    MAX7219_SendPacket(reg, data); // estos 16 bits terminan en el #0
    GPIO_CS_High();
}

// =============================================================
// MAX7219_SetRow
// Actualiza una fila de una matriz especifica.
// row 0 = fila superior, row 7 = fila inferior
// data: cada bit = 1 LED (bit7=izq, bit0=der)
// =============================================================
void MAX7219_SetRow(uint8_t device, uint8_t row, uint8_t data)
{
    uint8_t reg = MAX7219_REG_DIGIT0 + row; // DIGIT0=fila0, DIGIT1=fila1...
    MAX7219_SendToDevice(device, reg, data);
}

// =============================================================
// MAX7219_Clear
// Apaga todos los LEDs escribiendo 0x00 en las 8 filas
// de ambos dispositivos.
// Aprovechamos el daisy-chain para hacerlo de a 2 filas
// por transferencia (una de cada dispositivo).
// =============================================================
void MAX7219_Clear(void)
{
    uint8_t row;
    for (row = 0; row < 8; row++)
    {
        GPIO_CS_Low();
        MAX7219_SendPacket(MAX7219_REG_DIGIT0 + row, 0x00); // fila del #1
        MAX7219_SendPacket(MAX7219_REG_DIGIT0 + row, 0x00); // fila del #0
        GPIO_CS_High();
    }
}

// =============================================================
// MAX7219_SetIntensity
// Cambia el brillo de ambas matrices.
// =============================================================
void MAX7219_SetIntensity(uint8_t intensity)
{
    MAX7219_SendToAll(MAX7219_REG_INTENSITY, intensity);
}

// =============================================================
// MAX7219_Display
// Actualiza toda la matriz de una vez.
// display[0][0..7] = 8 filas del dispositivo 0
// display[1][0..7] = 8 filas del dispositivo 1
//
// Para cada fila mandamos ambos dispositivos juntos
// aprovechando el daisy-chain -> mas eficiente.
// =============================================================
void MAX7219_Display(uint8_t display[MAX7219_NUM_DEVICES][8])
{
    uint8_t row;
    for (row = 0; row < 8; row++)
    {
        uint8_t reg = MAX7219_REG_DIGIT0 + row;

        GPIO_CS_Low();
        MAX7219_SendPacket(reg, display[1][row]); // primero = va al #1
        MAX7219_SendPacket(reg, display[0][row]); // despues = va al #0
        GPIO_CS_High();
    }
}