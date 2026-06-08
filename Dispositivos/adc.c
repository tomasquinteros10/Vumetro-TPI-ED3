//Un solo canal de ADC: El micrófono solo usa un pin (vamos a dejar P0.23 / AD0.0

/**
 * @file microfono_vu.c
 * @brief Implementación de lectura de micrófono por ADC para Vúmetro 16x8.
 *
 * Este módulo lee la señal analógica de un micrófono mediante el ADC0.0
 * y procesa la señal para ser representada en una matriz de 16x8.
 *
 * @date Junio 2026
 */

#include "LPC17xx.h"

/* === DEFINICIÓN DE PINES Y CONSTANTES === */

// Pin del ADC para el Micrófono:
#define PIN_MICROFONO       23      // AD0.0 (P0.23) -> Entrada de audio

// Parámetros de la señal y matriz
#define MUESTRAS_PROMEDIO   8       // Mayor sobremuestreo para estabilizar audio
#define COMPENSACION_DC     2048    // Silencio teórico (VCC / 2) si usás preamificador con bias
#define NIVELES_MATRIZ      8       // 8 niveles de altura para el vúmetro

/* === VARIABLES GLOBALES === */
static volatile uint16_t nivel_audio = 0;   // Nivel procesado actual (0 a 4095)
static uint16_t prev_audio = 0;             // Para filtro de suavizado

/* === FUNCIONES INTERNAS === */

/**
 * @brief Configura el ADC para lectura del micrófono.
 */
static void config_adc(void) {
    // 1. Encender el periférico ADC
    LPC_SC->PCONP |= (1 << 12);  // Bit PCADC en registro PCONP

    // 2. Configurar P0.23 como entrada analógica (AD0.0)
    LPC_PINCON->PINSEL1 &= ~(3 << 14);  // Limpiar config previa
    LPC_PINCON->PINSEL1 |=  (1 << 14);  // P0.23 como función ADC (01)

    // 3. Configurar el ADC:
    // - PDN = 1 (encendido)
    // - CLKDIV = 3 (Frecuencia más alta para audio, ajustala según tu PCLK)
    LPC_ADC->ADCR = (1 << 21) | (3 << 8);
}

/* === FUNCIONES PÚBLICAS === */

/**
 * @brief Inicializa el módulo de captura de audio.
 */
void microfono_inicializar(void) {
    config_adc();
}

/**
 * @brief Lee el ADC, procesa la señal de audio y calcula la amplitud.
 * Debe ser llamada idealmente desde un Timer a una tasa fija (ej. 44.1kHz o menos)
 * o en el lazo principal si no requiere criticidad temporal.
 */
void microfono_actualizar(void) {
    uint32_t temp;
    uint32_t sum_audio = 0;
    
    // Tomar múltiples muestras rápidas para capturar el pico de la onda
    for (uint8_t i = 0; i < MUESTRAS_PROMEDIO; i++) {
        // Seleccionar canal 0 (AD0.0) y arrancar conversión
        LPC_ADC->ADCR &= ~((0xFF) | (7 << 24));
        LPC_ADC->ADCR |= (1U << 0);  // Canal 0
        LPC_ADC->ADCR |= (1U << 24); // Start conversion NOW
        
        // Esperar fin de conversión (bit DONE)
        do { 
            temp = LPC_ADC->ADDR0; 
        } while (!(temp & (1U << 31)));
        
        uint16_t muestra_raw = (uint16_t)((temp >> 4) & 0xFFF);
        
        /* * PROCESAMIENTO DE AUDIO: 
         * Si tu micrófono tiene un módulo con salida flotante en VCC/2 (2048),
         * queremos la amplitud absoluta respecto al silencio.
         */
        int32_t amplitud = (int32_t)muestra_raw - COMPENSACION_DC;
        if (amplitud < 0) amplitud = -amplitud; // Rectificación de onda completa
        
        sum_audio += amplitude;
    }
    
    // Calcular promedio de las amplitudes capturadas
    uint16_t actual_audio = (uint16_t)(sum_audio / MUESTRAS_PROMEDIO);
    
    // Filtro pasabajos digital (suavizado de caída del vúmetro para que no parpadee brusco)
    // 70% valor anterior y 30% nuevo valor. Podés jugar con estos números.
    nivel_audio = (uint16_t)((prev_audio * 7 + actual_audio * 3) / 10);
    prev_audio = nivel_audio;
}

/**
 * @brief Retorna el valor crudo procesado de la amplitud (0 a 2048 aprox)
 */
uint16_t microfono_leer_amplitud(void) {
    return nivel_audio;
}

/**
 * @brief Mapea la lectura actual al número de renglones/filas a encender en la matriz.
 * @return uint8_t Valor entre 0 y NIVELES_MATRIZ (0 a 8)
 */
uint8_t microfono_obtener_filas_matriz(void) {
    // Como el valor máximo de amplitud absoluta desde el centro (2048) es ~2048:
    // Mapeamos de 0-2000 (para dar un margen de sensibilidad) a 0-8.
    
    uint32_t resultado = ((uint32_t)nivel_audio * NIVELES_MATRIZ) / 2000;
    
    if (resultado > NIVELES_MATRIZ) {
        resultado = NIVELES_MATRIZ; // Límite máximo (saturación del vúmetro)
    }
    
    return (uint8_t)resultado;
}

