//ver1

#include <TimerOne.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
DS1307 rtc;
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27,16,2); 
String server = "api.pushingbox.com";
const String devid = "v74D7DE3A88EABDD";
const int led = LED_BUILTIN;
int ledState = LOW;
int Intervalo =900;// 3600;///3600
int Intervalo2 = 600;//600
int flagwifi=0;
String mem="ok   ";
String cadena="";
const int chipSelect = 53;//SD
File logfile;
///datoa a enviar
//int celData =0;int fehrData=0; int hicData=0; int  hifData=0;int humidityData =0;
//////////////////

int flag5 =0;
void setup() {
//  lcd.init();
//  lcd.backlight();
//  lcd.clear();
  delay(1000);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  Timer1.initialize(1000000);
  Serial2.begin(9600);
  Serial2.setTimeout(2000);
  Timer1.attachInterrupt(RegEnergy);
  interrupts();
    Wire.begin();  
// if (rtc.begin()) {
    //logfile.println("RTC failed");
   
    //////////////////////////////////
//sd init

 // initialize the SD card
  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(53, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println(F("card initialized."));
  
  // create a new file
  char filename[] = "datalog.txt";
//  for (uint8_t i = 0; i < 100; i++) {
//    filename[6] = i/10 + '0';
//    filename[7] = i%10 + '0';
//    if (! SD.exists(filename)) {
//      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      
//      break;  // leave the loop!
//    }
//  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  
  Serial.print(F("Logging to: "));
  Serial.println(filename);
  logfile.close();

////////////
if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
}
    
     rtc.adjust(DateTime(__DATE__, __TIME__));
    Serial.println("RTC ok");
    
 // }  
  //analogReference(INTERNAL);
  //analogReference(INTERNAL1V1); //solo Arduino Mega

/////////////////
Serial.print("Starting at: ");
gravaRTC();
Serial.println();

/////////////////77
 Serial2.println(F("AT"));
  if(Serial2.find("OK"))
    Serial2.println(F("Respuesta AT correcto"));
  else
    Serial2.println(F("Error en ESP8266"));


//-----Configuración de red-------//Podemos comentar si el ESP ya está configurado

    //ESP8266 en modo estación (nos conectaremos a una red existente)
    Serial2.println(F("AT+CWMODE=1"));
    //if(SerialESP8266.find("OK"))
    //  Serial.println(F("ESP8266 en modo Estacion"));
      
    //Nos conectamos a una red wifi 
    Serial2.println(F("AT+CWJAP=\"INFINITUM89DD_2.4\",\"rabr535455\""));
    //Serial.println(F("Conectandose a la red ..."));
    Serial2.setTimeout(10000); //Aumentar si demora la conexion
    if(Serial2.find("OK"))
      Serial.println(F("WIFI conectado"));
    else
      Serial.println(F("Error al conectarse en la red"));
    Serial2.setTimeout(2000);
    //Desabilitamos las conexiones multiples
    Serial2.println(F("AT+CIPMUX=0"));
    if(Serial2.find("OK"))
      Serial.println(F("Multiconexiones deshabilitadas"));
    delay(1000);
  //------fin de configuracion--------------


    

  
}

volatile unsigned long blinkCount = 0;
float corr1 = 0, corr2 = 0, corr3 = 0 ;
float Sum1 = 0, Sum2 = 0, Sum3 = 0, Sum4 = 0, Sum5 = 0, Sum6 = 0;
float ICrms1 = 0, ICrms2 = 0, ICrms3 = 0;
float Volt1 = 0, Volt2 = 0, Volt3 = 0;
float Vrms1 = 0, Vrms2 = 0, Vrms3 = 0;
float Vat1 = 0, Vat2 = 0, Vat3 = 0;
 long FP1=0, FP2=0, FP3=0;
 long Tiempo1,retrazo1;
 long Tiempo2, retrazo2;
 long Tiempo3, retrazo3;

void loop() {

   
  get_sensores(); //Corriente eficaz (A)

 if (flagwifi==1){ flagwifi=0;
 enviowifi();
 }
  //  Serial.print(ICrms1);
  //  Serial.print(",");
  //  Serial.print(ICrms2);
  //  Serial.print(",");
  //  Serial.print(ICrms3);
  //  Serial.print(",");
  //  Serial.print(Vrms1);
  //  Serial.print(",");
  //  Serial.print(Vrms2);
  //  Serial.print(",");
  //  Serial.println(Vrms3);
  //
  //  Serial.print(Vat1);
  //  Serial.print(",");
  //  Serial.print(Vat2);
  //  Serial.print(",");
  //  Serial.println(Vat3);

  //Serial.println(FP1);
  // delay(1000);
}

