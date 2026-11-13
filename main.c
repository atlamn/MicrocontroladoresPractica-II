/**
 * @file main.c
 * @brief Programa principal del sistema de cifrado simetrico hibrido.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#include "config.h"
#include "lcd.h"
#include "keypad.h"
#include "crypto.h"

/* Bits de configuracion del PIC16F887. */
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF
#pragma config DEBUG = OFF
#pragma config WRT = OFF

/**
 * @brief Estados de funcionamiento del sistema.
 * @details S0 inicializacion; S1 menu; S2 mensaje; S3 mensaje completo;
 *          S4 clave cifrado; S5 resultado cifrado; S6 criptograma;
 *          S7 criptograma completo; S8 clave descifrado; S9 resultado.
 */
typedef enum
{
    ESTADO_S0 = 0,
    ESTADO_S1,
    ESTADO_S2,
    ESTADO_S3,
    ESTADO_S4,
    ESTADO_S5,
    ESTADO_S6,
    ESTADO_S7,
    ESTADO_S8,
    ESTADO_S9
} EstadoSistema;

/**
 * @brief Mensaje original: cuatro caracteres y terminador nulo.
 * @details [R3] Cinco posiciones para cuatro caracteres.
 */
static char mensaje[5];

/**
 * @brief HEX de entrada: ocho caracteres y terminador nulo.
 * @details [R3] Nueve posiciones para ocho caracteres.
 */
static char hex_entrada[9];

/** @brief Cuatro bytes del criptograma. */
static uint8_t criptograma[4];

/**
 * @brief HEX de salida: ocho caracteres y terminador nulo.
 * @details [R3] Nueve posiciones para ocho caracteres.
 */
static char hex_salida[9];

/** @brief Codigo ASCII de la clave secreta. */
static uint8_t clave;

/** @brief Indice de escritura del mensaje. */
static uint8_t indice_mensaje;

/** @brief Indice de escritura del criptograma hexadecimal. */
static uint8_t indice_hex;

/** @brief Estado actual de la maquina de estados. */
static EstadoSistema estado_actual = ESTADO_S0;

/**
 * @brief Configura los registros principales del microcontrolador.
 */
static void hardware_init(void)
{
    INTCON = 0x00;

    /* [R4] Deshabilitar todos los canales analogicos. */
    ANSEL = 0x00;
    ANSELH = 0x00;

    /* RD0, RD1 y RD4-RD7 salidas; RD2-RD3 entradas. */
    TRISD = 0x0C;
    PORTD = 0x00;

    keypad_configurar();
}

/**
 * @brief Reinicia todos los buffers e indices.
 */
static void limpiar_buffers(void)
{
    uint8_t i;

    for (i = 0u; i < 5u; i++)
    {
        mensaje[i] = '\0';
    }

    for (i = 0u; i < 9u; i++)
    {
        hex_entrada[i] = '\0';
        hex_salida[i] = '\0';
    }

    for (i = 0u; i < 4u; i++)
    {
        criptograma[i] = 0u;
    }

    indice_mensaje = 0u;
    indice_hex = 0u;
    clave = 0u;
}

/** @brief Presenta el menu principal en la LCD. */
static void mostrar_menu(void)
{
    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("A: Cifrar");
    lcd_goto(1u, 0u);
    lcd_puts("B: Descifrar");
}

/** @brief Presenta el mensaje completo antes de solicitar la clave. */
static void mostrar_mensaje_completo(void)
{
    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Mensaje completo");
    lcd_goto(1u, 0u);
    lcd_puts(mensaje);
}

/** @brief Presenta la solicitud de captura de clave. */
static void mostrar_prompt_clave(void)
{
    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Ingrese clave:");
}

/**
 * @brief Inicia la captura del mensaje original.
 */
static void iniciar_captura_mensaje(void)
{
    uint8_t i;

    for (i = 0u; i < 5u; i++)
    {
        mensaje[i] = '\0';
    }

    indice_mensaje = 0u;

    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Mensaje: ");
    lcd_goto(0u, 9u);
}

