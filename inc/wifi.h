#ifndef WIFI_H_
#define WIFI_H_
#include <stdio.h>
#include <string.h>

int espConfig(void);
void validar(uint8_t *);
void procesar(uint8_t [50]);
void closeCone(void);
void analizarEntrada(uint8_t);

uint8_t getAdelante(void);
uint8_t getAtras(void);
uint8_t getGiro(void);
uint8_t getVelocidad(void);
uint8_t getAngulo(void);
uint8_t getRxFlag(void);
uint8_t getStop(void);

void setAdelante(uint8_t);
void setAtras(uint8_t);
void setGiro(uint8_t);
void setVelocidad(uint8_t);
void setAngulo(uint8_t);
void setRxFlag(uint8_t);
void setStop(uint8_t);


#endif