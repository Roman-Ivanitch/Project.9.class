#include <BlynkSimpleEsp8266.h>  // Библиотека для работы с сервисом Blynk
#include <ESP8266WiFi.h>         // Библиотека для работы c ESP
#include <FastLED.h>             // Библиотека для работы с адресной лентой
#include <GyverPortal.h>
#include <EEPROM.h>
//------------------------------------НАСТРОЙКИ------------------------------------
#define LED_COUNT 52            // Число светодиодов в кольце/ленте

#define LED_DT 14               // Пин, куда подключен DIN ленты
#define LED 12                  // Пин подключения лампы

#define NET1 5                  // пин подключения кнопки выбора сети 1              network - сеть
#define NET2 13                 // пин подключения кнопки выбора сети 2

struct Data {
  char token[40];  // учетные данные, если запускаете впервый раз, 
  char ssid[20];   // то на web странице вводите токен из приложения Blynk, 
  char pass[20];   // название и пароль wi-fi/точки доступа, 
  char server[20]; // сервер Blynk (стандартный blynk-cloud.com) или же свой (ip сервера)
  uint16_t port;   // и порт ( обычно 8080, или 80, или если есть ssl, то 443 вроде...)

  char token1[40];
  char ssid1[20];
  char pass1[20];
  char server1[20];
  uint16_t port1;
};
Data data;
GyverPortal portal;

////////////////////////////////Служебные переменные////////////////////////////////
struct CRGB leds[LED_COUNT];

byte max_bright = 201;
byte red, green, blue;
int ihue = 0;
int idex = 0;

int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
int ledsX[LED_COUNT][3];
int BOTTOM_INDEX = 0;

int thissat   =  255;
int thisdelay =  20;
int thisstep  =  10;
int thishue   =  0;
int ibright   =  0;             //-BRIGHTNESS (0-255)

float tcount = 0.0;          //-INC VAR FOR SIN LOOPS

int brightnes;     // ярколсть подсветки (от 0 до 255)
int brightnes1;    // ярколсть лампы     
int lightnes;      // вкл/выкл подсветки 
int light;         // вкл/выкл лампы     
int  mode = 3;     // режим подсветки

void setup() {
  EEPROM.begin(500);     // читаем данные из памяти
  EEPROM.get(0, data);
  
  pinMode(LED, OUTPUT);                 // Пин лампы в режиме выхода
  pinMode(NET1, INPUT);                 // пин тумблера для настройки через web
  pinMode(NET2, INPUT);                 // пин тумблера выбоа сети2 
  
  LEDS.setBrightness(max_bright);                      // Ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // Настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);                              // Погасить все светодиоды
  LEDS.show();                                         // Отослать команду
  
  if (digitalRead(NET1) == 1) web(); // положение тумблера для web интерфейса 
  else if (digitalRead(NET2) == 1)Blynk.begin(data.token1, data.ssid1, data.pass1, data.server1, data.port1); 
  else Blynk.begin(data.token, data.ssid, data.pass, data.server, data.port); // центральное положение тумблера
}

void loop() {
  Blynk.run();    // работа через интернет
  light_lamp();   // работа света
  mod ();         // работа режимов подсветки
}
