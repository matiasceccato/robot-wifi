#include "hcsr04_rb.h"

#define TRIG GPIO7
#define ECHO GPIO8

void HCSR04_RB_init(){
	gpioInit(TRIG, GPIO_OUTPUT);
	gpioInit(ECHO, GPIO_INPUT);

	// Inicializar en BAJO
	gpioWrite(TRIG, OFF);
}

double HCSR04_RB_medir(){
	uint16_t us = 0;

	// Enviar pulso de 10us
	gpioWrite(TRIG, ON);
	delayInaccurateUs(10);
	gpioWrite(TRIG, OFF);

	// Medir tiempo de pulso de respuesta
	while(!gpioRead(ECHO));
	while(gpioRead(ECHO)){
		delayInaccurateUs(1);
		us++;
	}

	// Calcular distancia medida
	return ((double) us / 58);
}