/**
 * @brief Inicia la captura de los ocho caracteres hexadecimales.
 */
static void iniciar_captura_criptograma(void)
{
    uint8_t i;

    for (i = 0u; i < 9u; i++)
    {
        hex_entrada[i] = '\0';
    }

    indice_hex = 0u;

    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Criptograma HEX:");
    lcd_goto(1u, 0u);
}

/**
 * @brief Comprueba si un caracter puede pertenecer al mensaje.
 * @param caracter Caracter ASCII recibido.
 * @return true si es valido: 0-9, A-D o #.
 */
static bool mensaje_caracter_valido(char caracter)
{
    if ((caracter >= '0') && (caracter <= '9'))
    {
        return true;
    }

    if ((caracter >= 'A') && (caracter <= 'D'))
    {
        return true;
    }

    return (caracter == '#');
}

/**
 * @brief Calcula y presenta el criptograma.
 */
static void mostrar_resultado_cifrado(void)
{
    uint8_t i;
    uint8_t posicion;

    for (i = 0u; i < 4u; i++)
    {
        /* [R2] C = (M XOR K) + 5 en aritmetica uint8_t. */
        criptograma[i] = cifrar_byte((uint8_t)mensaje[i], clave);

        posicion = (uint8_t)(2u * i);

        byte_a_hex(criptograma[i], &hex_salida[posicion]);
    }

    hex_salida[8] = '\0';

    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Cifrado (HEX):");
    lcd_goto(1u, 0u);
    lcd_puts(hex_salida);

    estado_actual = ESTADO_S5;
}

/**
 * @brief Reconstruye los bytes y presenta el mensaje descifrado.
 */
static void mostrar_resultado_descifrado(void)
{
    uint8_t i;
    uint8_t caracter_original;

    /* [R2] Ocho caracteres HEX se empaquetan en cuatro bytes. */
    if (!hex_a_bytes(hex_entrada, criptograma))
    {
        iniciar_captura_criptograma();
        estado_actual = ESTADO_S6;
        return;
    }

    for (i = 0u; i < 4u; i++)
    {
        /* [R2] M = (C - 5) XOR K: resta antes del XOR. */
        caracter_original = descifrar_byte(criptograma[i], clave);

        if ((caracter_original < 0x20u) ||
            (caracter_original > 0x7Eu))
        {
            mensaje[i] = '.';
        }
        else
        {
            mensaje[i] = (char)caracter_original;
        }
    }

    mensaje[4] = '\0';

    lcd_clear();
    lcd_goto(0u, 0u);
    lcd_puts("Mensaje Original");
    lcd_goto(1u, 0u);
    lcd_puts(mensaje);

    estado_actual = ESTADO_S9;
}

/** @brief Regresa al menu y reinicia los datos de la operacion. */
static void regresar_menu(void)
{
    limpiar_buffers();
    mostrar_menu();
    estado_actual = ESTADO_S1;
}

/**
 * @brief Procesa una tecla segun el estado actual.
 * @param tecla Codigo ASCII de la tecla detectada por IOC.
 */
