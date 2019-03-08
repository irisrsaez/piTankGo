/*
 * tecladoTL04.h
 *
 *  Created on: 4 mar. 2019
 *      Author: Iris
 */

#ifndef TECLADOTL04_H_
#define TECLADOTL04_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "piTankGoLib.h"
#include "fsm.h"
#include "tmr.h"

//#define CLK_MS_TECLADO 		5
//#define	DEBOUNCE_TIME_TECLADO	200
#define COL_REFRESH_TIME 25

#define	FLAG_KEY			3

#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

#define NUM_COLS 4
#define NUM_ROWS 4

enum columns_values {
	COL_1,
	COL_2,
	COL_3,
	COL_4,
};

enum rows_values {
	ROW_1,
	ROW_2,
	ROW_3,
	ROW_4
};

enum column_state {
	KEY_COL_1,
	KEY_COL_2,
	KEY_COL_3,
	KEY_COL_4
};

enum keypad_state {
	KEY_WAITING
};

typedef struct {
	int col;
	int row;
} TipoTecla;

typedef struct {
	int columna_actual;
	tmr_t* tmr_duracion_columna;
	TipoTecla teclaPulsada;
} TipoTeclado;

//Variable TipoTeclado externa para usarla en main
extern TipoTeclado teclado;
//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int initialize(TipoTeclado *p_teclado);
int key_pressed (fsm_t* this);
int CompruebaColumnTimeout (fsm_t* this) ;

// Prototipos de funciones de salida
void process_key (fsm_t* this);
void col_1 (fsm_t* this);
void col_2 (fsm_t* this);
void col_3 (fsm_t* this);
void col_4 (fsm_t* this);

#endif /* TECLADOTL04_H_ */
