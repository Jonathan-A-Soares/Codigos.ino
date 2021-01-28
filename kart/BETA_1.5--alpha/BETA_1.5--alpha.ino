#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include<SoftwareSerial.h>    
#include <TinyGPS++.h> // Include the TinyGPS++ library
#include <DS3231.h>      //Biblioteca para manipulação do DS3231


#define SS_PIN 10
#define RST_PIN 9
#define ARDUINO_GPS_RX 5 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 4 // GPS RX, Arduino TX pin
#define gpsPort ssGPS  // Alternatively, use Serial1 on the Leonardo
#define SerialMonitor Serial
#define GPS_BAUD 9600

LiquidCrystal_I2C lcd(0x27, 16, 2);//declaraçao de objeto
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
SoftwareSerial ssGPS(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
TinyGPSPlus tinyGPS; // Create a TinyGPSPlus object
DS3231 clock; //declaraçao de objeto
RTCDateTime dt;//declaraçao de objeto


//variaveis globais---------------------------------------------------------------------------------------------------------

float lat, lon, vel;                                                                                 //
unsigned long data, hora;                                                                      //
unsigned short sat;                                                                          //
int p = 0;                                                                                    //
int  tempBat,  tempbat2, KM, timet, tempAB, tempM, tempC, rlm, rmC, hjk;  //
float bat, bat2;
int Horas, Minutos, Segundos, Ano, Mes, Dia ;
int Horas1, Minutos1, Segundos1, Ano1, Mes1, Dia1 ;
int HorasF, MinutosF, SegundosF, AnoF, MesF, DiaF ;
float temperaturaControl;

int ThermistorPin = 2;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int ThermistorPin1 = 3;
int Vo1;
float R11 = 10000;
float logR21, R21, T1, Tc1, Tf1;
float c11 = 1.009249522e-03, c21 = 2.378405444e-04, c31 = 2.019202697e-07;



//======================================================FUNÇÃO do bip==============================
void Bip(int tme){
 switch(tme) {
  case 1:
    tone(6,600);    
    delay(50);
    noTone(6); 
    
    break;
  case 2:
    tone(6,600);    
    delay(20);
    noTone(6); 
    
    break;
  case 3:
    tone(6,600);    
    delay(100);
    noTone(6); 
    
    break;
  case 4:
    tone(6,349);    
    delay(400);
    noTone(6); 
    
    break;
  case 5:
    tone(6,392);    
    delay(400);
    noTone(6); 
    
    break;
  default:
  noTone(6);
  
    
}

 
 }
//========================================
void setup()
{
          
    //relogio
    clock.begin();
    clock.forceConversion();
    dt = clock.getDateTime();
    
    
    
    //GPS
    analogReference(DEFAULT);
    gpsPort.begin(GPS_BAUD);
    SerialMonitor.begin(9600);
    
    //LCD
    Serial.begin(9600);
    lcd.init();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Bloqueado");
    lcd.setCursor(0, 1);
    lcd.print("Aproxime A Tag");
    delay(500);
    
    //nfc
    Serial.begin(9600); // Inicia a serial
    SPI.begin();        // Inicia  SPI bus
    mfrc522.PCD_Init(); // Inicia MFRC522
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();
    // declaraçao de porta
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(3, INPUT_PULLUP);
    
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
              //verifica se cartão esta disponivel
                if (digitalRead(2) == HIGH)
                {
                    o = 1;
                }
                else
                {
                    o = 5;
                }
                // ================bem vindo sempre que a autoriçazão===========================================================
                 
                    
                    
                while (p < 2)
                { 
                  
                    Bip(1);//aviso sonoro
                    p = 2;
                    
                    lcd.clear(); // coteudo depois do desbloqueio
                    lcd.setCursor(4, 0);
                    lcd.print("Bem Vindo");
                    
                    //=========================verifica qual o carta oe para customizar msg de bom dia======================
                    if (conteudo.substring(1) == "F7 F7 30 63" ){
                      lcd.setCursor(1, 1);
                     lcd.print("Jonathan Soares");
                    }else{
                      lcd.setCursor(4, 1);
                     lcd.print("Visitante");
                    }
                    delay(2000);
                    lcd.clear(); 
                    
                }
                //========================================== a seguir  do bem vindo================================================================
                
                 //================================data================================
                lcd.setCursor(0, 0);
                lcd.print("Data: ");
                lcd.print(dt.day);   lcd.print("/");
                lcd.print(dt.month);  lcd.print("/");
                lcd.print(dt.year);    lcd.print(" ");
                
                  //================================horas================================
                delay(30);
                lcd.setCursor(0, 1);
                lcd.print("Horas: ");
                lcd.print(dt.hour);   lcd.print(":");
                lcd.print(dt.minute);
                
                delay(5000);
                lcd.clear();  

                 if(digitalRead(3) == LOW){ 
                  
                  //=========SO QUANDO PRECIONO BOTAO MOSTRA INFORMAÇOES DETALHADAS========================== 

                  
                //================================temperatura do Ambiente================================
                lcd.setCursor(0, 0);
                lcd.print("Ambiente: ");
                lcd.print(clock.readTemperature());
                delay(3000);
                lcd.clear();


                //================================temperatura do motor================================
                lcd.setCursor(0, 0);
                lcd.print("Motor: ");
                lcd.print(Tc);
                lcd.print("C");
                
                //================================temperatura da controladora================================
                lcd.setCursor(0, 1);
                lcd.print("Control: ");
                lcd.print(Tc1);
                lcd.print("C");
                delay(3000);
                lcd.clear();

                
                 //================================voltagem baterias================================
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

                
                
                //================================ latitude longtude================================
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


                //================================verifica se cartão esta disponivel================================
                if (digitalRead(2) == HIGH)
                {
                    o = 1;
                }
                else
                {
                    o = 5;
                }

                
                //================================altitude================================
                lcd.setCursor(0, 0);
                lcd.print("Alt: "); 
                lcd.print(tinyGPS.altitude.meters());
                lcd.print("M");
                delay(3000);
                lcd.clear();


                
                //=================================velocida km/h================================
                lcd.setCursor(0, 0);
                lcd.print("Speed: ");
                lcd.print(tinyGPS.speed.kmph());
                lcd.print("Km/h ");
                delay(3000);
                lcd.clear();


                
                //====================================satelites================================
                lcd.setCursor(0, 0);
                lcd.print("Satelites: "); 
                lcd.print(tinyGPS.satellites.value());
                delay(3000);
                lcd.clear();
              }else{ 
                
                // ===========================SE BOTAO ESTIVER DESLIGADO mostra velocidade e hora====================

                
                //======================================velocida km/h================================
                lcd.setCursor(0, 0);
                lcd.print("Speed: ");
                lcd.print(tinyGPS.speed.kmph());
                lcd.print("Km/h ");
                delay(3000);
                lcd.clear();
                
                }         
                
               
                

                

                //========================verifica se cartao esta prensete===========================================
                lcd.clear();
                if (digitalRead(2) == HIGH)
                {
                    o = 1;
                }
                else
                {
                    o = 5;
                }
                //========================================chama fuunçao do gps===================================================
                // print position, altitude, speed, time/date, and satellites:
                printGPSInfo();
              
                // "Smart delay" looks for GPS data while the Arduino's not doing anything else
                smartDelay(1000); 
                  
                                  
                //================================================================================================
            }
           
            delay(1000);
        }
        else // =======================================================fim autenticaçao=======================================================
        {    // se nao for autenticado apresata erro no dysplay
            Bip(5);//aviso sonoro
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Acesso Negado!");
            lcd.setCursor(2, 1);
            lcd.print("Tag Recusada");
            Serial.println();
            Serial.println("Acesso Negado");
            Serial.println("------------------------");
            digitalWrite(7, HIGH); //liga led vermelho
            digitalWrite(3, HIGH); // deliga rele
        }

        //=====================================================fim do if================================
    }
    else if (digitalRead(2) == LOW) // VERIFICA SE CARTAO NAO ESTA  presente
    {                               // Botão nao Pressionado
          Serial.println("------------------------");
          Serial.println("Cartao Nao Esta Presente"); // mostra no monitor serial que caratao/tag esta ausente
          Serial.println("------------------------");
          t = 1;// VARIAVEL DE CONTROLE
          digitalWrite(7, HIGH); //liga led vermelho
          digitalWrite(8, LOW);
          digitalWrite(3, HIGH); // deliga rele
        //========================MESNAGEM DE QUE ESTA BLOQEUADO============================================
           lcd.clear();
           lcd.setCursor(4, 0);
           lcd.print("Bloqueado");
           delay(30);
           lcd.setCursor(1, 1);
           lcd.print("Aproxime a Tag");
           
                  delay(3000);
                  lcd.clear();
            //================================data================================
                lcd.setCursor(0, 0);
                lcd.print("Data: ");
                lcd.print(dt.day);   lcd.print("/");
                lcd.print(dt.month);  lcd.print("/");
                lcd.print(dt.year);    lcd.print(" ");
                
                  //================================horas================================
                delay(30);
                lcd.setCursor(0, 1);
                lcd.print("Horas: ");
                lcd.print(dt.hour);   lcd.print(":");
                lcd.print(dt.minute); 
               
                delay(3000);
                 
                  
        //==============================================================================
              
        delay(1000);
       
    }

            delay(1000);
        // print position, altitude, speed, time/date, and satellites:
          printGPSInfo();
        
          // "Smart delay" looks for GPS data while the Arduino's not doing anything else
          smartDelay(1000); 
 
}//===============================================================================fim do loop===================================================



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

  //===========================calculo voltagem=======================================================

                                            Serial.println("------------------------");
                                            float tensao1 = analogRead(A0)*(5.0/1023.0)*10;
                                            bat = tensao1;
                                            float tensao2 = analogRead(A1)*(5.0/1023.0)*10;
                                            bat2 = tensao2;
                                          

                                            
                                            Serial.print("voltagem bateria 1 :");Serial.println(bat,5);
                                            
                                             if(bat >= 11.5){
                                              
                                               Serial.println("bateria Boa");
                                            }else if (bat2 <=11){
                                              Serial.println("Bateria Baixa");
                                            }
                                            Serial.println("------------------------");
                                            
                                             Serial.print("voltagem bateria 2 :");Serial.println(bat2,5);
                                             if(bat2 >= 11.5){
                                              
                                               Serial.println("bateria 2 Boa");
                                            }else if (bat2 <=11){
                                              Serial.println("Bateria 2 Baixa");
                                            }


                                            
                                            
                                            Serial.println("------------------------");
      //===================================================temperatura motor===============================================
                              
                                            
                                            Vo1 = analogRead(ThermistorPin);
                                            R21 = R11 * (1023.0 / (float)Vo1 - 1.0);
                                            logR21 = log(R21);
                                            T1 = (1.0 / (c11 + c21*logR21 + c31*logR21*logR21*logR21));
                                            Tc1 = T1 - 273.15;
                                            Tf1 = (Tc1 * 9.0)/ 5.0 + 32.0; 
                                              
                                            Serial.print("Temperatura Controladora: "); 
                                            Serial.print(Tc1);
                                            Serial.println(" Cº"); 
                                            

    //===================================================temperatura ambiente===============================================
                        
                                           
                                            Vo = analogRead(ThermistorPin1);
                                            R2 = R1 * (1023.0 / (float)Vo - 1.0);
                                            logR2 = log(R2);
                                            T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
                                            Tc = T - 273.15;
                                            Tf = (Tc * 9.0)/ 5.0 + 32.0; 
                                          
                                            Serial.print("Temperature Motor: "); 
                                            Serial.print(Tc);
                                            Serial.println(" Cº");   
                                            Serial.println("------------------------");
                                    

    
  //==========================================================data e hora=======================================================
                                              

                                              
                                              
                                              Serial.print("Data: ");
                                              Serial.print(dt.year);   Serial.print("-");
                                              Serial.print(dt.month);  Serial.print("-");
                                              Serial.print(dt.day);    Serial.print(" ");
                                              Serial.print(dt.hour);   Serial.print(":");
                                              Serial.print(dt.minute); Serial.print(":");
                                              Serial.print(dt.second); Serial.println("");
                                              Serial.println("------------------------");
                                              
                                            
                                              Serial.print("Temperatura Ambiente: ");
                                              Serial.println(clock.readTemperature());

                                             

                                              
                                              
                                              
                                              
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
