#include <Wire.h>
#include<SoftwareSerial.h>
#include <TinyGPS++.h> // Include the TinyGPS++ library

#define ARDUINO_GPS_RX 5 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 4 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600

SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object


//========================================Variaveis Do Gps========================================
float lat, lon, vel;                                                                                
unsigned long data, hora;                                                                     
unsigned short sat;                                                                          


void setup()
{
  //GPS
  analogReference(DEFAULT);
  gpsPort.begin(GPS_BAUD);
  SerialMonitor.begin(9600);
}


void loop()
{
  
  printInfo();
  smartDelay(1000);
  
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
