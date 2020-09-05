//Proyecto de control remoto V4

#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

struct package //Define el contenido de paquete a enviar
{
  byte botones1 = 72;    //01001000, Valor default
  byte botones2 = 159;   //10011111
  int LX;
  int LY;
  int RX;
  int RY;
};

typedef struct package Package;
Package data;

//Definir los pines donde se conectan los CD4021
int latchPin = 7;      //Ultimo pin del IC
int dataPin = 8;       //Tercer pin del IC
int clockPin = 6;      //Penultimo pin del IC

//Definir los pines donde se conectan los botones al CD4021
//String botonPulsado[] = {"pin 7", "pin 6", "pin 5", "pin 4", "pin 13", "pin 14", "pin 15", "pin 1"};
  String botonPulsado[] = {"ABAJO", "IZQUIERDA", "DERECHA", "ARRIBA", "CLEAR", "L3", "L1", "L2"};
//String botonPulsado2[] = {"2-pin 7", "2-pin 6", "2-pin 5", "2-pin 4", "2-pin 13", "2-pin 14", "2-pin 15", "2-pin 1"};
  String botonPulsado2[] = {"TRIANGULO", "CIRCULO", "X", "CUADRADO", "R2", "R1", "SWITCH 2", "R3"};

byte switchVar1 = 72; //01001000  //Valor default. Se recomienda un valor diferente de 0 para evitar errores.
byte switchVar2 = 159; //10011111

//Definir los pines de los analógicos
int pinLY = A0;
int pinLX = A1;
int pinRY = A2;
int pinRX = A3;

void setup()
{
  Serial.begin(9600);

  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, INPUT);
}

void loop()
{
  digitalWrite(latchPin,1);      //Pone el valor en "1" para recolectar los datos
  delayMicroseconds(20);         //Tiempo que necesita para recolectar los datos  
  digitalWrite(latchPin,0);      //Pone el valor en "0" para transmitir los datos recolectados

  switchVar1 = shiftIn(dataPin, clockPin);  //Carga en la variable, el byte con el estado de cada boton
  switchVar2 = shiftIn(dataPin, clockPin);

//  Serial.println(switchVar1, BIN);  //Solo para control. Muestra el Byte en el monitor
//  Serial.println(switchVar2, BIN);

  for (int n=0; n<=7; n++)     
    if (switchVar1 & (1 << n) )
      Serial.println(botonPulsado[n]);

  for (int s=0; s<=7; s++)                                
    if (switchVar2 & (1 << s) )
      Serial.println(botonPulsado2[s]);

  data.botones1 = switchVar1;
  data.botones2 = switchVar2;

  data.LX = analogRead(pinLX);//map(analogRead(pinLX),0,800,200,-200);
  data.LY = analogRead(pinLY);//map(analogRead(pinLY),0,800,-200,200);
  data.RX = analogRead(pinRX);//map(analogRead(pinRX),0,800,200,-200);
  data.RY = analogRead(pinRY);//map(analogRead(pinRY),0,800,-200,200);

  //if (data.LX>-15 && data.LX<15) data.LX=0;
  //if (data.LY>-15 && data.LY<15) data.LY=0;
  //if (data.RX>-15 && data.RX<15) data.RX=0;
  //if (data.RY>-15 && data.RY<15) data.RY=0;
/*
  Serial.print ("LX: ");
  Serial.print (data.LX);
  Serial.print ("  LY: ");
  Serial.print (data.LY);
  Serial.print ("   RX: ");
  Serial.print (data.RX);
  Serial.print ("  RY: ");
  Serial.println (data.RY);
*/
  myRadio.write(&data, sizeof(data));
  
  delay (50);
  
} //-----------------------------------------------------------------//
  //-------------------------FIN DEL LOOP----------------------------//
  //-----------------------------------------------------------------//


//Esto se encarga de tomar el estado de los pines y pasarlo al Byte. Recomiendo no tocar
byte shiftIn(int myDataPin, int myClockPin)
{ 
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);

  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp)
    {
      pinState = 1;
      myDataIn = myDataIn | (1 << i);
    }
    else pinState = 0;

    digitalWrite(myClockPin, 1);
  }return myDataIn;
}