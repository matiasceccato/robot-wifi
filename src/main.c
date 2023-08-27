#include "wifi.h"
#include "sapi.h"
#include "motor_rb.h"
#include "servo_rb.h"
#define BAUD_RATE 115200

void rxInt(void *);
uint8_t dato;

int main(void){
    boardConfig();
   
   SERVO_RB_init();
   /* SERVO_RB_girar(45);
    delay(1000);
    SERVO_RB_girar(0);
    delay(1000);*/
    SERVO_RB_girar(0);

    pwmConfig( 0, PWM_ENABLE );
    pwmConfig( PWM1, PWM_ENABLE_OUTPUT );
    gpioConfig( T_FIL0, GPIO_OUTPUT);
    gpioConfig( T_FIL3, GPIO_OUTPUT);
    gpioConfig( T_COL1, GPIO_OUTPUT);
   
    uartConfig(UART_232, 115200);
   
    while (espConfig()) delay(5000);
    gpioWrite( T_FIL0, 0);
    gpioWrite( T_FIL3, 1);

    MOTOR_RB_init();
    closeCone();
    

    while (1){
        if (uartReadByte(UART_232, &dato)){
            analizarEntrada(dato);
        }
        if(getAdelante()){
           MOTOR_RB_parar();
           delay(1000);
           MOTOR_RB_avanzar();
           MOTOR_RB_setVelocidad(50);
           setAdelante(0);
        }
        if(getAtras()){
            MOTOR_RB_parar();
            delay(1000);
            MOTOR_RB_setVelocidad(50);
            MOTOR_RB_retroceder();
            setAtras(0);
        }
        if(getGiro()){
            uartWriteString( UART_USB, "entro\r\n" );
            SERVO_RB_girar(-45);
            delayInaccurateMs(1000);
            SERVO_RB_girar(45);
            setGiro(0);
            
        }   
        if(getStop()){
            MOTOR_RB_parar();
            setStop(0);
        }
    }
}