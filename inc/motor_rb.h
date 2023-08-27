#ifndef MOTOR_RB_H_
#define MOTOR_RB_H_

#include "sapi.h"

void MOTOR_RB_init();
void MOTOR_RB_parar();
void MOTOR_RB_retroceder();
void MOTOR_RB_avanzar();
void MOTOR_RB_setVelocidad(uint8_t);

#endif