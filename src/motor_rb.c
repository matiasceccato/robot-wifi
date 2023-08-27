#include "motor_rb.h"

#define IN1 GPIO1
#define IN2 GPIO3
#define ENA PWM3  // TFIL2

// Voltajes minimo y maximo de trabajo del motor
#define V_MAX 9.0   // Para no sobrecargar la bateria
#define V_MIN 3

void MOTOR_RB_init(){
   // Setear GPIO1 como salida
   gpioInit(IN1, GPIO_OUTPUT);
   // Setear GPIO3 como salida
   gpioInit(IN2, GPIO_OUTPUT);
   
   // Setear PWM3 como salida de PWM
   pwmConfig( 0, PWM_ENABLE );
   pwmConfig(ENA, PWM_ENABLE_OUTPUT);
   
   // Inicializar el motor detenido
   MOTOR_RB_parar();
}

/*
Funcionamiento del puente H L298N:
      +-----+-----+
      | IN1 | IN2 |
      +-----+-----+
      |  0  |  0  |  -->  Motor detenido
      +-----+-----+
      |  0  |  1  |  -->  Avanza en direccion A
      +-----+-----+
      |  1  |  0  |  -->  Avanza en direccion B
      +-----+-----+
      |  1  |  1  |  -->  Motor detenido
      +-----+-----+

*/

void enviarVelocidad(uint8_t perc){
   // Transformar porcentaje a voltaje
   double volt = ((double) perc / 100) * (V_MAX - V_MIN) + V_MIN;
   // Escribir el numero transformandolo al rango 0 - 255
   pwmWrite(ENA, (volt / V_MAX) * 255);
}

void MOTOR_RB_parar(){
   // Escribir 0 en IN1
   gpioWrite(IN1, OFF);
   // Escribir 0 en IN2
   gpioWrite(IN2, OFF);
   //Setear a 0 la velocidad
   MOTOR_RB_setVelocidad(0);
}

void MOTOR_RB_retroceder(){
   // Escribir 0 en IN1
   gpioWrite(IN1, OFF);
   // Escribir 1 en IN2
   gpioWrite(IN2, ON);
}

void MOTOR_RB_avanzar(){
   // Escribir 1 en IN1
   gpioWrite(IN1, ON);
   // Escribir 0 en IN2
   gpioWrite(IN2, OFF);
}

void MOTOR_RB_setVelocidad(uint8_t perc){
   // Recibe un numero de 0 a 100
   
   /*static uint8_t lastVel = 0;

   // Evitar salto grande de velocidad
   if(perc - lastVel > 50){
      enviarVelocidad(perc / 2);
      delay(50);
   }

   lastVel = perc;*/
   enviarVelocidad(perc);
}
   
