
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
#define SHOOTING_PERIOD 0

// CLAVES PARA MUTEX
#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	4
#define TORRETA_FLAG		8

// Distribucion de pines GPIO empleada para el teclado
/*#define GPIO_COL_1 2
#define GPIO_COL_2 3
#define GPIO_COL_3 4
#define GPIO_COL_4 17

#define GPIO_ROW_1 27
#define GPIO_ROW_2 22
#define GPIO_ROW_3 10
#define GPIO_ROW_4 9*/

// Distribucion de pines GPIO empleada para el joystick
#define JOY_PIN_UP	5
#define JOY_PIN_DOWN	6
#define JOY_PIN_LEFT	13
#define JOY_PIN_RIGHT	19
#define JOY_PIN_CENTER 27

// Distribucion de pines GPIO empleada para el enlace IR
#define	IR_TX_PIN		21
#define	IR_RX_PIN		0
#define NEO				2

// Distribucion de pines GPIO empleada para la reproduccion de efectos
#define PLAYER_PWM_PIN 	23

// Distribucion de pines GPIO empleada para el control de los servos
#define	SERVO_VERTICAL_PIN		0
#define	SERVO_HORIZONTAL_PIN	0

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

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
};

//Externalizamos variables para poder usarlos en otras estructuras
extern int flags_system;
extern int flags_player;

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
