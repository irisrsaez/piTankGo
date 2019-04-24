#include "piTanKGo_1.h"
#include "player.h"
#include <string.h>
#include "tmr.h"
#include "fsm.h"
#include "joystick.h"
#include <wiringSerial.h>

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

	int i=0;
	p_efecto->num_notas = num_notas;
	strcpy(p_efecto->nombre, nombre);

	for ( i = 0; i < num_notas; i++ ){
		p_efecto ->frecuencias[i]=array_frecuencias[i];
		p_efecto ->duraciones[i]=array_duraciones[i];
	}

	return p_efecto->num_notas;

}

/*
 * Metodo que inicializa el player y todos sus parametros especificos
 * POSICION, FRECUENCIA, DURACION, EFECTO Y TIMER
 */
void InicializaPlayer (TipoPlayer *p_player) {

	//Creamos e inicializamos el timer
	tmr_t* tmr = tmr_new(timer_player_duracion_nota_actual_isr);
	p_player->tmr=tmr;

	//Inicializamos p_efecto para que apunte a cualquiera de los efectos
	p_player->p_efecto = &(p_player->efecto_disparo);

	//Inicializamos todos los parametros de un tipoPlayer
	p_player->posicion_nota_actual=0;
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	//Actualizamos el valor de frecuencia del tono en el pin dado
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);

	//Empieza el timer
	tmr_startms(p_player->tmr,(p_player->duracion_nota_actual));
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/*
 * Metodo comprobacion del flag del disparo
 */
int CompruebaStartDisparo (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	return result;
}

/*
 * Metodo de comprobacion del flag de impacto
 */
int CompruebaStartImpacto (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	return result;

}

/*
 * Metodo de comprobacion del flag de una nueva nota
 */
int CompruebaNuevaNota (fsm_t* this){

	int result = 0;

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	return !result; //Lo negamos ya que el mismo flag rige dos estados

}

/*
 * Metodo de comprobacion del flag del timer
 */
int CompruebaNotaTimeout (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	return result;

}

/*
 * Metodo de comprobacion del flag de final de efecto
 */
int CompruebaFinalEfecto (fsm_t* this) {

	int result = 0;

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	return result;

}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/*
 * Metodo de iniciacion del disparo
 */
void InicializaPlayDisparo (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	flags_player &= ~FLAG_START_DISPARO; //Bajamos el flag
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	TipoPlayer *p_player;

	p_player = (TipoPlayer*)(this->user_data);
	//Inicializo efecto disparo
	p_player->p_efecto = &(p_player->efecto_disparo);

	//Actualizamos el valor de frecuencia del tono en el pin dado
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);

	piLock (STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
	printf("\n[PLAYER][InicializaPlayDisparo][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

	//Empieza el timer y le ponemos la duracion de la nota actual
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);

}

/*
 * Metodo de inicializacion del impacto
 */
void InicializaPlayImpacto (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	flags_player &= ~FLAG_START_IMPACTO; //Bajamos el flag
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

	TipoPlayer *p_player;

	p_player = (TipoPlayer*)(this->user_data);
	p_player->p_efecto = &(p_player->efecto_impacto);

	//Actualizamos el valor de frecuencia del tono en el pin dado
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);

	piLock (STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
	printf("\n[PLAYER][InicializaPlayImpacto][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

	//Inicializamos el timer con el time del p_player y le ponemos la duracion de la nota
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
}

/*
 * Metodo que comienza una nueva nota
 */
void ComienzaNuevaNota (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	//Actualizamos el valor de frecuencia del tono en el pin dado
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);

	piLock (STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
	printf("\n[PLAYER][ComienzaNuevaNota][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

	//Inicializamos el timer con el time del p_player y le ponemos la duracion de la nota
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);

}

/*
 * Metodo que actualiza la nota
 */
void ActualizaPlayer (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	//Seguimos ejecutando la cancion si se cumple la condicion
	if(p_player->p_efecto->num_notas-1>=p_player->posicion_nota_actual){

		p_player->posicion_nota_actual++; //Incrementamos
		p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual]; //Actualizamos la nueva duracion
		p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]; //Actualizamos la nueva frecuencia

		piLock(STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
		printf("\n[PLAYER][ActualizaPlayer][NUEVA NOTA (%d de %d)]",p_player->posicion_nota_actual,p_player->p_efecto->num_notas);
		piUnlock(STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

	}else{ //Si no se cumple la condicion acabo

		piLock(PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
		flags_player |= FLAG_PLAYER_END; //Subimos el flag para acabar el juego
		flags_player &= ~FLAG_NOTA_TIMEOUT; //Bajamos el flag de procedimiento de atencion a las interrupciones
		piUnlock(PLAYER_FLAGS_KEY); //Desbloqueamos el MUTEX del player

	}
}

/*
 * Metodo de finalizacion del efecto
 */
void FinalEfecto (fsm_t* this) {

	piLock(PLAYER_FLAGS_KEY); //Bloquemos el MUTEX
	flags_player &= ~FLAG_PLAYER_END; //Bajamos el flag
	piUnlock(PLAYER_FLAGS_KEY); //Desbloqueamos el MUTEX

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	//Actualizamos el valor de frecuencia de tono en el pin dado. Lo ponemos a 0.
	softToneWrite(PLAYER_PWM_PIN,0);

	piLock(STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
	printf("\n[PLAYER][FinalEfecto]\n");
	piUnlock(STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

/*
 * Metodo del timer
 */
void timer_player_duracion_nota_actual_isr (union sigval value) {

	piLock (PLAYER_FLAGS_KEY); //Bloquemos el MUTEX del player
	flags_player |= FLAG_NOTA_TIMEOUT; //Subimos flag del timer
	piUnlock (PLAYER_FLAGS_KEY); //Desbloquemos el MUTEX del player

}
