#include <stdio.h>
#include <math.h>
//#include <LiquidCrystal.h>   //modo diagnóstico
//anular para ahorrar memoria si no está en modo diagnóstico
//LiquidCrystal lcd(1, 2, 4, 5, 6, 7);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Antirrebote pulsador, SELECTOR DE PROGRAMA//
int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;
int led6 = 8;
int led7 = 7;
int MFET = 5;
//Antirrebote pulsador, SELECTOR DE PROGRAMA//
// declaracion de constantes. Estas son usadas para configurar el numero de pin
const int buttonPin = 6; // el número del pin para la entrada del pulsador

int i=0;
// declaracion de variables:
int ledState = HIGH; // el estado actual del pin de salida
int buttonState; // la lectura actual del pin de entrada
int lastButtonState = LOW; // lectura previa del pin de entrada
long lastDebounceTime = 0; // la ultima vez que el pin de salida cambio
long debounceDelay = 50;

//FIN SELECTOR DE PROGRAMA
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//Programas de control
//
//45º----->1
//55º----->2
//65º----->3
//75º----->4
//85º----->5
//Max----->6
//

int prog[6]={45,55,65,80,90,100};  //VALORES DEL SELECTOR DE PROGRAMA en ºC
//insertamos el selector de programas para p[i]

//LECTURA DEL NTC 100K ohm 3950 compradas en Aliexpres
int analogPin=0;     // Pin donde esta conectado el divisor de tension en el Arduino
 
float Vin=5.0;       // [V]       Voltage de entrada en el divisor de tension
float Raux=10000;    // [ohm]     Valor de la resistencia secundaria del divisor de tension
float R0=100000;     // [ohm]     Valor de resistencia nominal del termistor (NTC) a 25ºC
float T0=298.15;     // [K] (25ºC)
 
float Vout=0.0;      // [V]        Voltage given by the Voltage-Divider
float Rout=0.0;      // [ohm]      Resistencia actual del Termistor (NTC)
 
// Valores para calcular el valor Beta, si no lo sabemos
// float T1=273;     // [K]        Temperatura del 1º punto del test (en grados Kelvin)
// float T2=373;     // [K]        Temperatura del 2º punto del test (en grados Kelvin)
// float RT1=19750;  // [ohms]     Resistencia a 273K grados Kelvin (0ºC)
// float RT2=2150;   // [ohms]     Resistencia a 373K grados Kelvin (100ºC)
 
float beta=0.0;      // [K]        Parametro Beta
float Rinf=0.0;      // [ohm]      Parametros Rinf
float TempK=0.0;     // [K]        Temperatura de salida en grados Kelvin
float TempC=0.0;     // [ºC]       Temperatura de salida en grados Celsius
int TempCmax=0;
int TempCmin=0;
 
int iCont;            // Contador de ciclos, par el calculo de la temperatura media
float cTemp1;         // Variable temporal para acumular las temperaturas leidas 
 
void setup() {
  // Configuramos el puerto Serie
//esta parte será para conectar el lcd 16x2        
  //lcd.begin(16,2);
  //lcd.print("  TEMP Start"); // Imprime "LCD Tutorial" sobre el LCD
  //lcd.setCursor(0,1); // Seteamos la ubicacion texto 0 linea 1 que sera escrita sobre el LCD
  //lcd.print("     Dana");
  //delay(4000);
  //lcd.clear(); // Limpia la pantalla
  //Selector de programa
Serial.begin (9600);
pinMode(led1, OUTPUT); 
pinMode(led2, OUTPUT); 
pinMode(led3, OUTPUT); 
pinMode(led4, OUTPUT); 
pinMode(led5, OUTPUT); 
pinMode(led6, OUTPUT); 
pinMode(led7, OUTPUT);
pinMode(MFET, OUTPUT);

//SELECTOR DE PROGRAMA
      pinMode(buttonPin, INPUT);
     


  //SELECTOR DE PROGRAMA   
 
  // Configuramos el pin del Arduino en entrada
  pinMode(analogPin, INPUT);
 
  // Parametros generales para el calculo
  // Formula para calcular el valor beta si no disponemos de el
  // beta=(log(RT1/RT2))/((1/T1)-(1/T2));
  // Valor beta, los fabricantes suelen disponer ya de este valor, 
  // mirar en la tabla de carateristicas del termistor
  beta = 3950;
  Rinf=R0*exp(-beta/T0);
}
 
