#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h> // Include the TinyGPS++ library

#define SS_PIN 10
#define RST_PIN 9
#define ARDUINO_GPS_RX 5 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 4 // GPS RX, Arduino TX pin
#define gpsPort ssGPS    // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);                     // Create MFRC522 instance.
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS;                                  // Create a TinyGPSPlus object
//variaveis globais---------------------------------------------------------------------------------------------------------
float lat, lon, vel;                                                         //
unsigned long data, hora;                                                    //
unsigned short sat;                                                          //
int p = 0;                                                                   //
int tempBat, bat, bat2, tempbat2, KM, timet, tempAB, tempM, tempC, rlm, rmC; //
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
    //==================================ler e guadar informaçoes de modulos =======================================

    //========================================================================================================

    //verifica se cartao esta presente
    digitalWrite(8, LOW);

    digitalWrite(7, LOW);
    if (digitalRead(2) == HIGH) // VERIFICA SE CARTAO ESTA  presente
    {                           // Botão  Pressionado;
                                // faz leitura da tag para verificar uid
        Serial.println("Cartao Presente");
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
        Serial.print("tag Reconhecida :"); //motra no monitor seria que tag foi reconhecida
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

        if (conteudo.substring(1) == "F7 F7 30 63" && conteudo.substring(1) == "F7 F7 30 63") // autenticaçao atraves do uid da tag
        {
            // -=======================================================-tudo aki =======================================================
            lcd.clear();
            Serial.println();
            Serial.print("Altenticado com sucesso"); // manda para monitor serial que foi autenticado con sucesso
            Serial.println();
            digitalWrite(3, LOW);  // liga rele
            digitalWrite(8, HIGH); //liga led verde

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
                    delay(1000);
                    lcd.clear(); // coteudo depois do desbloqueio
                }
                //==========================================codigo a seguir depois do bem vindo================================================================

                // latitude longtude
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Lat: ");
                lcd.print(tinyGPS.location.lat(), 6);
                lcd.setCursor(0, 1);
                lcd.print("Long: ");
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
                lcd.print(bat);
                lcd.setCursor(0, 1);
                lcd.print("Bateria2: ");
                lcd.print(bat2);
                delay(3000);
                lcd.clear();

                lcd.clear();
                // print position, altitude, speed, time/date, and satellites:
                printGPSInfo();

                // "Smart delay" looks for GPS data while the Arduino's not doing anything else
                smartDelay(1000);

                //====================================================================================================================================
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
            Serial.print("Acesso Negado");
            digitalWrite(7, HIGH); //liga led vermelho
            digitalWrite(3, HIGH); // deliga rele
        }

        //fim do if
    }
    else if (digitalRead(2) == LOW) // VERIFICA SE CARTAO NAO ESTA  presente
    {                               // Botão nao Pressionado

        Serial.println("Cartao Nao Esta Presente"); // mostra no monitor serial que caratao/tag esta ausente
        t = 1;
        digitalWrite(7, HIGH); //liga led vermelho
        digitalWrite(3, HIGH); // deliga rele
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Acesso Negado!"); //mostra que acesso esta negado
        lcd.setCursor(0, 1);
        lcd.print("tag Nao Reconhecida");
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
    SerialMonitor.print("Lat: ");
    SerialMonitor.println(tinyGPS.location.lat(), 6);
    SerialMonitor.print("Long: ");
    SerialMonitor.println(tinyGPS.location.lng(), 6);
    SerialMonitor.print("Alt: ");
    SerialMonitor.println(tinyGPS.altitude.meters());
    SerialMonitor.print("Speed: ");
    SerialMonitor.println(tinyGPS.speed.kmph());
    SerialMonitor.print("Date: ");
    printDate();
    SerialMonitor.print("Time: ");
    printTime();
    SerialMonitor.print("Sats: ");
    SerialMonitor.println(tinyGPS.satellites.value());
    SerialMonitor.println();

    float tensao1 = analogRead(A0);
    bat = (tensao1 * 5) / (1023) * 10;

    // Exibe o valor lido no pino a0
    Serial.print("voltagem bateria1 :");
    Serial.println(bat);

    float tensao2 = analogRead(A1);
    bat2 = (tensao2 * 5) / (1023) * 10;

    // Exibe o valor lido no pino a1
    Serial.print("voltagem bateria2 :");
    Serial.println(bat2);
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
    if (tinyGPS.time.minute() < 10)
        SerialMonitor.print('0');
    SerialMonitor.print(tinyGPS.time.minute());
    SerialMonitor.print(":");
    if (tinyGPS.time.second() < 10)
        SerialMonitor.print('0');
    SerialMonitor.println(tinyGPS.time.second());
}