/**
 * @file lcd.h
 * @brief Declaraciones del controlador LCD HD44780.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#ifndef LCD_H
#define LCD_H

#include "config.h"

/** @brief Inicializa la LCD en modo de cuatro bits. */
void lcd_init(void);

/**
 * @brief Envia un comando a la LCD.
 * @param comando Codigo de instruccion.
 */
void lcd_command(uint8_t comando);

/**
 * @brief Escribe un caracter en la LCD.
 * @param caracter Caracter ASCII que se desea mostrar.
 */
void lcd_putc(char caracter);

/**
 * @brief Escribe una cadena terminada en nulo.
 * @param texto Puntero a la cadena.
 */
void lcd_puts(const char *texto);

/**
 * @brief Posiciona el cursor de la LCD.
 * @param fila Fila de destino (0 o 1).
 * @param columna Columna de destino (0 a 15).
 */
void lcd_goto(uint8_t fila, uint8_t columna);

/** @brief Limpia completamente la pantalla. */
void lcd_clear(void);

#endif
