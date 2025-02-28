/*
 * piTankGo_1.c
 *
 *  Created on: 8 abr. 2019
 *      Author: Javier Abejaro Capilla e Iris Rubio Saez
 */

#include "piTankGo_1.h"
#include <softPwm.h>
#include "player.h"
#include "torreta.h"
#include "joystick.h"
#include <wiringSerial.h>

int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};

//Efectos seleccionados para el juego
int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

//Variables a utilizar para la conexi�n serial Arduino UNO
int fd ;
int fi;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

/* int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion del sistema.
 *  Realizara, entra otras, todas las operaciones necesarias para:
 *  configurar el uso de posibles librerias (e.g. Wiring Pi),
 *  configurar las interrupciones externas asociadas a los pines GPIO,
 *  configurar las interrupciones periodicas y sus correspondientes temporizadores,
 *  crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
 */
int ConfiguraSistema (TipoSistema *p_sistema) {

	int result = 0;

	//piLock (STD_IO_BUFFER_KEY); //Bloquemos el MUTEX del buffer de impresion
	wiringPiSetupGpio (); /*Inicializa libreria wiringPi y supone que el programa de llamada usar� el esquema de numeraci�n de pin wiringPi.
	 * Permite que los programas de llamada utilicen los n�meros de pin Broadcom GPIO directamente sin volver a asignar*/
	pinMode(PLAYER_PWM_PIN, OUTPUT); //Establecemos el modo del pin dado como salida
	softToneCreate(PLAYER_PWM_PIN); //Creamos un pin de tono controlado por software
	softToneWrite(PLAYER_PWM_PIN,0); //Actualizamos el valor de frecuencia del tono en el pin dado a 0 para inicializarlo.
	//piUnlock (STD_IO_BUFFER_KEY); //Desbloquemos el MUTEX del buffer de impresion

	//Abrimos e inicializamos el dispositivo serie y establecemos la velocidad de transmisi�n del mismo.
	if ((fd = serialOpen ("/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0", 115200)) < 0){ //Asignamos esa ruta ya que es su ruta fija, no var�a
		fprintf (stderr, "Problema conexion serial: %s\n", strerror (errno)) ;
		return 1 ;
	}

	//Abrimos e inicializamos el dispositivo serie y establecemos la velocidad de transmisi�n del mismo.
	if ((fi = serialOpen ("/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_95335343136351D05271-if00", 115200)) < 0){ //Asignamos esa ruta ya que es su ruta fija, no var�a
		fprintf (stderr, "Problema conexion serial: %s\n", strerror (errno)) ;
		return 1 ;
	}

	//Descartamos todos los datos recibidos o esperamos a que se env�e el dispositivo dado
	void serialFlush (int fd);
	return 1;

	//Descartamos todos los datos recibidos o esperamos a que se env�e el dispositivo dado
	void serialFlush (int fi);
	return 1;

	return result;

}

/* Procedimiento de inicializacion del sistema.
 * Realizara, entra otras, todas las operaciones necesarias para:
 * la inicializacion de los diferentes elementos de los que consta nuestro sistema,
 * la torreta, los efectos, etc.
 * igualmente arrancara el thread de exploracion del teclado del PC
 */
int InicializaSistema (TipoSistema *p_sistema) {
	int result = 0;

	p_sistema->debug=0;
	p_sistema->teclaPulsada=' ';

	InicializaTorreta(&(p_sistema->torreta)); //Inicializamos la torreta
	InicializaEfecto(&(p_sistema->player.efecto_disparo),p_sistema->player.efecto_disparo.nombre,frecuenciasDisparo,tiemposDisparo,16); //Inicializamos el efecto disparo del sistema
	InicializaEfecto(&(p_sistema->player.efecto_impacto),p_sistema->player.efecto_impacto.nombre,frecuenciasImpacto,tiemposImpacto,32); //Inicializamos efecto impacto del sistema
	InicializaPlayer(&(p_sistema->player)); //Inicializamos antes player porque sino efecto disparo no se inicializa y hay violacion de segmento
	InicializaJoy(); //Inicializamos el joystick

	return result;
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main (){

	TipoSistema sistema; //Declaramos una variable de TipoSistema compuesta por sus diversos par�metros
	unsigned int next;
	ConfiguraSistema (&sistema); // Configuracion del sistema
	InicializaSistema (&sistema); //Inicializacion del sistema
	//InicializaTorreta(&sistema.torreta); //Inicializamos la torreta del sistema

	/*Declaracion de los estados y transiciones de la maquina de estados del PLAYER*/
	fsm_trans_t player[] = {
		{ WAIT_START, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
		{ WAIT_START, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
		{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
		{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
		{-1, NULL, -1, NULL },
	};

	/*Declaracion de los estados y transiciones de la maquina de estados de la TORRETA*/
	fsm_trans_t torreta[] = {

			{ WAIT_START, CompruebaComienzo, WAIT_MOVE, ComienzaSistema },
			{ WAIT_MOVE, CompruebaJoystickUp, WAIT_MOVE, MueveTorretaArriba},
			{ WAIT_MOVE, CompruebaJoystickLeft, WAIT_MOVE, MueveTorretaIzquierda},
			{ WAIT_MOVE, CompruebaJoystickRight, WAIT_MOVE, MueveTorretaDerecha},
			{ WAIT_MOVE, CompruebaJoystickDown, WAIT_MOVE, MueveTorretaAbajo},
			{ WAIT_MOVE, CompruebaFinalJuego, WAIT_END, FinalizaJuego},
			{ WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR},
			{ TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR},
			{ TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado },
			{-1, NULL, -1, NULL },
	};

	//Declaracion de las maquinas de estados con su estado de comiento, la maquina y el objeto
	fsm_t* player_fsm = fsm_new (WAIT_START, player, &(sistema.player));
	fsm_t* torreta_fsm = fsm_new (WAIT_START, torreta, &(sistema.torreta));

	next = millis();

	while (1) {
		fsm_fire (player_fsm); /*Definici�n de la funci�n que permite liberar la memoria utilizada por la m�quina de estado */
		fsm_fire (torreta_fsm);

		/*CLK_MS permite una frecuencia de actualizacion del estado del
		sistema alta para que los tiempos de respuesta sean
		bajos y no haya conflictos*/
		next += CLK_MS;
		delay_until (next); //Espero
	}

	//Destruimos el timer y las maquinas de estados
	tmr_destroy ((tmr_t*)(player_fsm->user_data));
	tmr_destroy ((tmr_t*)(torreta_fsm->user_data));
	fsm_destroy (player_fsm);
	fsm_destroy (torreta_fsm);

	return 0;

}
