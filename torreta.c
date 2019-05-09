/*
 * torreta.c
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro Capilla e Iris Rubio Saez
 */

#include <softPwm.h>
#include "tmr.h"
#include "torreta.h"
#include "piTankGoLib.h"
#include "piTankGo_1.h"
#include "joystick.h"
#include <wiringSerial.h>

int flags_torreta = 0;

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

/*
 * Timer de la torreta
 */
void timer_torreta_isr (union sigval value) {

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta |= FLAG_SHOOT_TIMEOUT; //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de la torreta

}

//---------------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//---------------------------------------------------------------

/*
 * Metodo de inicializacion de una torreta (Objeto TipoTorreta)
 */
void InicializaTorreta (TipoTorreta *p_torreta) {

	/*Inicializa libreria wiringPi y supone que el programa de llamada usará el esquema de numeración de pin wiringPi.
	Permite que los programas de llamada utilicen los números de pin Broadcom GPIO directamente sin volver a asignar*/
	wiringPiSetupGpio();

	tmr_t* tmr = tmr_new(timer_torreta_isr); //Creo el timer y se lo asigno a la torreta
	p_torreta->tmr = tmr;
	p_torreta->duracion = SHOOTING_PERIOD;

	pinMode(IR_TX_PIN, OUTPUT); //Configuramos pin de salida del IR
	digitalWrite(IR_TX_PIN, LOW); //Le damos un valor inicial a LOW

	pinMode(IR_RX_PIN, INPUT); //Configuramos pin de entrada del IR
	pullUpDnControl(IR_RX_PIN, PUD_DOWN);/*Establecemos el modo de resistencia de pull-down en el pin dado, que debe establecerse como una entrada.
	 * Tienen un valor de aproximadamente 50KΩ*/
	wiringPiISR(IR_RX_PIN, INT_EDGE_RISING, ImpactoRX); //Configuramos el metodo que va a ejecutar el RX

	//Inicializamos x e y de la misma manera ambos
	p_torreta->servo_x.incremento = TORRETA_INCREMENTO;
	p_torreta->servo_y.incremento = TORRETA_INCREMENTO;
	p_torreta->servo_x.minimo = TORRETA_MINIMO;
	p_torreta->servo_y.minimo = TORRETA_MINIMO;
	p_torreta->servo_x.maximo = TORRETA_MAXIMO;
	p_torreta->servo_y.maximo = TORRETA_MAXIMO;

	//Imponemos una posicion de inicio a la torreta
	p_torreta->servo_x.inicio 	= TORRETA_MINIMO + (TORRETA_MAXIMO - TORRETA_MINIMO)/2;
	p_torreta->servo_y.inicio 	= TORRETA_MINIMO + (TORRETA_MAXIMO - TORRETA_MINIMO)/2;

	//Asignamos posicion de inicio a la torreta
	p_torreta->posicion.x	= p_torreta->servo_x.inicio;
	p_torreta->posicion.y	= p_torreta->servo_y.inicio;

	//Ponemos limites a las posiciones
	if(p_torreta->posicion.x > p_torreta->servo_x.maximo)
		p_torreta->posicion.x = p_torreta->servo_x.maximo;
	if(p_torreta->posicion.y > p_torreta->servo_y.maximo)
			p_torreta->posicion.y = p_torreta->servo_y.maximo;

	if(p_torreta->posicion.x < p_torreta->servo_x.minimo)
			p_torreta->posicion.x = p_torreta->servo_x.minimo;
	if(p_torreta->posicion.y < p_torreta->servo_y.minimo)
				p_torreta->posicion.y = p_torreta->servo_y.minimo;

	//Creamos un pin PWM controlado por software al cual le decimos la posicion de inicio y el rango de trabajo
	softPwmCreate (TORRETA_PIN_PWM_X, p_torreta->servo_x.inicio, TORRETA_PWM_RANGE);
	//Actualizamos el valor de PWM en el pin dado.
	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	//Creamos un pin PWM controlado por software al cual le decimos la posicion de inicio y el rango de trabajo
	softPwmCreate (TORRETA_PIN_PWM_Y, p_torreta->servo_y.inicio, TORRETA_PWM_RANGE);
	//Actualizamos el valor de PWM en el pin dado.
	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y);

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/*
 * Metodo de comprobacion del comienzo del sistema
 */
int CompruebaComienzo (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_SYSTEM_START);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del movimiento de la torreta hacia arriba
 */
int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_JOYSTICK_UP);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del movimiento de la torreta hacia abajo
 */
int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_JOYSTICK_DOWN);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del movimiento de la torreta hacia la izquierda
 */
int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_JOYSTICK_LEFT);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del movimiento de la torreta hacia la derecha
 */
int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_JOYSTICK_RIGHT);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag de timeout del disparo
 */
int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_SHOOT_TIMEOUT);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del impacto
 */
int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	result = (flags_torreta & FLAG_TARGET_DONE);
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	return result;
}

/*
 * Metodo de comprobacion del flag del trigger
 */
int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_TRIGGER_BUTTON);
	piUnlock(TORRETA_FLAG);

	return result;
}

/*
 * Metodo de comprobacion del flag de finalizacion del juego
 */
int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_SYSTEM_END);
	piUnlock(TORRETA_FLAG);

	return result;
}

//-----------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//-----------------------------------------------------------

/*
 * Metodo que sube el flag del sistema para que empiece
 */
