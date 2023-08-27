/* Copyright 2022, Agustin Barbosa.
 * Copyright 2022, Matias Ceccato.
 * All rights reserved.
 *
 * This file is part of the RoadBeast project in collaboration with Demoro, Gil and Parodi.
/*
 * Date: 2022-10-05
 */

/* 
ESP01 (ESP8266) connections:

   +--------------------------------------+
   |  |          +----+                   |           
   |  +--+  |    |    |      RX o o VCC   |
   |     |  |    +----+   GPIO0 o o RST   |         
   |  +--+  |    +----+   GPIO2 o o CH_PD |
   |  |     |    |    |     GND o o TX    |
   |  +-----+    +----+                   |
   +--------------------------------------+

   VCC ESP8266 <--> +3.3V EDU-CIAA-NXP
   RST ESP8266 <--> (SIN CONEXION)
 CH_PD ESP8266 <--> +3.3V EDU-CIAA-NXP
    TX ESP8266 <--> RX EDU-CIAA-NXP

    RX ESP8266 <--> TX EDU-CIAA-NXP
 GPIO0 ESP8266 <--> (SIN CONEXION)
 GPIO0 ESP8266 <--> (SIN CONEXION)
   GND ESP8266 <--> GND EDU-CIAA-NXP
*/

/*==================[inclusions]=============================================*/
#include "sapi.h"
#include "wifi.h"

volatile unsigned char rxFlag = 0;
volatile unsigned char adelante = 0;
volatile unsigned char atras = 0;
volatile unsigned char giro = 0;
volatile unsigned char velocidad = 0;
volatile unsigned char angulo = 0;
volatile unsigned char stop = 0;
static uint8_t save = 0;
static uint8_t cadena[50];
static uint8_t i;
static uint8_t cont = 0;

void analizarEntrada(uint8_t dato){

    if (save){
        cadena[i++] = dato;
    }
    if (dato == '?'){
        save = !save;
        cont++;
        if (cont == 2) {
         procesar(cadena);
         cont = 0;
         i = 0;
        }
    }
    //uartWriteByte( UART_USB, dato );
    
}

int espConfig(void){
      uartConfig(UART_USB, 115200);
      uartConfig(UART_232, 115200);
      uint8_t todoOk = 1;
      uint8_t pwmVal = 100;

      //Prendemos LED ROJO 
      gpioWrite( T_COL1, 1);
      gpioWrite( T_FIL3, 0);
      delay(1000);
      uartWriteString( UART_USB, "> Configurando ESP8266-01\r\n" );
      // Modo 3, configurado como AcessPoint
      if(todoOk){
         uartWriteString( UART_232, "AT+CWMODE=3\r\n" );
         validar(&todoOk);
      }
      // Seteo de la Red, seguridad WPA2_CK
      if(todoOk){
         uartWriteString( UART_232, "AT+CWSAP=\"RoadBeast\",\"12345678\",11,3,3\r\n" );
         validar(&todoOk);
      }
      // Habilitacion de multiples conexiones
      if(todoOk){
         uartWriteString( UART_232, "AT+CIPMUX=1\r\n" );
         validar(&todoOk);
      }
      // Habilitacion de servidor en puerto 80
      if(todoOk){
         uartWriteString( UART_232, "AT+CIPSERVER=1,80\r\n" );
         validar(&todoOk);
      } 
      // Si salio todo bien, devolvemos un 1
      if(todoOk) {
         uartWriteString( UART_USB, "> ESP8266-01 configurado correctamente\r\n" );
         //Prender LED AMARILLO
         gpioWrite( T_COL1, 0);
         gpioWrite( T_FIL0, 1);
         pwmWrite( PWM1, pwmVal );
         delay(750);
         pwmWrite( PWM1, 0 );
         
         return 0;
      }else{
         uartWriteString( UART_USB, "> Error en la configuracion del  ESP8266-01\r\n" );
         //Prender LED ROJO
         return 1;
      }
}

void validar(uint8_t *todoOk){
   uint8_t ready = 0;
   uint8_t dato  = 0;
   uint8_t ant = 0;
   uint8_t antant = 0;

   while(!ready){
      antant = ant;
      ant = dato;
      uartReadByte( UART_232 , &dato );
      uartWriteByte( UART_USB , dato);
      if( (antant == 'E') && (ant == 'R') && (dato == 'R') ){
         *todoOk = 0;
         ready = 1;
      }
      if((ant == 'O') && (dato == 'K') ){
         ready = 1;
      }
   }

}

void procesar(uint8_t cadena[50]){
   uint8_t j = 0;
   uint8_t tmp = 15; //max 12 caracteres aprox
   uint8_t act = 1 ;
   uint8_t aux = 0;
   if ( strstr( cadena , "adelante" ) != NULL ) adelante = 1;
   else  if ( strstr( cadena , "atras" ) != NULL ) atras = 1;
         else  if (strstr( cadena, "giro") != NULL) giro = 1;
               else  if (strstr( cadena, "stop") != NULL) stop = 1;
                     else act = 0;
   
   if (adelante){
      while(cadena[j] != '!' && tmp) {j++; tmp--;}
      velocidad = ((cadena[j + 1] - 48) * 10) + (cadena[j + 2] - 48);
      j = 0;
      aux = velocidad;
      if(velocidad == 90) uartWriteString( UART_USB, "es 90\r\n" );
    }
    if (giro){
      while(cadena[j] != '!' && tmp) {j++; tmp--;}
      angulo = ((cadena[j + 1] - 48) * 100) + ((cadena[j + 2] - 48)* 10) + (cadena[j + 3] - 48);
      j = 0;
      aux = angulo;
      if(angulo == 75)uartWriteString( UART_USB, "es 75\r\n" );
   }
   if (act){ 
      delay(10); 
      closeCone();
      uartWriteString( UART_USB, cadena );
      uartWriteString( UART_USB, "\r\n" );
      }
}

void closeCone(void){
   uartWriteString( UART_232, "AT+CIPCLOSE=0\r\n");
   delay(10); 
   uartWriteString( UART_232, "AT+CIPCLOSE=1\r\n");
   
}

/* GETTERS & SETTERS*/

uint8_t getAdelante(){
   return adelante;
}

uint8_t getAtras(){
   return atras;
}

uint8_t getGiro(){
   return giro;
}

uint8_t getVelocidad(){
   return velocidad;
}

uint8_t getAngulo(){
   return angulo;
}

uint8_t getRxFlag(){
   return rxFlag;
}

uint8_t getStop(){
   return stop;
}

void setAdelante(uint8_t aux){
   adelante=aux;
}

void setAtras(uint8_t aux){
   atras=aux;
}

void setGiro(uint8_t aux){
   giro=aux;
}

void setVelocidad(uint8_t aux){
   velocidad=aux;
}

void setAngulo(uint8_t aux){
   angulo=aux;
}

void setRxFlag(uint8_t aux){
   rxFlag=aux;
}
void setStop(uint8_t aux){
   stop=aux;
}

/*==================[end of file]============================================*/