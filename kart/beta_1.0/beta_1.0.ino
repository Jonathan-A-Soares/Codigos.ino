#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include<SoftwareSerial.h>    
#include <TinyGPS++.h> // Include the TinyGPS++ library


#define SS_PIN 10
#define RST_PIN 9
#define ARDUINO_GPS_RX 5 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 4 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object
//variaveis globais---------------------------------------------------------------------------------------------------------
float lat, lon, vel;                                                                                 //
unsigned long data, hora;                                                                      //
unsigned short sat;                                                                          //
int p = 0;                                                                                    //
int  tempBat,  tempbat2, KM, timet, tempAB, tempM, tempC, rlm, rmC;  //
float bat, bat2;
//---------------------------------------------------------------------------------------------------------------------


void setup()
{
    analogReference(DEFAULT);
    gpsPort.begin(GPS_BAUD);
    SerialMonitor.begin(9600);
    //LCD
    Serial.begin(9600);
    lcd.init();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("----Bloqueado----");
    lcd.setCursor(0, 1);
    lcd.print("Aproxime A Tag");
    delay(500);
    //nfc
    Serial.begin(9600); // Inicia a serial
    SPI.begin();        // Inicia  SPI bus
    mfrc522.PCD_Init(); // Inicia MFRC522
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(2, INPUT_PULLUP);
    
}

void loop()
{
    int t = 0;
    p = 0;
    //==================================ler e guadar informaçoes de modulos =======================================

    //========================================================================================================

    //verifica se cartao esta presente

    if (digitalRead(2) == HIGH) // VERIFICA SE CARTAO ESTA  presente
    {                           // Botão  Pressionado;
                                // faz leitura da tag para verificar uid
        Serial.println("---------------");
        Serial.println("Cartao Presente");
        Serial.println("---------------");
        t = 0;

        //

        //=======================================================autenticação=======================================================
        // Procura por cartao RFID
        if (!mfrc522.PICC_IsNewCardPresent())
        {
            return;
        }
        // Seleciona o cartao RFID
        if (!mfrc522.PICC_ReadCardSerial())
        {
            return;
        }
        //Mostra UID na serial
        Serial.println();
        Serial.println("------------------------");
        Serial.println("tag Reconhecida :"); //motra no monitor seria que tag foi reconhecida
        String conteudo = "";
        byte letra;
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
            //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            // Serial.print(mfrc522.uid.uidByte[i], HEX);
            conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
        }
        Serial.println();

        conteudo.toUpperCase();

        if (conteudo.substring(1) == "F7 F7 30 63" || conteudo.substring(1) == "07 6E A9 5F") // autenticaçao atraves do uid da tag
        {
            // -=======================================================-tudo aki =======================================================
            lcd.clear();
            
            
            Serial.println("Altenticado com sucesso"); // manda para monitor serial que foi autenticado con sucesso
            Serial.println("------------------------");
            Serial.println();
            
            digitalWrite(8, HIGH); //liga led verde
            digitalWrite(7, LOW);

            for (int o = 0; o < 5;) //conteudo do display aki
            {
              
                if (digitalRead(2) == HIGH)
                {
                    o = 1;
                }
                else
                {
                    o = 5;
                }
                // ================bem vindo por pouco tempo======================================================

                while (p < 2)
                {
                    p++;
                    lcd.clear(); // coteudo depois do desbloqueio
                    lcd.setCursor(4, 0);
                    lcd.print("bem vindo");
                    
                    
                    if (conteudo.substring(1) == "F7 F7 30 63" ){
                      lcd.setCursor(0, 1);
                     lcd.print("Jonathan Soares");
                    }else{
                      lcd.setCursor(3, 1);
                     lcd.print("Visitante");
                    }
                    delay(1000);
                    lcd.clear(); 
                    
                }
                //==========================================codigo a seguir depois do bem vindo================================================================
                //horas
                
                // latitude longtude
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Lat: "); 
                delay(30);
                lcd.print(tinyGPS.location.lat(), 6);
                lcd.setCursor(0, 1);
                lcd.print("Long: "); 
                delay(30);
                lcd.print(tinyGPS.location.lng(), 6);
                delay(3000);
                lcd.clear();


                
                //altitude
                lcd.setCursor(0, 0);
                lcd.print("Alt: "); 
                lcd.print(tinyGPS.altitude.meters());
                lcd.print("M");
                delay(3000);
                lcd.clear();


                
                //velocida km/h
                lcd.setCursor(0, 0);
                lcd.print("Speed: ");
                lcd.print(tinyGPS.speed.kmph());
                lcd.print("Km/h ");
                delay(3000);
                lcd.clear();


                
                //satelites
                lcd.setCursor(0, 0);
                lcd.print("Satelites: "); 
                lcd.println(tinyGPS.satellites.value());
                delay(3000);
                lcd.clear();

                
                //voltagem baterias
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Bateria1: "); 
                lcd.print(bat,3);
                delay(30);
                lcd.setCursor(0, 1);
                lcd.print("Bateria2: "); 
                lcd.print(bat2,3);
                delay(3000);
                lcd.clear();
                //tempo de atividade
                
                //=====================verifica se carta oesta prenset=======================
                lcd.clear();
                if (digitalRead(2) == HIGH)
                {
                    o = 1;
                }
                else
                {
                    o = 5;
                }
                //===============================chama fuunçao do gps===========================================
                // print position, altitude, speed, time/date, and satellites:
                printGPSInfo();
              
                // "Smart delay" looks for GPS data while the Arduino's not doing anything else
                smartDelay(1000); 
                  
                                  
                //===================================================================================
            }
           
            delay(1000);
        }
        else // =======================================================fim autenticaçao=======================================================
        {    // se nao for autenticado apresata erro no dysplay
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Acesso Negado!");
            lcd.setCursor(0, 1);
            lcd.print("tag Nao Reconhecida");
            Serial.println();
            Serial.println("Acesso Negado");
            Serial.println("------------------------");
            digitalWrite(7, HIGH); //liga led vermelho
            digitalWrite(3, HIGH); // deliga rele
        }

        //fim do if
    }
    else if (digitalRead(2) == LOW) // VERIFICA SE CARTAO NAO ESTA  presente
    {                               // Botão nao Pressionado
        Serial.println("------------------------");
        Serial.println("Cartao Nao Esta Presente"); // mostra no monitor serial que caratao/tag esta ausente
        Serial.println("------------------------");
        t = 1;
        digitalWrite(7, HIGH); //liga led vermelho
        digitalWrite(8, LOW);
        digitalWrite(3, HIGH); // deliga rele

        if (digitalRead(2) == LOW)
                {
                  lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Acesso Negado!"); //mostra que acesso esta negado
        lcd.setCursor(0, 1);
        lcd.print("tag Nao Reconhecida");
                }
                
        
       
        delay(1000);
       
    }

    delay(1000);
