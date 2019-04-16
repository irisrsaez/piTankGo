/*
 * joystick.h
 *
 *  Created on: 8 abr. 2019
 *      Author: Iris
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>



#ifndef JOYSTICK_H_
#define JOYSTICK_H_

extern int flags_torreta;
extern int flags_player;

//FUNCION ENTRADA
int inicializa();

//FUNCIONES DE SALIDA
void joystick_up();
void joystick_down();
void joystick_left();
void joystick_right();
void joystick_center();

#endif /* JOYSTICK_H_ */