void ComienzaSistema (fsm_t* this) {

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	flags_torreta |= FLAG_SYSTEM_START; //Subimos el flag del comienzo del sistema
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

}

/*
 * Metodo que realiza el movimiento de la torreta hacia abajo
 * EN EL EJE Y
 */
void MueveTorretaAbajo (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloquemos el MUTEX de la torreta
	flags_torreta &= (~FLAG_JOYSTICK_DOWN); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Desbloquemos el MUTEX de la torreta

	//Si se cumplen estas condiciones nos movemos hacia abajo
	if(p_torreta->posicion.y - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {
		p_torreta->posicion.y = p_torreta->posicion.y - p_torreta->servo_y.incremento;

	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y); //Actualizamos el valor de PWM en el pin dado.

	serialPrintf(fi,"    MOVE DOWN"); //Mandamos por serial el mensaje al LCD
	fflush(stdout);

	}
}

/*
 * Metodo que realiza el movimiento de la torreta hacia arriba
 * EN EL EJE Y
 */
void MueveTorretaArriba (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_JOYSTICK_UP); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de la torreta

	//Si se cumple esta condicion realizamos movimiento arriba
	if(p_torreta->posicion.y - p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {
		p_torreta->posicion.y = p_torreta->posicion.y + p_torreta->servo_y.incremento;

	//Actualizamos el valor de PWM en el pin dado.
	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y);

	serialPrintf(fi,"    MOVE UP"); //Mandamos por serial el mensaje al LCD
	fflush(stdout);

	}
}

/*
 * Metodo que realiza el movimiento de la torreta hacia la izquierda
 * EN EL EJE X
 */
void MueveTorretaIzquierda (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_JOYSTICK_LEFT); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Desbloqueamos el MUTEX de la torreta

	//Si se cumple esta condicion realizamos movimiento a la izquierda
	if(p_torreta->posicion.x - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {
		p_torreta->posicion.x = p_torreta->posicion.x - p_torreta->servo_x.incremento;

	//Actualizamos el valor de PWM en el pin dado.
	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	serialPrintf(fi,"    MOVE LEFT"); //Mandamos por serial el mensaje al LCD
	fflush(stdout);

	}
}

/*
 * Metodo que realiza el movimiento de la torreta hacia la derecha
 * EN EL EJE X
 */
void MueveTorretaDerecha (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_JOYSTICK_RIGHT); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

	//Si se cumple esta condicion realizamos movimiento a la derecha
	if(p_torreta->posicion.x - p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {
		p_torreta->posicion.x = p_torreta->posicion.x + p_torreta->servo_x.incremento;

	//Actualizamos el valor de PWM en el pin dado.
	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	serialPrintf(fi,"   MOVE RIGTH"); //Mandamos por serial el mensaje al LCD
	fflush(stdout);

	}
}


void DisparoIR (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_TRIGGER_BUTTON); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

	//Escribo en el pin un 1 si se produce el disparo
	digitalWrite(IR_TX_PIN, HIGH);

	serialPrintf(fd,"disparo");
	serialPrintf(fi," PIUM PIUM PIUM"); //Mandamos por serial el mensaje al LCD

	piLock(PLAYER_FLAGS_KEY); //Bloqueamos el MUTEX de la torreta
	//Inicializamos el timer con el time del p_torreta y le ponemos la duracion del disparo
	tmr_startms(p_torreta->tmr,p_torreta->duracion);
	flags_player |= FLAG_START_DISPARO; //Subimos el flag para que suene la musica del disparo
	piUnlock(PLAYER_FLAGS_KEY); //Bloqueamos el MUTEX de la torreta


	printf("\n[TORRETA][DisparoIR]\n");

}

void FinalDisparoIR (fsm_t* this) {

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_SHOOT_TIMEOUT); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

	//Escribo en el pin un 0 si finaliza el disparo
	digitalWrite(IR_TX_PIN, LOW);

	printf("\n[TORRETA][FinalDisparoIR]");

}

/*
 * Método que se realizará al detectar en el pin RX a HIGH haciendo que se reproduzca el sonido
 * del efecto impacto.
 */
void ImpactoDetectado (fsm_t* this) {

	TipoTorreta *p_torreta; //Creacion de un puntero TipoTorreta
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_TARGET_DONE); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

	digitalWrite(IR_TX_PIN, LOW);

	serialPrintf(fd,"impacto"); //Mandamos por serial el mensaje al arduino para que encienda
	serialPrintf(fi," PUFFFF"); //Mandamos por serial el mensaje al LCD

	piLock(PLAYER_FLAGS_KEY); //Bloqueamos el MUTEX de la torreta
	tmr_startms(p_torreta->tmr,p_torreta->duracion); //Timer duracion del tmr de torreta
	flags_player |= FLAG_START_IMPACTO; //Subimos el flag para que suene la musica del IMPACTO
	piUnlock(PLAYER_FLAGS_KEY); //Bloqueamos el MUTEX de la torreta

	printf("\n[TORRETA][ImpactoDetectado]");

}

/*
 * Metodo de finalizacion del juego
 */
void FinalizaJuego (fsm_t* this) {

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta &= (~FLAG_SYSTEM_END); //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

	printf("\n[TORRETA][FinalizaJuego]");
}

/*
 * Método que realiza RX de subir el flag cuando detecta el impacto
 */
void ImpactoRX(){

	piLock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta
	flags_torreta |= FLAG_TARGET_DONE; //Bajamos el flag
	piUnlock(TORRETA_FLAG); //Bloqueamos el MUTEX de la torreta

}
