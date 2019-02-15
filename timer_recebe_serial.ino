/*
Código de exemplo de como receber os horas e minutos via serial.
Exemplo:
L0900 (Liga as 09:00 da manhã)
D0943 (Desliga as 09:43 da manhã)
Obs. tem que ter um caracter de terminação "\n" (NewLine)
*/

#include <Wire.h>
#include <RTClib.h>

RTC_Millis rtc;

const int CIRC1 = 8;

int horaLiga = 0;
int minutoLiga = 0;
int horaDesliga = 0;
int minutoDesliga = 0;

int tempo = 100;// em milisegundos
#define led 13
int INTERVALO_PrintRelogio = 1000;

//***prototipo de função***
String zero(int digits);
int alarme(int hora, int minuto);
int leSerial();

void setup()
{
  Serial.begin(57600);
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.begin(DateTime(__DATE__, __TIME__));
  delay(1000);
  pinMode(CIRC1, OUTPUT);
  pinMode(led, OUTPUT);
}//***end setup()

//******************************************
void loop ()
{
  leSerial();
  static unsigned long TempoAnterior = 0;          //Inicializa em 0ms.
  unsigned long TempoCorrente = millis();          //Retorna o tempo desde que o programa foi executado.

  if (TempoCorrente < TempoAnterior)
  {
    TempoAnterior = TempoCorrente = millis();
  }
  if ( (TempoCorrente - TempoAnterior) > INTERVALO_PrintRelogio)
  {
    TempoAnterior = TempoCorrente;

    DateTime now = rtc.now();
    alarme(now.hour(), now.minute());
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(zero(now.hour()));
    Serial.print(':');
    Serial.print(zero(now.minute()));
    Serial.print(':');
    Serial.print(zero(now.second()));
    Serial.println();
  }
}//***end loop()***

//******************************************
String zero(int digits)
{
  String lead;
  if (digits < 10)
  {
    lead = "0" + String(digits);
  }
  else
  {
    lead = String(digits);
  }
  return lead;
}//***end zero(int digits)***

//******************************************
int alarme(int hora, int minuto) {
  int hAtual = ((hora * 60) + minuto); // soma dos minutos hora atual
  int hLiga = (horaLiga * 60) + minutoLiga;
  int hDesliga = (horaDesliga * 60) + minutoDesliga;

  if (hAtual >= hLiga && hAtual < hDesliga)
  {
    digitalWrite(CIRC1, HIGH);
    digitalWrite(led, HIGH);
    Serial.println("LIgado as "+ (String)zero(horaLiga) + ":" +(String)zero(minutoLiga));
  }
  else
  {
    digitalWrite(CIRC1, LOW);
    digitalWrite(led, LOW);
    Serial.println("Desligado as "+ (String) zero(horaDesliga)+ ":" + (String)zero(minutoDesliga));
  }
}// *** end int alarme(int hora, int minuto)***

//******************************************
int leSerial() {
  if (Serial.available() > 0 )
  {
    unsigned long timeout = millis();
    String str1;
    char chr1;

    do
    {
      if (Serial.available() > 0)
      {
        chr1 =  Serial.read();
        delay(2);
        if (chr1 != '\n')
        {
          str1 += (char)chr1;
        }
      }
    } while ((chr1 != '\n') && ((millis() - timeout) < tempo/*ms*/));

    if ((millis() - timeout) > tempo/*ms*/ || (chr1 != '\n'))//Se acontecer um timeout ou não tiver o caractere de terminação,   
    {                                                        //limpar os dados recebidos
      str1 = "";                                              
      Serial.println("TimeOUT");
    }

    String cmd = str1.substring(0, 1);
    String h = str1.substring(1, 3);
    String m = str1.substring(3, 5);

    if (cmd == "L")
    {
      cmd = "";
      Serial.println("Liga as " + h + ":" + m);
      horaLiga = h.toInt();
      minutoLiga = m.toInt();
    }
    else if (cmd == "D")
    {
      cmd = "";
      Serial.println("Desliga as " + h + ":" + m);
      horaDesliga = h.toInt();
      minutoDesliga = m.toInt();
    }
  }
}//***end int leSerial()***
