#include "servo_rb.h"

#define SERVO_N SERVO0

void SERVO_RB_init(){
	//servoConfig(0, SERVO_ENABLE );
	//servoConfig(SERVO_N, SERVO_ENABLE_OUTPUT );
   gpioConfig( T_FIL1, GPIO_OUTPUT);
}

void SERVO_RB_girar(int16_t ang){
   uint16_t high = 0;
   uint8_t i;
	//servoWrite(SERVO_N, ang + 90);
   switch (ang){
      case 0: high = 1000; break;
      case 45: high = 1200; break;
      case -45: high = 800; break;
   }
   for (i=0;i<5;i++){
   gpioWrite(T_FIL1, 1);
   delayInaccurateUs(high);
   gpioWrite(T_FIL1, 0); 
   delayInaccurateUs(20000 - high);
   }
}