void get_sensores() {
  Sum1 = 0, Sum2 = 0, Sum3 = 0, Sum4 = 0, Sum5 = 0, Sum6 = 0;
  //  float corr1=0, corr2=0, corr3=0 ;
  //  float Sum1=0,Sum2=0,Sum3=0,Sum4=0,Sum5=0,Sum6=0; //(1,2 y 3 coriientes y 4,5 y 6 voltajes)
  //  float ICrms1=0,ICrms2=0,ICrms3 =0;
  //  float Volt1=0, Volt2=0, Volt3=0;
  //  float Vrms1=0, Vrms2=0, Vrms3=0;
  long tiempo = millis();
  
  int N = 0;
  int flag1=0;
  int flag2=0;
  int flag3=0;
  
 
  
  while (millis() - tiempo < 500) //Duración 0.5 segundos(Aprox. 30 ciclos de 60Hz)
  {

    //////////////////////////////
    Volt1 = (analogRead(A1) - 510);
    if (flag1 == 0) {
      if (Volt1 < 515) {
        Tiempo1 = millis();
        flag1 = 1;
      }
    }

    corr1 = analogRead(A0) ;
    if (flag1 == 1){
        if (corr1>15){
          retrazo1=(millis()-Tiempo1)*0.3966;
        FP1=cos(retrazo1);
        //
        flag1=0;
        
        }
      
    }
////////////////

     //////////////////////////////
    Volt2 = (analogRead(A3) - 510);
    if (flag2 == 0) {
      if (Volt2 < 515) {
        Tiempo2 = millis();
        flag2 = 1;
      }
    }

    corr2 = analogRead(A2) ;
    if (flag2 == 1){
        if (corr2>15){
          retrazo2=(millis()-Tiempo2)*0.3966;
        FP2=cos(retrazo2);
        //
        flag2=0;
        
        }
      
    }
////////////////

  //////////////////////////////
    Volt3 = (analogRead(A5) - 510);
    if (flag3 == 0) {
      if (Volt3 < 515) {
        Tiempo3 = millis();
        flag3 = 1;
      }
    }

    corr3 = analogRead(A4) ;
    if (flag3 == 1){
        if (corr3>15){
          retrazo3=(millis()-Tiempo3)*0.3966;
        FP3=cos(retrazo3);
        //
        flag3=0;
        
        }
      
    }
////////////////

   
    corr1 = corr1 * 30 * (1.1 / 1023.0); ////voltaje del sensor corriente=VoltajeSensor*(30A/1V)
    Sum1 = Sum1 + sq(corr1); //Sumatoria de Cuadrados
    corr2 = corr2 * 30 * (1.1 / 1023.0); ////voltaje del sensor corriente=VoltajeSensor*(30A/1V)
    Sum2 = Sum2 + sq(corr2);
    corr3 = corr3 * 30 * (1.1 / 1023.0); ////voltaje del sensor corriente=VoltajeSensor*(30A/1V)
    Sum3 = Sum3 + sq(corr3);
     Volt1 = (Volt1) * 2.47; ////voltaje del sensor
    Sum4 = Sum4 + sq(Volt1);
    Volt2 = Volt2 * 2.47; ////voltaje del sensor
    Sum5 = Sum5 + sq(Volt2);
    Volt3 = Volt3 * 2.47; ////voltaje del sensor3 
    Sum6 = Sum6 + sq(Volt3);





    

    N = N + 1;
    delay(1);
  }
  Sum1 = Sum1 * 2; //Para compensar los cuadrados de los semiciclos negativos.
  ICrms1 = sqrt((Sum1) / N); //ecuación del RMS
  Sum2 = Sum2 * 2; //Para compensar los cuadrados de los semiciclos negativos.
  ICrms2 = sqrt((Sum2) / N);
  Sum3 = Sum3 * 2; //Para compensar los cuadrados de los semiciclos negativos.
  ICrms3 = sqrt((Sum3) / N);
  Vrms1 = sqrt((Sum4) / N);
  // if (Vrms1 < 1.5) { Vrms1=0; }
  Vrms2 = sqrt((Sum5) / N);
  // if (Vrms2 < 1.5) { Vrms2=0; }
  Vrms3 = sqrt((Sum6) / N);
  // if (Vrms3 < 1.5) { Vrms3=0; }
  Vat1 = Vrms1 * ICrms1;
  Vat2 = Vrms2 * ICrms2;
  Vat3 = Vrms3 * ICrms3;
}


//volatile unsigned long blinkCount = 0; // use volatile for shared variables

int count = Intervalo-60;
int count2=Intervalo2-10;
float Ener1 = 0, Ener2 = 0, Ener3 = 0, SumEnergy = 0;
float BufEner1 = 0, BufEner2 = 0, BufEner3 = 0;
float Ener11 = 0, Ener22 = 0, Ener33 = 0, SumEnergy2 = 0;


