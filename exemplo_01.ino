#include <Wire.h>
#include <RTClib.h>

RTC_Millis rtc;
const int CIRC1 = 8;
const int AUTO = 3;
const int MANUAL = 4;

int horaLiga = 14;
int minutoLiga = 26;
int horaDesliga = 05;
int minutoDesliga = 27;
int estadobotao = 0;
int estadobotao2 = 0;

void setup() {
  Serial.begin(57600);
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.begin(DateTime(__DATE__, __TIME__));
  delay(1000);
  pinMode(CIRC1, OUTPUT);
  pinMode(AUTO, INPUT);
  pinMode(MANUAL, INPUT);
}

void loop () {
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
  delay(3000);
}
String zero(int digits) {
  String lead;
  if (digits < 10) {
    lead = "0" + String(digits);
  }
  else {
    lead = String(digits);
  }
  return lead;
}

int alarme(int hora, int minuto) {
  int hAtual = ((hora * 60) + minuto); // soma dos minutos hora atual
  int hLiga = (horaLiga * 60) + minutoLiga;
  int hDesliga = (horaDesliga * 60) + minutoDesliga;
  if (hDesliga < hLiga) {
    if (hAtual >= hLiga) {
      digitalWrite(CIRC1, HIGH);
      //Serial.println("teste") ;
    }
    if (hAtual >= hDesliga && hAtual < hLiga) {
      digitalWrite(CIRC1, LOW);
      //Serial.println("teste2") ;
    }
  }
  if (hDesliga > hLiga) {
    if (hAtual >= hLiga and hAtual <= hDesliga) {
      digitalWrite(CIRC1, HIGH);
    }
    if (hAtual >= hDesliga) {
      digitalWrite(CIRC1, LOW);
    }
  }
}
