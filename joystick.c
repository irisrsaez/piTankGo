/*
 * joystick.c
 *
 *  Created on: 8 abr. 2019
 *      Author: Iris
 */

#include "joystick.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include "tmr.h"
#include "fsm.h"


int inicializa(){

	pinMode(JOY_PIN_LEFT, INPUT); //configurar el puerto de entrada
	pullUpDnControl(JOY_PIN_LEFT, PUD_DOWN);
	wiringPiISR(JOY_PIN_LEFT, INT_EDGE_RISING, joystick_left);

	pinMode(JOY_PIN_UP, INPUT); //configurar el puerto de entrada
	pullUpDnControl(JOY_PIN_UP, PUD_DOWN);
	wiringPiISR(JOY_PIN_UP, INT_EDGE_RISING, joystick_up);

	pinMode(JOY_PIN_DOWN, INPUT); //configurar el puerto de entrada
	pullUpDnControl(JOY_PIN_DOWN, PUD_DOWN);
	wiringPiISR(JOY_PIN_DOWN, INT_EDGE_RISING, joystick_down);

	pinMode(JOY_PIN_RIGHT, INPUT); //configurar el puerto de entrada
	pullUpDnControl(JOY_PIN_RIGHT, PUD_DOWN);
	wiringPiISR(JOY_PIN_RIGHT, INT_EDGE_RISING, joystick_right);

	pinMode(JOY_PIN_CENTER, INPUT); //configurar el puerto de entrada
	pullUpDnControl(JOY_PIN_CENTER, PUD_UP);
	wiringPiISR(JOY_PIN_CENTER, INT_EDGE_RISING, joystick_center);

	piLock(TORRETA_FLAG);
	flags_torreta |= FLAG_SYSTEM_START;
	piUnlock(TORRETA_FLAG);

	printf("\n[JOYSTICK START]\n");

	return 0;

}

void joystick_down(){

		//if (digitalRead(JOY_PIN_DOWN) == HIGH) { //cuando detecta un flanco de subida
			piLock(TORRETA_FLAG);
			flags_torreta |= FLAG_JOYSTICK_DOWN;
			piUnlock(TORRETA_FLAG);
			printf("\n[PULSACION][TORRETA DOWN!!!] %i\n",flags_torreta);
		//}
}

void joystick_up(){
		//if (digitalRead(JOY_PIN_UP) == HIGH) { //cuando detecta un flanco de subida
			piLock(TORRETA_FLAG);
			flags_torreta |= FLAG_JOYSTICK_UP;
			piUnlock(TORRETA_FLAG);
			printf("\n[PULSACION][TORRETA UP!!!]\n");
		//}

}

void joystick_right(){

		//if (digitalRead(JOY_PIN_RIGHT) == HIGH) { //cuando detecta un flanco de subida
					piLock(TORRETA_FLAG);
					flags_torreta |= FLAG_JOYSTICK_RIGHT;
					piUnlock(TORRETA_FLAG);
					printf("\n[PULSACION][TORRETA RIGHT!!!]\n");
		//}
}

void joystick_left(){

		//if (digitalRead(JOY_PIN_LEFT) == HIGH) { //cuando detecta un flanco de subida
			piLock(TORRETA_FLAG);
			flags_torreta |= FLAG_JOYSTICK_LEFT;
			piUnlock(TORRETA_FLAG);
			printf("\n[PULSACION][TORRETA LEFT!!!]\n");
		//}
}

void joystick_center(){

		//if (digitalRead(JOY_PIN_LEFT) == HIGH) { //cuando detecta un flanco de subida
			piLock(PLAYER_FLAGS_KEY);
			flags_player |= FLAG_START_DISPARO;
			piUnlock(PLAYER_FLAGS_KEY);
			printf("\n[PULSACION][TORRETA LEFT!!!]\n");
		//}
}
