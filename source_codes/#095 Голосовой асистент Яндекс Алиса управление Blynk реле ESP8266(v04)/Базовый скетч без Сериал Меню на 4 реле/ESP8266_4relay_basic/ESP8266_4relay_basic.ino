// Базовый простой скетч
// Что бы понять работу и основу Blynk

#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define DoorPin_1 D5
#define DoorPin_2 D6
#define DoorPin_3 D7
#define DoorPin_4 D8

// Для реле с обратной логикой 1(это выключено)
// Для реле с прямой логикой 0 (это выключено)
bool rele1 = 1;
bool rele2 = 1;
bool rele3 = 1;
bool rele4 = 1;


char auth[] = "??????????????????????"; //  сюды токен
char ssid[] = "?????????????????????"; // имя вашего вайфая
char pass[] = "????????????????????????"; // пароль от вайфая

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode( DoorPin_1, OUTPUT); 
  pinMode( DoorPin_2, OUTPUT);
  pinMode( DoorPin_3, OUTPUT); 
  pinMode( DoorPin_4, OUTPUT);
  digitalWrite(DoorPin_1, rele1);
  digitalWrite(DoorPin_2, rele2);
  digitalWrite(DoorPin_3, rele3);
  digitalWrite(DoorPin_4, rele4);

}

BLYNK_WRITE(V0) // запрос данных с датчика
{
rele1= param.asInt();
digitalWrite(DoorPin_1, rele1);  
}
BLYNK_WRITE(V1) // запрос данных с датчика
{
rele2= param.asInt();
digitalWrite(DoorPin_2, rele2);
}
BLYNK_WRITE(V2) // запрос данных с датчика
{
rele3= param.asInt();
digitalWrite(DoorPin_3, rele3);
}

BLYNK_WRITE(V3) // запрос данных с датчика
{
rele4= param.asInt();
digitalWrite(DoorPin_4, rele4);
}

void loop(){
  Blynk.run();
}
