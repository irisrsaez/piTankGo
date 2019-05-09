/*
 * joystick.h
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro Capilla e Iris Rubio Saez
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Prototipos de funciones de entrada
int InicializaJoy();

// Prototipos de funciones de salida
void joystick_up();
void joystick_down();
void joystick_left();
void joystick_right();
void joystick_center();
void ComienzaJuego();
void FinalJuego();

#endif /* JOYSTICK_H_ */
