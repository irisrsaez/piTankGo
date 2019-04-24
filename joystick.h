/*
 * joystick.h
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro e Iris Rubio
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

//Variables externas para poder usarlas en otras estructuras
extern int flags_torreta;
extern int flags_player;

// Prototipos de funciones de entrada
int InicializaJoy();

// Prototipos de funciones de salida
void joystick_up();
void joystick_down();
void joystick_left();
void joystick_right();
void joystick_center();

#endif /* JOYSTICK_H_ */
