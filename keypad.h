/**
 * @file keypad.h
 * @brief Interfaz del teclado matricial 4x4 mediante IOC.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include "config.h"

/**
 * @brief Codigo ASCII de la ultima tecla detectada.
 * @details Variable compartida entre la ISR y el programa principal.
 */
extern volatile uint8_t g_tecla_evento;

/**
 * @brief Bandera que indica la existencia de una tecla pendiente.
 * @details Vale 1 cuando la ISR registra una pulsacion.
 */
extern volatile uint8_t g_bandera_tecla;

/**
 * @brief Configura el puerto B y los registros IOC.
 * @details Mantiene las interrupciones deshabilitadas.
 */
void keypad_configurar(void);

/** @brief Habilita las interrupciones por cambio de PORTB. */
void keypad_habilitar_ioc(void);

/** @brief Restablece las filas y rearma la interrupcion IOC. */
void keypad_rearme(void);

#endif

