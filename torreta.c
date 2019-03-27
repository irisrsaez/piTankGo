#include <softPwm.h>
#include "torreta.h"
#include "piTankGoLib.h"

int flags_torreta = 0;

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {

	wiringPiSetupGpio();

	//Inicializo x e y de la misma manera ambos
	p_torreta->servo_x.incremento = TORRETA_INCREMENTO;
	p_torreta->servo_y.incremento = TORRETA_INCREMENTO;
	p_torreta->servo_x.minimo = TORRETA_MINIMO;
	p_torreta->servo_y.minimo = TORRETA_MINIMO;
	p_torreta->servo_x.maximo = TORRETA_MAXIMO;
	p_torreta->servo_y.maximo = TORRETA_MAXIMO;

	p_torreta->servo_x.inicio 	= TORRETA_MINIMO + (TORRETA_MAXIMO - TORRETA_MINIMO)/2;
	p_torreta->servo_y.inicio 	= TORRETA_MINIMO + (TORRETA_MAXIMO - TORRETA_MINIMO)/2;

	p_torreta->posicion.x	= p_torreta->servo_x.inicio;
	p_torreta->posicion.y	= p_torreta->servo_y.inicio;

	if(p_torreta->posicion.x > p_torreta->servo_x.maximo)
		p_torreta->posicion.x = p_torreta->servo_x.maximo;
	if(p_torreta->posicion.y > p_torreta->servo_y.maximo)
			p_torreta->posicion.y = p_torreta->servo_y.maximo;

	if(p_torreta->posicion.x < p_torreta->servo_x.minimo)
			p_torreta->posicion.x = p_torreta->servo_x.minimo;
	if(p_torreta->posicion.y < p_torreta->servo_y.minimo)
				p_torreta->posicion.y = p_torreta->servo_y.minimo;

	softPwmCreate (TORRETA_PIN_PWM_X, p_torreta->servo_x.inicio, TORRETA_PWM_RANGE);
	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	softPwmCreate (TORRETA_PIN_PWM_Y, p_torreta->servo_y.inicio, TORRETA_PWM_RANGE);
	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y);

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_SYSTEM_START);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_JOYSTICK_UP);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_JOYSTICK_DOWN);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_JOYSTICK_LEFT);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_JOYSTICK_RIGHT);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_SHOOT_TIMEOUT);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_TARGET_DONE);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_TRIGGER_BUTTON);
	piUnlock(TORRETA_FLAG);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;

	piLock(TORRETA_FLAG);
	result = (flags_torreta & FLAG_SYSTEM_END);
	piUnlock(TORRETA_FLAG);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

//Esto falta pero no sé que tiene que hacer je je
void ComienzaSistema (fsm_t* this) {

	piLock(TORRETA_FLAG);
	flags_torreta |= FLAG_SYSTEM_START;
	piUnlock(TORRETA_FLAG);
	printf("COMIENZA SISTEMA\n");


}

//Se mueve arriba y abajo en el EJE Y
void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG);
	flags_torreta &= (~FLAG_JOYSTICK_UP);
	piUnlock(TORRETA_FLAG);

	if(p_torreta->posicion.y - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {
		p_torreta->posicion.y = p_torreta->posicion.y - p_torreta->servo_y.incremento;

	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y);

	printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.y);
	fflush(stdout);
	}
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG);
	flags_torreta &= (~FLAG_JOYSTICK_DOWN);
	piUnlock(TORRETA_FLAG);

	if(p_torreta->posicion.y - p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {
		p_torreta->posicion.y = p_torreta->posicion.y + p_torreta->servo_y.incremento;

	softPwmWrite(TORRETA_PIN_PWM_Y, p_torreta->posicion.y);

	printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.y);
	fflush(stdout);
	}
}

//Izquierda y derecha es en el EJE X
void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG);
	flags_torreta &= (~FLAG_JOYSTICK_LEFT);
	piUnlock(TORRETA_FLAG);

	if(p_torreta->posicion.x - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {
		p_torreta->posicion.x = p_torreta->posicion.x - p_torreta->servo_x.incremento;

	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.x);
	fflush(stdout);
	}
}

//Izquierda y derecha es en el EJE X
void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(TORRETA_FLAG);
	flags_torreta &= (~FLAG_JOYSTICK_RIGHT);
	piUnlock(TORRETA_FLAG);

	if(p_torreta->posicion.x - p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {
		p_torreta->posicion.x = p_torreta->posicion.x + p_torreta->servo_x.incremento;

	softPwmWrite(TORRETA_PIN_PWM_X, p_torreta->posicion.x);

	printf("[TORRETA][POSICION]=[%d]\n", p_torreta->posicion.x);
	fflush(stdout);
	}
}


//Todavia NO!!!
void DisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalDisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void ImpactoDetectado (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalizaJuego (fsm_t* this) {
	// A completar por el alumno
	// ...
}
