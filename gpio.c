// =============================================================
// gpio.c - Capa de Dispositivos: GPIO
//
// Configura P0.0 (DATA), P0.1 (CLK) y P0.2 (CS) como salidas
// digitales simples para bit-banging con el MAX7219.
//
// Por que PINSEL_FUNC_00?
//   Porque queremos GPIO puro. Si pusieramos FUNC_01/02/03
//   el pin quedaria asignado a un periferico (SPI, UART, etc)
//   y no podriamos controlarlo manualmente.
//
// Por que TRISTATE?
//   Porque estos pines son SALIDAS. El pull-up/pull-down solo
//   importa en entradas (para definir el nivel cuando nadie
//   maneja el pin). Como salida, el micro maneja el nivel solo.
//
// Por que no open-drain?
//   Open-drain solo puede poner el pin en bajo o dejarlo flotando.
//   Necesitamos poder poner el pin en alto Y en bajo -> normal.
// =============================================================

#include "gpio.h"

void GPIO_MAX7219_Init(void)
{
    PINSEL_CFG_T pinCfg;
    pinCfg.port      = GPIO_MAX_PORT;
    pinCfg.func      = PINSEL_FUNC_00;  // GPIO, sin funcion alternativa
    pinCfg.mode      = PINSEL_TRISTATE; // salida: no necesita pull
    pinCfg.openDrain = DISABLE;         // salida normal

    // Configurar DATA
    pinCfg.pin = GPIO_DATA_PIN;
    PINSEL_ConfigPin(&pinCfg);

    // Configurar CLK
    pinCfg.pin = GPIO_CLK_PIN;
    PINSEL_ConfigPin(&pinCfg);

    // Configurar CS
    pinCfg.pin = GPIO_CS_PIN;
    PINSEL_ConfigPin(&pinCfg);

    // Todos como salidas digitales
    GPIO_SetDir(GPIO_MAX_PORT,
                GPIO_DATA_MASK | GPIO_CLK_MASK | GPIO_CS_MASK,
                GPIO_OUTPUT);

    // Estado inicial seguro:
    GPIO_SetPins  (GPIO_MAX_PORT, GPIO_CS_MASK);   // CS alto = inactivo
    GPIO_ClearPins(GPIO_MAX_PORT, GPIO_CLK_MASK);  // CLK bajo = reposo
    GPIO_ClearPins(GPIO_MAX_PORT, GPIO_DATA_MASK); // DATA bajo = reposo
}

// Wrappers simples para que max7219.c sea mas legible
void GPIO_DATA_High(void) { GPIO_SetPins  (GPIO_MAX_PORT, GPIO_DATA_MASK); }
void GPIO_DATA_Low(void)  { GPIO_ClearPins(GPIO_MAX_PORT, GPIO_DATA_MASK); }
void GPIO_CLK_High(void)  { GPIO_SetPins  (GPIO_MAX_PORT, GPIO_CLK_MASK);  }
void GPIO_CLK_Low(void)   { GPIO_ClearPins(GPIO_MAX_PORT, GPIO_CLK_MASK);  }
void GPIO_CS_High(void)   { GPIO_SetPins  (GPIO_MAX_PORT, GPIO_CS_MASK);   }
void GPIO_CS_Low(void)    { GPIO_ClearPins(GPIO_MAX_PORT, GPIO_CS_MASK);   }