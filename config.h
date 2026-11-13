/**
 * @file config.h
 * @brief Configuracion general del sistema de cifrado PIC16F887.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#include <stdint.h>

/** @brief Frecuencia del cristal externo de 4 MHz. */
#define _XTAL_FREQ 4000000UL

/* LCD HD44780: RS=RD0, E=RD1, D4-D7=RD4-RD7, RW a GND. */
#define LCD_RS PORTDbits.RD0
#define LCD_EN PORTDbits.RD1
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7

#define LCD_RS_TRIS TRISDbits.TRISD0
#define LCD_EN_TRIS TRISDbits.TRISD1
#define LCD_D4_TRIS TRISDbits.TRISD4
#define LCD_D5_TRIS TRISDbits.TRISD5
#define LCD_D6_TRIS TRISDbits.TRISD6
#define LCD_D7_TRIS TRISDbits.TRISD7

/** @brief Numero de columnas de la LCD. */
#define LCD_COLUMNAS 16u
/** @brief Numero de filas de la LCD. */
#define LCD_FILAS 2u

/* Teclado 4x4: filas RB0-RB3, columnas RB4-RB7. */
#define KEYPAD_PORT PORTB
#define KEYPAD_TRIS TRISB
#define KEYPAD_WPUB WPUB
#define KEYPAD_IOCB IOCB

/** @brief Mascara de las cuatro filas RB0-RB3. */
#define KEYPAD_ROW_MASK 0x0Fu
/** @brief Mascara de las cuatro columnas RB4-RB7. */
#define KEYPAD_COLUMN_MASK 0xF0u
/** @brief Numero de filas del teclado. */
#define KEYPAD_FILAS 4u
/** @brief Numero de columnas del teclado. */
#define KEYPAD_COLUMNAS 4u

#endif
