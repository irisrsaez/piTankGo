/*
 * joystick.c
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro e Iris Rubio
 */

#include "joystick.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include <wiringSerial.h>
#include "tmr.h"
#include "fsm.h"

/*
 * Método que inicializa el joystick
 * POSICIONES: DERECHA, IZQUIERDA, ARRIBA, ABAJO Y CENTRO
 */
int InicializaJoy(){

	//Configuramos el pin dado como entrada
	pinMode(JOY_PIN_LEFT, INPUT);
	/*Establecemos el modo de resistencia de pull-down en el pin dado, que debe establecerse como una entrada.
	 * Tienen un valor de aproximadamente 50KΩ*/
	pullUpDnControl(JOY_PIN_LEFT, PUD_DOWN);
	//Registramos la función para recibir interrupciones en el pin especificado.
	wiringPiISR(JOY_PIN_LEFT, INT_EDGE_RISING, joystick_left);

	pinMode(JOY_PIN_UP, INPUT);
	pullUpDnControl(JOY_PIN_UP, PUD_DOWN);
	wiringPiISR(JOY_PIN_UP, INT_EDGE_RISING, joystick_up);

	pinMode(JOY_PIN_DOWN, INPUT);
	pullUpDnControl(JOY_PIN_DOWN, PUD_DOWN);
	wiringPiISR(JOY_PIN_DOWN, INT_EDGE_RISING, joystick_down);

	pinMode(JOY_PIN_RIGHT, INPUT);
	pullUpDnControl(JOY_PIN_RIGHT, PUD_DOWN);
	wiringPiISR(JOY_PIN_RIGHT, INT_EDGE_RISING, joystick_right);

	pinMode(JOY_PIN_CENTER, INPUT);
	pullUpDnControl(JOY_PIN_CENTER, PUD_UP); //PUD_UP pull a 3.3v y R=50KΩ
	wiringPiISR(JOY_PIN_CENTER, INT_EDGE_RISING, joystick_center);

	return 0;

}

/*
 * Metodo pulsacion joystick abajo
 * Mueve la torreta hacia abajo en cada pulsacion
 */
void joystick_down(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de torreta
	flags_torreta |= FLAG_JOYSTICK_DOWN; //Subimos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de torreta

	//Imprimimos por el Serial el mensaje
	printf("down");
	serialPrintf(fd,"ABAJO");
	fflush(stdout);

}

/*
 * Metodo pulsacion joystick arriba
 * Mueve la torreta hacia arriba en cada pulsacion
 */
void joystick_up(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de torreta
	flags_torreta |= FLAG_JOYSTICK_UP; //Subimos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de torreta

	//Imprimimos por el Serial el mensaje
	printf("up");
	serialPrintf(fd,"ARRIBA");
	fflush(stdout);

}

/*
 * Metodo pulsacion joystick derecha
 * Mueve la torreta hacia la derecha en cada pulsacion
 */
void joystick_right(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de torreta
	flags_torreta |= FLAG_JOYSTICK_RIGHT; //Subimos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de torreta

	//Imprimimos por el Serial el mensaje
	printf("derecha");
	serialPrintf(fd,"DERECHA");
	fflush(stdout);

}

/*
 * Metodo pulsacion joystick izquierda
 * Mueve la torreta hacia la izquierda en cada pulsacion
 */
void joystick_left(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de torreta
	flags_torreta |= FLAG_JOYSTICK_LEFT; //Subimos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de torreta

	//Imprimimos por el Serial el mensaje
	printf("izquierda");
	serialPrintf(fd,"IZQUIERDA");
	fflush(stdout);

}

/*
 * Metodo pulsacion joystick centro
 * Dispara en cada pulsacion
 */
void joystick_center(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX del player
	flags_torreta |= FLAG_TRIGGER_BUTTON; //Subimos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX del player

	//Imprimimos por el Serial el mensaje
	printf("centro");
	serialPrintf(fd,"disparo IR");
	fflush(stdout);

}
