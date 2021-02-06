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



//variaveis globais---------------------------------------------------------------------------------------------------------

float lat, lon, vel;                                                                                 //
unsigned long data, hora;                                                                      //
unsigned short sat;                                                                          //



//========================================
void setup()
{
    //GPS
    analogReference(DEFAULT);
    gpsPort.begin(GPS_BAUD);
    SerialMonitor.begin(9600);
}

void loop()
{


          printKARTInfo();// impreme todas imformaçoes monitor serial e atualiza hora dat etc.
          smartDelay(1000); // "Atraso inteligente" procura os dados do GPS enquanto o Arduino não faz mais nada
          
          
 
}//==========================================================


void printKARTInfo()
{
  //===========================coleta informaçoes do mudlo gps ==========================================
            // Print latitude, longitude, altitude in feet, course, speed, date, time,
            // and the number of visible satellites.
            SerialMonitor.print("Lat: "); SerialMonitor.println(tinyGPS.location.lat(), 6);
            SerialMonitor.print("Long: "); SerialMonitor.println(tinyGPS.location.lng(), 6);
            SerialMonitor.print("Alt: "); SerialMonitor.println(tinyGPS.altitude.meters());
            SerialMonitor.print("Speed: "); SerialMonitor.println(tinyGPS.speed.kmph());
            SerialMonitor.print("Date: "); printDate();
            SerialMonitor.print("Time: "); printTime();
            SerialMonitor.print("Sats: "); SerialMonitor.println(tinyGPS.satellites.value());

  
  //===============================================================================================================================
}

        // This custom version of delay() ensures that the tinyGPS object
        // is being "fed". From the TinyGPS++ examples.
        static void smartDelay(unsigned long ms)
{
        unsigned long start = millis();
        do
  {
        // If data has come in from the GPS module
        while (gpsPort.available())
          tinyGPS.encode(gpsPort.read()); // Send it to the encode function
        // tinyGPS.encode(char) continues to "load" the tinGPS object with new
        // data coming in from the GPS module. As full NMEA strings begin to come in
        // the tinyGPS library will be able to start parsing them for pertinent info
  }   while (millis() - start < ms);
}

// printDate() formats the date into dd/mm/yy.
//===============================================================================================
void printDate()
{
      SerialMonitor.print(tinyGPS.date.day());
      SerialMonitor.print("/");
      SerialMonitor.print(tinyGPS.date.month());
      SerialMonitor.print("/");
      SerialMonitor.println(tinyGPS.date.year());
}

// printTime() formats the time into "hh:mm:ss", and prints leading 0's
// where they're called for.

//=================================================================================================
void printTime()
{
        SerialMonitor.print(tinyGPS.time.hour());
        SerialMonitor.print(":");
        if (tinyGPS.time.minute() < 10) SerialMonitor.print('0');
        SerialMonitor.print(tinyGPS.time.minute());
        SerialMonitor.print(":");
        if (tinyGPS.time.second() < 10) SerialMonitor.print('0');
        SerialMonitor.println(tinyGPS.time.second());
}
//================================================================================================
