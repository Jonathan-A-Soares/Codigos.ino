//https://github.com/Jonathan-A-Soares

#include <SoftwareSerial.h>
#include <TinyGPS++.h> // Include the TinyGPS++ library
#include <Sim800l.h>
#include <SPI.h>
//#include <SD.h>


//const int chipSelect = 4;

#define ARDUINO_GPS_RX 2 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 3 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600

TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial

//========================================variaveis sim800l========================================================

Sim800l Sim800l;
String textSms, numberSms ;
char* number ;
bool error;

//========================================Variaveis Do Gps========================================
float lat, lon, vel;
unsigned long data, hora;
unsigned short sat;
//========================================================================================


//SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  //GPS
  analogReference(DEFAULT);
  gpsPort.begin(GPS_BAUD);
  SerialMonitor.begin(9600);
  //tinyGPS.begin(9600);

//  Serial.begin(9600);
//  while (!Serial); 
//  Serial.println("Initializing SD card...");
//
//
//
//
//  if (!SD.begin(4)) {
//  Serial.println("initialization failed!");
//  while (1);

//Serial.println("initialization done.");

      //sim800l
      Sim800l.begin();
      Serial.print("Limpando SMS antigos...");
      error = Sim800l.delAllSms(); //Apaga SMS
      Serial.println("Apagados!");
      Serial.println("Aguardando comandos por SMS...");
      number = "35000000000";
}
void loop()  
{
    char* lat1, lot1, vel1;
    dtostrf(lat, 8, 8, lat1);
    dtostrf(lon, 8, 8, lot1);
    dtostrf(vel, 8, 8, vel1);
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
        error = Sim800l.sendSms(number,lat1);
      }

      else if (textSms.indexOf("CORTE") != -1)
      {
        Serial.println("Opçao 2");
        error = Sim800l.sendSms(number, "Cortou");
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
        error = Sim800l.sendSms(number, "Cortou");
        error = Sim800l.sendSms(number, "Aki");
        error = Sim800l.sendSms(number, "Alarme Disparado");

      }


      Sim800l.delAllSms();
    }
  }
  
  
  printInfo();
  smartDelay(1000);
//  grava();
}

//void grava()
//{
//  File dataFile = SD.open("GpsDat.txt", FILE_WRITE);
//
//  // if the file is available, write to it:
//  if (dataFile) {
//    dataFile.println("");
//    dataFile.println("Latitude: ");
//    dataFile.println(tinyGPS.location.lat(), 6);
//    dataFile.print("Longitude: ");
//    dataFile.println(tinyGPS.location.lng(), 6);
//    dataFile.print("Hora: ");
//    dataFile.print(tinyGPS.time.hour() - 3);
//    dataFile.print(":"); dataFile.println(tinyGPS.time.minute());
//    dataFile.print("Data: ");
//    dataFile.println(tinyGPS.date.value()); // Raw date in DDMMYY
//    dataFile.print("Velocidade: ");
//    dataFile.print(tinyGPS.speed.mps());
//    dataFile.println(" Km/h");
//    dataFile.println("");
//    dataFile.close();
//
//  }
//  else
//  {
//    Serial.println("Failed to open file");
//    Serial.println();
//  }
//}
//
//


void printInfo()
{
  //====================================== Localização ======================================
  SerialMonitor.print("Lat: "); SerialMonitor.println(tinyGPS.location.lat(), 6);
  SerialMonitor.print("Long: "); SerialMonitor.println(tinyGPS.location.lng(), 6);
  SerialMonitor.print("Alt: "); SerialMonitor.println(tinyGPS.altitude.meters());
  SerialMonitor.print("Speed: "); SerialMonitor.println(tinyGPS.speed.kmph());
  SerialMonitor.print("Sats: "); SerialMonitor.println(tinyGPS.satellites.value());

  //====================================== Data ======================================
  SerialMonitor.print(tinyGPS.date.day());
  SerialMonitor.print("/");
  SerialMonitor.print(tinyGPS.date.month());
  SerialMonitor.print("/");
  SerialMonitor.println(tinyGPS.date.year());

  //====================================== Hora ======================================
  SerialMonitor.print(tinyGPS.time.hour());
  SerialMonitor.print(":");
  if (tinyGPS.time.minute() < 10) SerialMonitor.print('0');
  SerialMonitor.print(tinyGPS.time.minute());
  SerialMonitor.print(":");
  if (tinyGPS.time.second() < 10) SerialMonitor.print('0');
  SerialMonitor.println(tinyGPS.time.second());

  //==================================================================================
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {

    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read());
  }   while (millis() - start < ms);
}
