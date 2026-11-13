/**
 * @file keypad.c
 * @brief Control del teclado matricial 4x4 mediante interrupciones IOC.
 * @author Estudiante - Universidad del Cauca
 * @date 09/11/2026
 */

#include "keypad.h"

/** @brief Codigo ASCII de la tecla detectada por la ISR. */
volatile uint8_t g_tecla_evento = 0u;

/** @brief Bandera de evento pendiente para el programa principal. */
volatile uint8_t g_bandera_tecla = 0u;

/**
 * @brief Matriz de correspondencia entre filas, columnas y teclas.
 * @details Las filas representan RB0-RB3 y las columnas RB4-RB7.
 */
static const char mapa_teclas[KEYPAD_FILAS][KEYPAD_COLUMNAS] =
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/**
 * @brief Escanea las cuatro filas para identificar una tecla.
 * @details Se ejecuta desde la ISR. Los retardos son de asentamiento
 *          de microsegundos, no esperas prolongadas.
 * @return Codigo ASCII de la tecla o cero si no hay pulsacion.
 */
static uint8_t keypad_escanear(void)
{
    uint8_t fila;
    uint8_t columna;
    uint8_t columnas;

    for (fila = 0u; fila < KEYPAD_FILAS; fila++)
    {
        /* Todas las filas en alto antes de seleccionar una. */
        KEYPAD_PORT = KEYPAD_ROW_MASK;
        __delay_us(10);

        /* La fila seleccionada se lleva a nivel bajo. */
        KEYPAD_PORT = (uint8_t)(
            KEYPAD_ROW_MASK &
            (uint8_t)(~(uint8_t)(1u << fila))
        );
        __delay_us(10);

        columnas = (uint8_t)(KEYPAD_PORT & KEYPAD_COLUMN_MASK);

        if (columnas != KEYPAD_COLUMN_MASK)
        {
            for (columna = 0u; columna < KEYPAD_COLUMNAS; columna++)
            {
                if ((columnas &
                    (uint8_t)(1u << (columna + 4u))) == 0u)
                {
                    return (uint8_t)mapa_teclas[fila][columna];
                }
            }
        }
    }

    return 0u;
}

/**
 * @brief Configura los registros del teclado matricial.
 * @details TRISB=0xF0, WPUB=0xF0, IOCB=0xF0 y OPTION_REG=0x08.
 */
void keypad_configurar(void)
{
    volatile uint8_t lectura;

    KEYPAD_TRIS = 0xF0;
    KEYPAD_PORT = 0x00;

    /* Pull-ups individuales en RB4-RB7. */
    KEYPAD_WPUB = 0xF0;

    /* RBPU=0; T0CS=0; PSA=1; PS2:PS0=000. */
    OPTION_REG = 0x08;

    /* IOC exclusivamente en las columnas. */
    KEYPAD_IOCB = 0xF0;

    INTCONbits.RBIE = 0;
    INTCONbits.GIE = 0;

    lectura = KEYPAD_PORT;
    (void)lectura;

    INTCONbits.RBIF = 0;

    g_tecla_evento = 0u;
    g_bandera_tecla = 0u;
}

/**
 * @brief Habilita las interrupciones por cambio en PORTB.
 */
void keypad_habilitar_ioc(void)
{
    volatile uint8_t lectura;

    KEYPAD_PORT = 0x00;
    lectura = KEYPAD_PORT;
    (void)lectura;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.GIE = 1;
}

/**
 * @brief Realiza el rearme de la interrupcion IOC.
 * @details Orden: filas a bajo, lectura PORTB, limpieza RBIF y RBIE.
 */
void keypad_rearme(void)
{
    volatile uint8_t lectura;

    KEYPAD_PORT = 0x00;
    lectura = KEYPAD_PORT;
    (void)lectura;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
}

/**
 * @brief Rutina de servicio de interrupcion del microcontrolador.
 * @details Identifica la tecla, descarta el flanco de soltado y registra
 *          un evento. No utiliza LCD ni retardos prolongados.
 */
void __interrupt() isr(void)
{
    uint8_t tecla;
    volatile uint8_t lectura;

    if (INTCONbits.RBIF != 0u)
    {
        INTCONbits.RBIE = 0;

        lectura = KEYPAD_PORT;
        (void)lectura;

        tecla = keypad_escanear();

        /* Filas en bajo durante reposo. */
        KEYPAD_PORT = 0x00;

        /* Lectura para terminar la discrepancia de PORTB. */
        lectura = KEYPAD_PORT;
        (void)lectura;

        /* RBIF se limpia despues de leer PORTB. */
        INTCONbits.RBIF = 0;

        if (tecla != 0u)
        {
            if (g_bandera_tecla == 0u)
            {
                g_tecla_evento = tecla;
                g_bandera_tecla = 1u;
            }

            /*
             * RBIE permanece deshabilitado hasta que main procese
             * el evento, aplique 20 ms de antirrebote y rearme.
             */
        }
        else
        {
            /* Evento de liberacion: no se genera una tecla nueva. */
            INTCONbits.RBIE = 1;
        }
    }
}
