/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(10, 11); // RX, TX
LiquidCrystal_I2C lcd(0x27,16,2);

char c = ' ';
int length = 30;
char buffer [31];
String myString;
char termChar = 10;
 byte index = 0;
 float Ener1=0, Ener2=0, Ener3=0;
 


void setup() {
  // Open serial communications and wait for port to open:
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
   // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  lcd.print("LOG COVID2020");
  delay(500);
}

void loop() { // run over and over
  if (mySerial.available()) {
   // Serial.write(mySerial.read());
   c = mySerial.read();
     if (c != termChar)
     {
       buffer[index] = c;
       index = index + 1;
     }
 
     else
     {
       buffer[index] = '\0';
       index = 0;
       processNewData();
     }
  }
  



    
  
//  lcd.setCursor(0,0);
//    lcd.print("hola");
    delay(100);
}

void processNewData()
{
  myString= String(buffer);
   int commaIndex = myString.indexOf(',');
int secondCommaIndex = myString.indexOf(',', commaIndex+1);
String firstValue = myString.substring(0, commaIndex);
String secondValue = myString.substring(commaIndex+1, secondCommaIndex);
String thirdValue = myString.substring(secondCommaIndex+1); 
Ener1=firstValue.toFloat();
Ener2=secondValue.toFloat();
Ener3=thirdValue.toFloat();
  lcd.setCursor(0,0);
  lcd.print("W:");
    lcd.print(Ener1);
    lcd.print(" ");
     //lcd.print("Wh2:");
    lcd.print(Ener2);
    lcd.setCursor(0,1);
     //lcd.print("");
    lcd.print(Ener3);
    
   Serial.println(buffer); 
   
}





