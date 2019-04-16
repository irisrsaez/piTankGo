#include "piTanKGo_1.h"
#include "player.h"
#include <string.h>
#include "tmr.h"
#include "fsm.h"
#include "joystick.h"
//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

	int i=0;
	p_efecto->num_notas = num_notas;
	// strcpy: terminal raspi escribo: man 3 strcpy
	strcpy(p_efecto->nombre, nombre);
	//printf("Dura %i %i\n",array_duraciones[0],num_notas);
	for ( i = 0; i < num_notas; i++ ){
		p_efecto ->frecuencias[i]=array_frecuencias[i];
		p_efecto ->duraciones[i]=array_duraciones[i];
	}
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicializacion comun a InicializaPlayDisparo y InicializaPlayImpacto
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
	//printf("\n[PLAYER][InicializaPlayer]\n");

	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);

	tmr_startms(p_player->tmr,(p_player->duracion_nota_actual));
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO); //Devuelve 1 o 0 segun el bit en cuestion
	piUnlock (PLAYER_FLAGS_KEY);
	//printf("\n[PLAYER][CompruebaStartDisparo]\n");

	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock (PLAYER_FLAGS_KEY);
	//printf("\n[PLAYER][CompruebaStartImpacto]\n");

	return result;

}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);
	//printf("\n[PLAYER][CompruebaNuevaNota]\n");

	return !result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY);
	//printf("\n[PLAYER][CompruebaNotaTimeout]\n");

	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;

	piLock (PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);
	//printf("\n[PLAYER][CompruebaFinalEfecto]\n");

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO; //Lo pone a 0
	piUnlock (PLAYER_FLAGS_KEY);

	TipoPlayer *p_player;

	p_player = (TipoPlayer*)(this->user_data);
	//Inicializo efecto disparo
	p_player->p_efecto = &(p_player->efecto_disparo);

	//Ponemos a través del pin 19 la onda de nuestro efecto
	//softToneWrite(pin,frecuencia)
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);//Esto actualiza el valor de frecuencia de tono en el pin dado. El tono se reproducirá hasta que establezca la frecuencia en 0.

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayDisparo][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY);

	//Inicializamos el timer con el time del p_player y le ponemos la duracion de la nota
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
}

void InicializaPlayImpacto (fsm_t* this) {

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO; //Lo pone a 0
	piUnlock (PLAYER_FLAGS_KEY);

	TipoPlayer *p_player;

	p_player = (TipoPlayer*)(this->user_data);
	p_player->p_efecto = &(p_player->efecto_impacto);

	//Ponemos a traves del pin 19 la onda de nuestro efecto
	//softToneWrite(pin,frecuencia)
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);//Esto actualiza el valor de frecuencia de tono en el pin dado. El tono se reproducir� hasta que establezca la frecuencia en 0.

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayImpacto][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY);

	//Inicializamos el timer con el time del p_player y le ponemos la duracion de la nota
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
}

void ComienzaNuevaNota (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	//player->melodia->frecuencias[player->posicion_nota_actual]);
	softToneWrite(PLAYER_PWM_PIN,p_player->p_efecto->frecuencias[p_player->posicion_nota_actual]);//Esto actualiza el valor de frecuencia de tono en el pin dado. El tono se reproducirá hasta que establezca la frecuencia en 0.

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][ComienzaNuevaNota][Nota %d][Frec %d][Dura %d]\n",p_player->posicion_nota_actual,p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY);

	//Inicializamos el timer con el time del p_player y le ponemos la duracion de la nota
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);//Iniciamos el timer llamando a timer_isr

}

void ActualizaPlayer (fsm_t* this) {
	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	if(p_player->p_efecto->num_notas-1>=p_player->posicion_nota_actual){

		p_player->posicion_nota_actual++;
		p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
		p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

		piLock(STD_IO_BUFFER_KEY);
		printf("\n[PLAYER][ActualizaPlayer][NUEVA NOTA (%d de %d)]",p_player->posicion_nota_actual,p_player->p_efecto->num_notas);
		piUnlock(STD_IO_BUFFER_KEY);

	}else{

		piLock(PLAYER_FLAGS_KEY);
		flags_player |= FLAG_PLAYER_END;//encedemos END porque hemos acabado la cancion y queremos volver a WAIT_START
		flags_player &= ~FLAG_NOTA_TIMEOUT;
		piUnlock(PLAYER_FLAGS_KEY);

	}
}

//Acaba con el efecto disparo asique todo a 0
void FinalEfecto (fsm_t* this) {

	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	softToneWrite(PLAYER_PWM_PIN,0);

	piLock(STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][FinalEfecto]\n");
	piUnlock(STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
}
