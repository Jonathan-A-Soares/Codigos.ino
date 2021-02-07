//https://github.com/Jonathan-A-Soares
#include <Wire.h>
#include<SoftwareSerial.h>
#include<TinyGPS++.h> // Include the TinyGPS++ library
#include<SD.h>
#include<SPI.h>

#define ARDUINO_GPS_RX 2 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 3 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600

SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object


//========================================Variaveis Do Gps========================================
float lat, lon, vel;
unsigned long data, hora;
unsigned short sat;
//================================================================================================

void setup()
{
  //GPS
  analogReference(DEFAULT);
  gpsPort.begin(GPS_BAUD);
  SerialMonitor.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}





void grava()
{
  File dataFile = SD.open("GpsDat.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("")
    dataFile.println("Latitude: "); dataFile.println(tinyGPS.location.lat(), 6);
    dataFile.print("Longitude: "); dataFile.println(tinyGPS.location.lng(), 6);
    dataFile.print("Hora: "); dataFile.print(tinyGPS.time.hour() - 3); dataFile.print(":"); dataFile.println(tinyGPS.time.minute());
    dataFile.print("Data: "); dataFile.println(tinyGPS.date.value()); // Raw date in DDMMYY
    dataFile.print("Velocidade: "); dataFile.print(gps.speed.mps()); dataFile.println(" Kmph")
    dataFile.println("")
    dataFile.close();

  }
  else
  {
    Serial.println("Failed to open file");
    Serial.println();
  }
}




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


void loop()
{

  printInfo();
  smartDelay(1000);
  grava();
}
