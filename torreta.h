/*
 * torreta.h
 *
 *  Created on: 21 de enero de 2019
 *      Author: FFM
 */

#ifndef _TORRETA_H_
#define _TORRETA_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringSerial.h>

#include "tmr.h"
#include "fsm.h"
#include "kbhit.h"
#include "joystick.h"

//Asignacion de pines PWM de salida
#define	TORRETA_PIN_PWM_X	18
#define	TORRETA_PIN_PWM_Y	24

// PWM SW: el SO provoca que cuanto más alta sea la frecuencia de la señal PWM de control menos estable resulta esta (i.e. cimbreo del servo)
// Interesa una frecuencia para la señal de control relativamente baja para minimizar el cimbreo
// El problema es que tampoco puede ser demasiado baja o de lo contrario el control del servo se resiente y su respuesta es peor
// (más lenta ante variaciones rápidas del ciclo de trabajo de la señal de control)
#define TORRETA_PWM_RANGE		400 // 100 * 400 = 40,000 µS = 40 ms // 25 Hz

//Variables constantes
#define TORRETA_INCREMENTO	1
#define TORRETA_MINIMO		9
#define TORRETA_MAXIMO		22

//Estructura que describe un objeto TipoServo
typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
} TipoServo;

//Estructura que describe un objeto TipoPosicionTorreta
typedef struct {
	int x; // Coordenada x correspondiente a la posicion del servo horizontal
	int y; // Coordenada y correspondiente a la posicion del servo vertical
} TipoPosicionTorreta;

//Estructura que describe un objeto TipoTorreta
typedef struct {
	TipoPosicionTorreta posicion;
	TipoServo servo_x;
	TipoServo servo_y;
	tmr_t* tmr;
	int duracion;
} TipoTorreta;

//Externalizamos el objeto TipoTorreta torreta para poder usarlo en otras extructuras
extern TipoTorreta torreta;

// Prototipos de procedimientos de inicializacion de los objetos especificos
void InicializaTorreta (TipoTorreta *p_torreta);

// Prototipos de funciones de entrada
int CompruebaComienzo (fsm_t* this);
int CompruebaJoystickUp (fsm_t* this);
int CompruebaJoystickDown (fsm_t* fsm_player);
int CompruebaJoystickLeft (fsm_t* this);
int CompruebaJoystickRight (fsm_t* this);
int CompruebaTimeoutDisparo (fsm_t* this);
int CompruebaImpacto (fsm_t* this);
int CompruebaTriggerButton (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);

// Prototipos de funciones de salida
void ComienzaSistema (fsm_t* this);
void MueveTorretaArriba (fsm_t* this);
void MueveTorretaAbajo (fsm_t* this);
void MueveTorretaIzquierda (fsm_t* this);
void MueveTorretaDerecha (fsm_t* this);
void DisparoIR (fsm_t* this);
void FinalDisparoIR (fsm_t* this);
void ImpactoDetectado (fsm_t* this);
void FinalizaJuego (fsm_t* this);
void timer_player_duracion_nota_actual_isr (union sigval value);
void Empieza ();

// Prototipos de procedimientos de atencion a las interrupciones
//static void timer_duracion_disparo_isr (union sigval value);

#endif /* _TORRETA_H_ */
