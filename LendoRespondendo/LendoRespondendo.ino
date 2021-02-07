//Autor:https://github.com/Jonathan-A-Soares
#include <Sim800l.h>
#include <SoftwareSerial.h>
/*
 *    PINOUT: 

 *        GND          >>>   GND
 *        RX  9       >>>   TX    
 *        TX  10        >>>   RX
 *       RESET 2       >>>   RST 
*/

Sim800l Sim800l;
String textSms, numberSms ;
char* number ;
bool error;
void setup()
{
  Serial.begin(9600);


  Sim800l.begin();
  Serial.print("Limpando SMS antigos...");
  error = Sim800l.delAllSms(); //Apaga SMS
  Serial.println(" Apagados!");
  Serial.println("\nAguardando comandos por SMS...");
  number = "35000000000";
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
        error = Sim800l.sendSms(number, "Alarme Disparado");

      }


      Sim800l.delAllSms();
    }
  }
}
