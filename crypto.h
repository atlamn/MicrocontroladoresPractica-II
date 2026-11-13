/**
 * @file crypto.h
 * @brief Declaraciones de las funciones criptograficas.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Cifra un byte utilizando XOR y desplazamiento de cinco.
 * @param mensaje Byte original.
 * @param clave Byte de la clave secreta.
 * @return Byte cifrado en aritmetica de ocho bits.
 */
uint8_t cifrar_byte(uint8_t mensaje, uint8_t clave);

/**
 * @brief Descifra un byte utilizando la ecuacion inversa.
 * @param cifrado Byte cifrado.
 * @param clave Byte de la clave secreta.
 * @return Byte original recuperado.
 */
uint8_t descifrar_byte(uint8_t cifrado, uint8_t clave);

/**
 * @brief Convierte un byte en dos caracteres hexadecimales.
 * @param valor Byte que se desea convertir.
 * @param salida Arreglo de dos caracteres.
 */
void byte_a_hex(uint8_t valor, char salida[2]);

/**
 * @brief Convierte un caracter hexadecimal en un nibble.
 * @param caracter Caracter hexadecimal ASCII.
 * @param nibble Puntero donde se almacena el resultado.
 * @return true si el caracter es hexadecimal valido.
 */
bool hex_a_nibble(char caracter, uint8_t *nibble);

/**
 * @brief Empaqueta ocho caracteres HEX en cuatro bytes.
 * @param hex Arreglo de ocho caracteres hexadecimales.
 * @param bytes Arreglo de cuatro bytes resultantes.
 * @return true si todos los caracteres son validos.
 */
bool hex_a_bytes(const char hex[9], uint8_t bytes[4]);

#endif