void loop()
{
 
  cTemp1=0;  
  for (iCont = 1; iCont <=60; iCont ++)
  {
    // Cálculo del valor de la resistencia termistor (NTC) actual (a través de Vout)
    Vout=Vin*((float)(analogRead(analogPin))/1024.0); 
    Rout=(Raux*Vout/(Vin-Vout));
 
    // Calculo de la temperatura en grados Kelvin
    TempK=(beta/log(Rout/Rinf));
    // Calculo de la temperatura en grados Celsius
    TempC=TempK-273.15; 
    // Almacenamos la temperatura (grados Celsuis) actual para despues obtener la media
    cTemp1 = cTemp1 + TempC; 
    // Hacemos una pausa de 10 milisegundos    
    //delay(10);
    
  }
 
 // Calculamos la temperatura media
  TempC = cTemp1/iCont;
 
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++// 
 //QUITAR CUANDO ESTEMOS FUERA DEL MODO DIAGNÓSTICO
 // Mostramos la temperatura media en grados Celsius
 //lcd.setCursor(1,0);
 //lcd.print("temp=");
 //lcd.print(TempC);
 //delay (1000);
 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
 // Hacemos una pausda de 1 seg.
 
 //SELECTOR DE PROGRAMA
 // lee el estado del pulsador y lo asigna a una variable local
int reading = digitalRead(buttonPin);

// revisa si el pulsador fue presionado
// (ejemplo, la entrada fue de LOW-HIGH), y tu estas esperando
// suficiente tiempo desde la última pulsación para ignorar el ruido:
// si el pulsador cambia, debido al ruido o a la pulsación:
     if (reading != lastButtonState) 
     {
// resetea el tiempo de rebote
          lastDebounceTime = millis();
          }

       if ((millis() - lastDebounceTime) > debounceDelay) {
// cualquiera que sea la lectura que ha estado ahí por mas tiempo
// que el retardo de entrada se toma como el estado actual real:

// si el estado del pulsador cambia:
             if (reading != buttonState) {
                         buttonState = reading;
// solo se cambia el estado del LED si el estado del pulsador el HIGH

 

                       if (buttonState == HIGH) {
                              i=i+1;
                              if (i==7)
                              i=0;
                        }
                }
           }
      Serial.print ("contador=");
      Serial.println (i);
      Serial.print ("Tobj=");
      Serial.println (TempC);
      Serial.print ("Tmax=");
      Serial.println (prog[i]);
    
     switch (i)
      {
        case 0:
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        digitalWrite(led4, HIGH); 
        digitalWrite(led5, LOW);
        digitalWrite(led6, LOW); 
        digitalWrite(led7, HIGH);
        break;
        case 1:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH); 
        digitalWrite(led5, HIGH);
        digitalWrite(led6, HIGH); 
        digitalWrite(led7, LOW);
        break;
        case 2:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH); 
        digitalWrite(led5, LOW);
        digitalWrite(led6, HIGH); 
        digitalWrite(led7, HIGH);
        break; 
        case 3:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
        digitalWrite(led4, HIGH); 
        digitalWrite(led5, LOW);
        digitalWrite(led6, LOW); 
        digitalWrite(led7, HIGH); 
        break;
        case 4:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, LOW); 
        digitalWrite(led5, LOW);
        digitalWrite(led6, HIGH); 
        digitalWrite(led7, HIGH); 
        break;
        case 5:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW); 
        digitalWrite(led5, HIGH);
        digitalWrite(led6, HIGH); 
        digitalWrite(led7, HIGH); 
        break;
        case 6:
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH); 
        digitalWrite(led5, HIGH);
        digitalWrite(led6, LOW); 
        digitalWrite(led7, HIGH); 
        break;
        }
 
// guarda la lectura. En el siguiente ciclo,
// este se convertirá en el estado anterior del pulsador:
          lastButtonState = reading;
//SELECTOR DE PROGRAMA
//Controlador flip-flop Heatbed
TempCmin=prog[i]-5;
TempCmax=prog[i]+5;
if (i<6)
{
if (TempC<TempCmin||TempC<TempCmax)
  {
    digitalWrite (MFET, HIGH);
    }
}
    if (TempC>TempCmax||i==6)
      digitalWrite (MFET, LOW);
}
