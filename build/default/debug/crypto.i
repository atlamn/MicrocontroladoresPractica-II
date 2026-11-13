# 1 "crypto.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 295 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "crypto.c" 2







# 1 "./crypto.h" 1
# 11 "./crypto.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/musl_xc8.h" 1 3
# 5 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 26 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 133 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned short uintptr_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef short intptr_t;
# 164 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;
# 192 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef int32_t intmax_t;







typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;
# 233 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef uint32_t uintmax_t;
# 27 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3

typedef int8_t int_fast8_t;




typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;




typedef uint8_t uint_fast8_t;




typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;
# 148 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 149 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdint.h" 2 3
# 12 "./crypto.h" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stdbool.h" 1 3
# 13 "./crypto.h" 2







uint8_t cifrar_byte(uint8_t mensaje, uint8_t clave);







uint8_t descifrar_byte(uint8_t cifrado, uint8_t clave);






void byte_a_hex(uint8_t valor, char salida[2]);







_Bool hex_a_nibble(char caracter, uint8_t *nibble);







_Bool hex_a_bytes(const char hex[9], uint8_t bytes[4]);
# 9 "crypto.c" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stddef.h" 1 3
# 19 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stddef.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 1 3
# 24 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef long int wchar_t;
# 128 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef unsigned size_t;
# 138 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/bits/alltypes.h" 3
typedef int ptrdiff_t;
# 20 "C:\\Program Files\\Microchip\\xc8\\v3.10\\pic\\include\\c99/stddef.h" 2 3
# 10 "crypto.c" 2


const char HEX[] = "0123456789ABCDEF";







uint8_t cifrar_byte(uint8_t mensaje, uint8_t clave)
{
    uint8_t resultado_xor;

    resultado_xor = (uint8_t)(mensaje ^ clave);


    return (uint8_t)(resultado_xor + 5u);
}







uint8_t descifrar_byte(uint8_t cifrado, uint8_t clave)
{
    uint8_t resultado_resta;






    resultado_resta = (uint8_t)(cifrado - 5u);

    return (uint8_t)(resultado_resta ^ clave);
}






void byte_a_hex(uint8_t valor, char salida[2])
{
    if (salida == ((void*)0))
    {
        return;
    }

    salida[0] = HEX[(valor >> 4u) & 0x0Fu];
    salida[1] = HEX[valor & 0x0Fu];
}







_Bool hex_a_nibble(char caracter, uint8_t *nibble)
{
    if (nibble == ((void*)0))
    {
        return 0;
    }

    if ((caracter >= '0') && (caracter <= '9'))
    {
        *nibble = (uint8_t)(caracter - '0');
        return 1;
    }

    if ((caracter >= 'A') && (caracter <= 'F'))
    {
        *nibble = (uint8_t)(caracter - 'A' + 10);
        return 1;
    }

    return 0;
}







_Bool hex_a_bytes(const char hex[9], uint8_t bytes[4])
{
    uint8_t i;
    uint8_t alto;
    uint8_t bajo;

    if ((hex == ((void*)0)) || (bytes == ((void*)0)))
    {
        return 0;
    }

    for (i = 0u; i < 4u; i++)
    {
        if (!hex_a_nibble(hex[(uint8_t)(2u * i)], &alto))
        {
            return 0;
        }

        if (!hex_a_nibble(hex[(uint8_t)(2u * i + 1u)], &bajo))
        {
            return 0;
        }

        bytes[i] = (uint8_t)(
            (uint8_t)(alto << 4u) | bajo
        );
    }

    return 1;
}
