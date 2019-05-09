/*
 * piTankGoLib.h
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro Capilla e Iris Rubio Saez
 */
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

#include <stdio.h>

#include "fsm.h"
#include "tmr.h"

//Reloj del sistema
#define CLK_MS 5

// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME_TECLADO	200
// DURACION DISPARO IR
#define SHOOTING_PERIOD 1500

// CLAVES PARA MUTEX
#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	4
#define TORRETA_FLAG		8

// Distribucion de pines GPIO empleada para el joystick
#define JOY_PIN_UP	5
#define JOY_PIN_DOWN	6
#define JOY_PIN_LEFT	19
#define JOY_PIN_RIGHT	13
#define JOY_PIN_CENTER 27

// Distribucion de pines GPIO empleada para los botones
#define BUTTON_START 16
#define BUTTON_END 12

// Distribucion de pines GPIO empleada para el enlace IR
#define	IR_TX_PIN		21
#define	IR_RX_PIN		20

// Distribucion de pines GPIO empleada para la reproduccion de efectos
#define PLAYER_PWM_PIN 	23

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
#define FLAG_SYSTEM_START 		1
#define FLAG_JOYSTICK_UP 		2
#define FLAG_JOYSTICK_DOWN 		4
#define FLAG_JOYSTICK_LEFT		8
#define FLAG_JOYSTICK_RIGHT		16
#define FLAG_TRIGGER_BUTTON		32
#define FLAG_SHOOT_TIMEOUT		64
#define FLAG_TARGET_DONE		128
#define FLAG_SYSTEM_END			256

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
//Solo un bit a 1 sino puede haber conflicto
#define FLAG_START_DISPARO		1
#define FLAG_START_IMPACTO		2
#define FLAG_PLAYER_STOP		4
#define FLAG_PLAYER_END			8
#define FLAG_NOTA_TIMEOUT		16

extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

#endif /* _PITANKGOLIB_H_ */
