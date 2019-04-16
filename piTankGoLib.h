
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

#include <stdio.h>

#include "fsm.h"
#include "tmr.h"

#define CLK_MS 5 //10

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME_TECLADO	200
// DURACION DISPARO IR
#define SHOOTING_PERIOD 0

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	SYSTEM_FLAGS_KEY	1 //Mutex para SYSTEM
#define	PLAYER_FLAGS_KEY	2 //Mutex para PLAYER
#define	STD_IO_BUFFER_KEY	4

// Distribucion de pines GPIO empleada para el teclado
// ATENCION: Valores a modificar por el alumno
#define GPIO_COL_1 0
#define GPIO_COL_2 1
#define GPIO_COL_3 2
#define GPIO_COL_4 3

#define GPIO_ROW_1 5
#define GPIO_ROW_2 6
#define GPIO_ROW_3 12
#define GPIO_ROW_4 13
//#define JOY_PIN_UP	0
//#define JOY_PIN_DOWN	0
//#define JOY_PIN_LEFT	0
//#define JOY_PIN_RIGHT	0
//#define JOY_PIN_CENTER 0

/*#define	TECLADO_ROW_1	66
#define	TECLADO_ROW_2	68
#define	TECLADO_ROW_3	70
#define	TECLADO_ROW_4	72

#define	TECLADO_COL_1	74
#define	TECLADO_COL_2	76
#define	TECLADO_COL_3	78
#define	TECLADO_COL_4	80*/

// Distribucion de pines GPIO empleada para el enlace IR
// ATENCION: Valores a modificar por el alumno
#define	IR_TX_PIN		0
#define	IR_RX_PIN		0

// Distribucion de pines GPIO empleada para la reproducción de efectos
// ATENCION: Valores a modificar por el alumno
#define PLAYER_PWM_PIN 	18

// Distribucion de pines GPIO empleada para el control de los servos
// ATENCION: Valores a modificar por el alumno
#define	SERVO_VERTICAL_PIN		0
#define	SERVO_HORIZONTAL_PIN	0

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
// ATENCION: Valores a modificar por el alumno
#define FLAG_SYSTEM_START 		6
#define FLAG_JOYSTICK_UP 		8
#define FLAG_JOYSTICK_DOWN 		10
#define FLAG_JOYSTICK_LEFT		12
#define FLAG_JOYSTICK_RIGHT		14
#define FLAG_TRIGGER_BUTTON		16
#define FLAG_SHOOT_TIMEOUT		18
#define FLAG_TARGET_DONE		20
#define FLAG_SYSTEM_END			22

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
// ATENCION: Valores a modificar por el alumno
#define FLAG_START_DISPARO		24
#define FLAG_START_IMPACTO		26
#define FLAG_PLAYER_STOP		28
#define FLAG_PLAYER_END			30
#define FLAG_NOTA_TIMEOUT		32

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
	//JOYSTICK_UP_ISR,
	//JOYSTICK_DOWN_ISR,
	//JOYSTICK_LEFT_ISR,
	//JOYSTICK_RIGHT_ISR,
	//JOYSTICK_CENTER_ISR
};

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