void RegEnergy (void) {
  //lcd.print(FP1); 
     
//////////////////////////////
  if (count < Intervalo)
  {
    Ener1 = Ener1 + Vat1 ;/// 3600;
    Ener2 = Ener2 + Vat2 ;/// 3600;
    Ener3 = Ener3 + Vat3 ;/// 3600;
//    Ener1 = Ener1 + Vat1 / 3600;
//    Ener2 = Ener2 + Vat2 / 3600;
//    Ener3 = Ener3 + Vat3 / 3600;
    
    count = count + 1;
  }
  else {
    count = 0;
   Ener1 = Ener1/3600;
   Ener2=  Ener2/3600;
   Ener3= Ener3/3600;
    BufEner1=Ener1;
     BufEner2=Ener2;
      BufEner3=Ener3;
    SumEnergy = Ener1 + Ener2 + Ener3;
    envia();
    //enviowifi();
    flagwifi=1;
//    Serial.print("Ener1=");
//    Serial.print(Ener1);
//    Serial.print(",");
//     Serial.print("Ener2=");
//    Serial.print(Ener2);
//    Serial.print(",");
//     Serial.print("Ener3=");
//    Serial.println(Ener3);
    Ener1 = 0;
    Ener2 = 0;
    Ener3 = 0;
  }

///////////////////////////

//////////////////////////////
  if (count2 <= Intervalo2)
  {
//    Ener11 = Ener11 + Vat1 / 3600;
//    Ener22 = Ener22 + Vat2 / 3600;
//    Ener33 = Ener33 + Vat3 / 3600;

Ener11 =  Vat1   ;
Ener22 =  Vat2 ;
Ener22 =  Vat3 ;


    count2 = count2 + 1;
  }
  else {
    count2 = 0;
   
    //SumEnergy2 = Ener11 + Ener22 + Ener33;
    envia2();
    //salvaSD();
   
    
    Ener11 = 0;
    Ener22 = 0;
    Ener33 = 0;
  }

///////////////////////////
  
  if (ledState == LOW) {
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(led, ledState);
  
}


void envia(void)
{
  gravaRTC();
  Serial.print (Ener11);
  Serial.print (",");
  Serial.print(FP1);
  Serial.print (",");
  Serial.print (Ener22);
  Serial.print (",");
  Serial.println(Ener33);

 

}
void envia2 (void)
{
  
   Serial1.print (Ener11); //////
  Serial1.print (",");
  Serial1.print (Ener22);
  Serial1.print (",");
  Serial1.println(Ener33);
}


void gravaRTC(void){

noInterrupts();
  //Serial.println("aqui borra");
  interrupts();
   DateTime now = rtc.now();
    
   Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ");

///////////////
File logfile = SD.open("datalog.txt", FILE_WRITE);
   if ( logfile) {
/////////////////////
logfile.print(now.year(), DEC);
  logfile.print(F("/"));
  logfile.print(now.month(), DEC);
  logfile.print(F("/"));
  logfile.print(now.day(), DEC);
  logfile.print(F(" "));
  logfile.print(now.hour(), DEC);
  logfile.print(F(":"));
  logfile.print(now.minute(), DEC);
  logfile.print(F(":"));
  logfile.print(now.second(), DEC);
   logfile.print(" "); 
    logfile.print(Ener11);
    logfile.print(",");
    logfile.print(FP1);
    logfile.print(",");
  logfile.print(Ener22);   
  logfile.print(","); 
  logfile.print(Ener33);    
   logfile.println();
logfile.close();
    mem="ok";
   }
   else{
    Serial.print("couldnt create file");
    mem="fallaSD";
        }


////////////////







    


     
      
}

void salvaSD(void){
//  noInterrupts();
//  //Serial.println("aqui grava");
//  interrupts();
// // delay(10);
//
//  DateTime now = rtc.now();
//  File logfile = SD.open("datalog.txt", FILE_WRITE);
//   if ( logfile) {
///////////////////////
//logfile.print(now.year(), DEC);
//  logfile.print(F("/"));
//  logfile.print(now.month(), DEC);
//  logfile.print(F("/"));
//  logfile.print(now.day(), DEC);
//  logfile.print(F(" "));
//  logfile.print(now.hour(), DEC);
//  logfile.print(F(":"));
//  logfile.print(now.minute(), DEC);
//  logfile.print(F(":"));
//  logfile.print(now.second(), DEC);
//   logfile.print(F(", ")); 
//    logfile.print(Ener11);
//    logfile.print(",");
//  logfile.print(Ener22);   
//  logfile.print(","); 
//  logfile.print(Ener33);    
//   logfile.println();
//logfile.close();
//    mem="ok";
//   }
//   else{
//    Serial.print("couldnt create file");
//    mem="fallaSD";
//        }
  }





  
  void error(char *str)
{
  Serial.print(F("error: "));
  Serial.println(str);
  mem="Falla Sd";
  // red LED indicates error
//  digitalWrite(redLEDpin, HIGH);

 // while(1);
}

//void enviowifi(void){
  
  
  
  
  
  

  
