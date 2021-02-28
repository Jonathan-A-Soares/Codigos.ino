//Autor:https://github.com/Jonathan-A-Soares
#include <Wire.h>
#include <Sim800l.h>
#include <SoftwareSerial.h>
#include<TinyGPS++.h>
#include<SPI.h>

#define ARDUINO_GPS_RX 2 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 3 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define GPS_BAUD 9600
/*
      PINOUT:

          GND          >>>   GND
          RX  9       >>>   TX
          TX  10        >>>   RX
         RESET 2       >>>   RST
*/

SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object
Sim800l Sim800l;
String textSms, numberSms;
char* number , text  ;
bool error;
void setup()
{
  //GPS
  analogReference(DEFAULT);
  gpsPort.begin(GPS_BAUD);
  Serial.begin(9600);




  //sms
  Serial.begin(9600);
  Sim800l.begin();
  Serial.print("Limpando SMS antigos...");
  error = Sim800l.delAllSms(); //Apaga SMS
  Serial.println(" Apagados!");
  Serial.println("\nAguardando comandos por SMS...");
  number = "35000000000";
}
void printInfo()
{
  //====================================== Localização ======================================
  Serial.print("Lat: "); Serial.println(tinyGPS.location.lat(), 6);
  Serial.print("Long: "); Serial.println(tinyGPS.location.lng(), 6);
  Serial.print("Alt: "); Serial.println(tinyGPS.altitude.meters());
  Serial.print("Speed: "); Serial.println(tinyGPS.speed.kmph());
  Serial.print("Sats: "); Serial.println(tinyGPS.satellites.value());

  //====================================== Data ======================================
  Serial.print(tinyGPS.date.day());
  Serial.print("/");
  Serial.print(tinyGPS.date.month());
  Serial.print("/");
  Serial.println(tinyGPS.date.year());

  //====================================== Hora ======================================
  Serial.print(tinyGPS.time.hour());
  Serial.print(":");
  if (tinyGPS.time.minute() < 10) Serial.print('0');
  Serial.print(tinyGPS.time.minute());
  Serial.print(":");
  if (tinyGPS.time.second() < 10) Serial.print('0');
  Serial.println(tinyGPS.time.second());

  //==================================================================================
}
void loop()
{

  textSms = Sim800l.readSms(1);

  if (textSms.indexOf("OK") != -1)
  {
    if (textSms.length() > 7)
    {
      numberSms = Sim800l.getNumberSms(1);
      numberSms.remove(0, 1);
      Serial.println(numberSms);
      textSms.toUpperCase();



      numberSms.toCharArray(number, 20);




      if (textSms.indexOf("KDTU") != -1)
      {
        Serial.println("Opçao 1");
        error = Sim800l.sendSms(number, "Aki");
      }

      else if (textSms.indexOf("CORTE") != -1)
      {
        Serial.println("Opçao 2");
        error = Sim800l.sendSms(number, "cortou");
      }
      else if (textSms.indexOf("OFFALARME") != -1)
      {
        Serial.println("Opçao 3");
        error = Sim800l.sendSms(number, "Alarme Desligado");
      }
      else if (textSms.indexOf("ONALARME") != -1)
      {
        Serial.println("Opçao 4");
        error = Sim800l.sendSms(number, "Alarme Ligado");
      }
      else if (textSms.indexOf("DISPARAR") != -1)
      {


        Serial.println("Opçao 5");
        error = Sim800l.sendSms(number, "cortou");
        error = Sim800l.sendSms(number, "Aki");
        error = Sim800l.sendSms(number, text);

      }


      Sim800l.delAllSms();
    }
  }
}
