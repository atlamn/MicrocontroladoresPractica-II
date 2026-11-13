/**
 * @file lcd.c
 * @brief Implementacion del controlador LCD HD44780 en cuatro bits.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#include "lcd.h"

/**
 * @brief Envia un nibble de cuatro bits a la LCD.
 * @param nibble Valor de cuatro bits que se enviara.
 */
static void lcd_enviar_nibble(uint8_t nibble)
{
    LCD_D4 = (nibble >> 0u) & 0x01u;
    LCD_D5 = (nibble >> 1u) & 0x01u;
    LCD_D6 = (nibble >> 2u) & 0x01u;
    LCD_D7 = (nibble >> 3u) & 0x01u;

    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(50);
}

/**
 * @brief Inicializa el controlador LCD.
 */
void lcd_init(void)
{
    LCD_RS_TRIS = 0;
    LCD_EN_TRIS = 0;
    LCD_D4_TRIS = 0;
    LCD_D5_TRIS = 0;
    LCD_D6_TRIS = 0;
    LCD_D7_TRIS = 0;

    LCD_RS = 0;
    LCD_EN = 0;

    __delay_ms(20);

    /* Secuencia de inicializacion de cuatro bits. */
    lcd_enviar_nibble(0x03);
    __delay_ms(5);
    lcd_enviar_nibble(0x03);
    __delay_us(150);
    lcd_enviar_nibble(0x03);
    __delay_us(150);
    lcd_enviar_nibble(0x02);
    __delay_us(150);

    /* Dos lineas, matriz 5x8, pantalla encendida y cursor apagado. */
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_clear();
}

/**
 * @brief Envia un comando al controlador LCD.
 * @param comando Codigo de instruccion.
 */
void lcd_command(uint8_t comando)
{
    LCD_RS = 0;
    lcd_enviar_nibble((uint8_t)(comando >> 4u));
    lcd_enviar_nibble((uint8_t)(comando & 0x0Fu));

    if ((comando == 0x01u) || (comando == 0x02u))
    {
        __delay_ms(2);
    }
}

/**
 * @brief Escribe un caracter ASCII en la LCD.
 * @param caracter Caracter que se desea mostrar.
 */
void lcd_putc(char caracter)
{
    uint8_t dato = (uint8_t)caracter;

    LCD_RS = 1;
    lcd_enviar_nibble((uint8_t)(dato >> 4u));
    lcd_enviar_nibble((uint8_t)(dato & 0x0Fu));
}

/**
 * @brief Escribe una cadena completa en la LCD.
 * @param texto Puntero a la cadena terminada en nulo.
 */
void lcd_puts(const char *texto)
{
    while (*texto != '\0')
    {
        lcd_putc(*texto);
        texto++;
    }
}

/**
 * @brief Ubica el cursor en una posicion especifica.
 * @param fila Fila 0 o 1.
 * @param columna Columna 0 a 15.
 */
void lcd_goto(uint8_t fila, uint8_t columna)
{
    uint8_t direccion;

    if ((fila >= LCD_FILAS) || (columna >= LCD_COLUMNAS))
    {
        return;
    }

    if (fila == 0u)
    {
        direccion = columna;
    }
    else
    {
        direccion = (uint8_t)(0x40u + columna);
    }

    lcd_command((uint8_t)(0x80u | direccion));
}

/** @brief Limpia la pantalla LCD. */
void lcd_clear(void)
{
    lcd_command(0x01);
}