// print position, altitude, speed, time/date, and satellites:
  printGPSInfo();

  // "Smart delay" looks for GPS data while the Arduino's not doing anything else
  smartDelay(1000); 
}
void printGPSInfo()
{
  // Print latitude, longitude, altitude in feet, course, speed, date, time,
  // and the number of visible satellites.
  SerialMonitor.print("Lat: "); SerialMonitor.println(tinyGPS.location.lat(), 6);
  SerialMonitor.print("Long: "); SerialMonitor.println(tinyGPS.location.lng(), 6);
  SerialMonitor.print("Alt: "); SerialMonitor.println(tinyGPS.altitude.meters());
  SerialMonitor.print("Speed: "); SerialMonitor.println(tinyGPS.speed.kmph());
  SerialMonitor.print("Date: "); printDate();
  SerialMonitor.print("Time: "); printTime();
  SerialMonitor.print("Sats: "); SerialMonitor.println(tinyGPS.satellites.value());
  SerialMonitor.println();
  //===========================calculo voltagem===============

  
  float tensao1 = analogRead(A0)*(5.0/1023.0)*10;

   bat = tensao1;
   //(c*b) / a
   //a --- b 
  //c --- x
  
  Serial.print("voltagem bateria1 :");Serial.println(bat,5);
  
  float tensao2 = analogRead(A1)*(5.0/1023.0)*10;
  bat2 = tensao2;

 
  Serial.print("voltagem bateria2 :");Serial.println(bat2,5);
  Serial.println("------------------------");

  
  //==========================================================
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
  } while (millis() - start < ms);
}

// printDate() formats the date into dd/mm/yy.
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
