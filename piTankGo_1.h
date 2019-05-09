/*
 * piTankGo_1.h
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro Capilla e Iris Rubio Saez
 */

#ifndef _PITANKGO_1_H_
#define _PITANKGO_1_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softTone.h>
#include <wiringSerial.h>

#include "kbhit.h"
#include "player.h"
#include "torreta.h"
#include "tmr.h"

// Posibles estados de las FSMs
enum fsm_state {
	WAIT_START,
	WAIT_KEY,
	WAIT_PUSH,
	WAIT_NEXT,
	WAIT_MOVE,
	WAIT_END,
	TRIGGER_BUTTON
}TipoEstado;

//Estructura que describe un objeto TipoSistema
typedef struct {
	TipoTorreta torreta; // Objeto para el control de la torreta
	TipoPlayer player; // Reproductor de efectos
	char teclaPulsada;  // Variable que almacena la ultima tecla pulsada
	int debug; // Variable que habilita o deshabilita la impresion de mensajes por salida estandar
} TipoSistema;

//Variable externa del serial para su uso en otras estructuras
extern int fd;
extern int fi;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int ConfiguraSistema (TipoSistema *p_sistema);
int InicializaSistema (TipoSistema *p_sistema);

#endif /* _PITANKGO_1_H_ */
