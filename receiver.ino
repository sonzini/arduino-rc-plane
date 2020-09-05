//Proyecto de control remoto V3

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

//Definir los pines donde se conectan los botones al CD4021
//String botonPulsado[] = {"pin 7", "pin 6", "pin 5", "pin 4", "pin 13", "pin 14", "pin 15", "pin 1"};
  String botonPulsado[] = {"ABAJO", "IZQUIERDA", "DERECHA", "ARRIBA", "CLEAR", "L3", "L1", "L2"};
//String botonPulsado2[] = {"2-pin 7", "2-pin 6", "2-pin 5", "2-pin 4", "2-pin 13", "2-pin 14", "2-pin 15", "2-pin 1"};
  String botonPulsado2[] = {"TRIANGULO", "CIRCULO", "X", "CUADRADO", "R1", "R2", "SWITCH 2", "R3"};

byte switchVar1 = 72; //01001000  //Valor default. Se recomienda un valor diferente de 0 para evitar errores.
byte switchVar2 = 159; //10011111

void setup()
{
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop()
{
  if ( myRadio.available()) //Se encarga de leer los datos
    while (myRadio.available())
      myRadio.read( &data, sizeof(data) );
      
  switchVar1 = data.botones1;
  switchVar2 = data.botones2;

  for (int n=0; n<=7; n++)     
    if (switchVar1 & (1 << n) )
      Serial.println(botonPulsado[n]);

  for (int s=0; s<=7; s++)                                
    if (switchVar2 & (1 << s) )
      Serial.println(botonPulsado2[s]);

  data.LX = map(data.LX, 0, 1024, 10, -10);
  data.LY = map(data.LY, 0, 1024, -10, 10);
  data.RX = map(data.RX, 0, 1024, 10, -10);
  data.RY = map(data.RY, 0, 1024, -10, 10);

  if (data.LX >= -1 && data.LX <= 1) data.LX=0;
  if (data.LY >= -1 && data.LY <= 1) data.LY=0;
  if (data.RX >= -1 && data.RX <= 1) data.RX=0;
  if (data.RY >= -1 && data.RY <= 1) data.RY=0;

  Serial.print ("LX: ");
  Serial.print (data.LX);
  Serial.print ("  LY: ");
  Serial.print (data.LY);
  Serial.print ("   RX: ");
  Serial.print (data.RX);
  Serial.print ("  RY: ");
  Serial.println (data.RY);

  delayMicroseconds(20);
  delay (100);
  
} //-----------------------------------------------------------------//
  //-------------------------FIN DEL LOOP----------------------------//
  //-----------------------------------------------------------------//