/**
 * @file crypto.c
 * @brief Implementacion del cifrado y descifrado de ocho bits.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#include "crypto.h"
#include <stddef.h>

/** @brief Tabla de caracteres hexadecimales en mayusculas. */
const char HEX[] = "0123456789ABCDEF";

/**
 * @brief Cifra un byte con C = (M XOR K) + 5.
 * @param mensaje Byte original.
 * @param clave Byte de la clave.
 * @return Byte cifrado.
 */
uint8_t cifrar_byte(uint8_t mensaje, uint8_t clave)
{
    uint8_t resultado_xor;

    resultado_xor = (uint8_t)(mensaje ^ clave);

    /* El cast conserva el resultado en aritmetica de ocho bits. */
    return (uint8_t)(resultado_xor + 5u);
}

/**
 * @brief Descifra un byte aplicando la ecuacion inversa.
 * @param cifrado Byte cifrado.
 * @param clave Byte de la clave.
 * @return Byte original recuperado.
 */
uint8_t descifrar_byte(uint8_t cifrado, uint8_t clave)
{
    uint8_t resultado_resta;

    /*
     * Primero se resta 5 y despues se aplica XOR.
     * La inversa deshace las operaciones en orden contrario
     * al cifrado: primero deshace la suma y luego el XOR.
     */
    resultado_resta = (uint8_t)(cifrado - 5u);

    return (uint8_t)(resultado_resta ^ clave);
}

/**
 * @brief Convierte un byte en dos caracteres HEX.
 * @param valor Byte que se desea convertir.
 * @param salida Arreglo de dos posiciones.
 */
void byte_a_hex(uint8_t valor, char salida[2])
{
    if (salida == NULL)
    {
        return;
    }

    salida[0] = HEX[(valor >> 4u) & 0x0Fu];
    salida[1] = HEX[valor & 0x0Fu];
}

/**
 * @brief Convierte un caracter hexadecimal ASCII en su valor.
 * @param caracter Caracter hexadecimal.
 * @param nibble Puntero de salida.
 * @return true si la conversion fue exitosa.
 */
bool hex_a_nibble(char caracter, uint8_t *nibble)
{
    if (nibble == NULL)
    {
        return false;
    }

    if ((caracter >= '0') && (caracter <= '9'))
    {
        *nibble = (uint8_t)(caracter - '0');
        return true;
    }

    if ((caracter >= 'A') && (caracter <= 'F'))
    {
        *nibble = (uint8_t)(caracter - 'A' + 10);
        return true;
    }

    return false;
}

/**
 * @brief Empaqueta ocho caracteres HEX en cuatro bytes.
 * @param hex Cadena de ocho caracteres.
 * @param bytes Arreglo de cuatro bytes.
 * @return true si la conversion completa fue correcta.
 */
bool hex_a_bytes(const char hex[9], uint8_t bytes[4])
{
    uint8_t i;
    uint8_t alto;
    uint8_t bajo;

    if ((hex == NULL) || (bytes == NULL))
    {
        return false;
    }

    for (i = 0u; i < 4u; i++)
    {
        if (!hex_a_nibble(hex[(uint8_t)(2u * i)], &alto))
        {
            return false;
        }

        if (!hex_a_nibble(hex[(uint8_t)(2u * i + 1u)], &bajo))
        {
            return false;
        }

        bytes[i] = (uint8_t)(
            (uint8_t)(alto << 4u) | bajo
        );
    }

    return true;
}