static void procesar_tecla(uint8_t tecla)
{
    char caracter_hex;

    switch (estado_actual)
    {
        case ESTADO_S0:
            /* La inicializacion se realiza desde main(). */
            break;

        case ESTADO_S1:
            /*
             * [R1] FILTRO EXPLICITO DEL MENU:
             * solo A y B son aceptadas; las demas teclas se ignoran.
             */
            switch (tecla)
            {
                case 'A':
                    limpiar_buffers();
                    iniciar_captura_mensaje();
                    estado_actual = ESTADO_S2;
                    break;

                case 'B':
                    limpiar_buffers();
                    iniciar_captura_criptograma();
                    estado_actual = ESTADO_S6;
                    break;

                default:
                    /* Tecla invalida: se ignora. */
                    break;
            }
            break;

        case ESTADO_S2:
            /* [R3] Comprobar el indice antes de escribir. */
            if ((indice_mensaje < 4u) &&
                mensaje_caracter_valido((char)tecla))
            {
                mensaje[indice_mensaje] = (char)tecla;

                lcd_goto(0u, (uint8_t)(9u + indice_mensaje));
                lcd_putc((char)tecla);

                indice_mensaje++;

                if (indice_mensaje == 4u)
                {
                    mensaje[4] = '\0';
                    mostrar_mensaje_completo();
                    estado_actual = ESTADO_S3;
                }
            }
            break;

        case ESTADO_S3:
            if (tecla == '*')
            {
                mostrar_prompt_clave();
                estado_actual = ESTADO_S4;
            }
            break;

        case ESTADO_S4:
            /* Clave cruda: no se aplica el mapeo E/F. */
            clave = tecla;

            lcd_goto(1u, 0u);
            lcd_putc('*');

            /* El asterisco permanece visible 300 ms. */
            __delay_ms(300);

            mostrar_resultado_cifrado();
            estado_actual = ESTADO_S5;
            break;

        case ESTADO_S5:
            if (tecla == '*')
            {
                regresar_menu();
            }
            break;

        case ESTADO_S6:
            /*
             * [R3] UNICO BLOQUE DE MAPEO HEX, exclusivo de S6:
             * '*' representa E y '#' representa F.
             */
            if (tecla == '*')
            {
                caracter_hex = 'E';
            }
            else if (tecla == '#')
            {
                caracter_hex = 'F';
            }
            else if (((tecla >= '0') && (tecla <= '9')) ||
                     ((tecla >= 'A') && (tecla <= 'D')))
            {
                caracter_hex = (char)tecla;
            }
            else
            {
                break;
            }

            /* [R3] Se verifica el indice antes de escribir. */
            if (indice_hex < 8u)
            {
                hex_entrada[indice_hex] = caracter_hex;

                lcd_goto(1u, indice_hex);
                lcd_putc(caracter_hex);

                indice_hex++;

                if (indice_hex == 8u)
                {
                    hex_entrada[8] = '\0';

                    lcd_clear();
                    lcd_goto(0u, 0u);
                    lcd_puts("HEX completo");
                    lcd_goto(1u, 0u);
                    lcd_puts(hex_entrada);

                    estado_actual = ESTADO_S7;
                }
            }
            break;

        case ESTADO_S7:
            /* El mapeo ya esta desactivado; '*' es control. */
            if (tecla == '*')
            {
                mostrar_prompt_clave();
                estado_actual = ESTADO_S8;
            }
            break;

        case ESTADO_S8:
            /* Clave cruda: no se aplica el mapeo E/F. */
            clave = tecla;

            lcd_goto(1u, 0u);
            lcd_putc('*');

            __delay_ms(300);

            mostrar_resultado_descifrado();
            break;

        case ESTADO_S9:
            if (tecla == '*')
            {
                regresar_menu();
            }
            break;

        default:
            estado_actual = ESTADO_S0;
            break;
    }
}

/**
 * @brief Punto de entrada del programa.
 * @details El ciclo principal no escanea el teclado ni espera teclas.
 *          Solamente procesa eventos registrados por la ISR IOC.
 */
void main(void)
{
    uint8_t tecla;

    hardware_init();
    estado_actual = ESTADO_S0;

    while (1)
    {
        if (estado_actual == ESTADO_S0)
        {
            lcd_init();
            limpiar_buffers();
            mostrar_menu();

            estado_actual = ESTADO_S1;

            /* [R4] Habilitar IOC despues de inicializar periféricos. */
            keypad_habilitar_ioc();
        }

        /* [R4] Main procesa un evento; no escanea el teclado. */
        if (g_bandera_tecla != 0u)
        {
            tecla = g_tecla_evento;
            g_bandera_tecla = 0u;

            procesar_tecla(tecla);

            /* Antirrebote fuera de la ISR. */
            __delay_ms(20);

            /* [R4] Rearme: filas, lectura PORTB, RBIF, RBIE. */
            keypad_rearme();
        }
    }
}
