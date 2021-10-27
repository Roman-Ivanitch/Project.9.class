#include <BlynkSimpleEsp8266.h>  // Библиотека для работы с сервисом Blynk
#include <ESP8266WiFi.h>         // Библиотека для работы c ESP
#include <FastLED.h>             // Библиотека для работы с адресной лентой

//------------------------------------НАСТРОЙКИ------------------------------------
#define LED_COUNT 50            // Число светодиодов в кольце/ленте

#define LED_DT 4                // Пин, куда подключен DIN ленты
#define LED 14                  // Пин подключения лампы
#define LED1 12                 // Пин подключения реле

#define NET1 5                  // пин подключения кнопки выбора сети 1              network - сеть
#define NET2 13                 // пин подключения кнопки выбора сети 2

char auth[] = "токен";          // лампа Blynk-cloud.com // Токен приложения Blynk
char auth_server[] = "токен";   // лампа local-server    // Токен приложения Blynk

char ssid_home[] = "wi-fi-home-0000575";  // сеть wi-fi
char pass_home[] = "7851xdwifi";          // Пароль WiFi

char ssid_phone[] = "A30";                // Мобильная точка доступа WiFi
char pass_phone[] = "87651234";           // Пароль WiFi

char ip_local[] = "192.168.88.200";       // свой server - local ip
char ip_net[] = "ip адрес";               // свой server - internet ip

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
//                         при первом включении
int brightnes = 127.5; // Изначальная ярколсть подсветки (от 0 до 255)
int brightnes1 = 127.5;// Изначальная ярколсть лампы     (от 0 до 255)
int lightnes = 0;      // Изначальное вкл/выкл подсветки ( 0 - выкл, 1 - вкл )
int light = 0;         // Изначальное вкл/выкл лампы     ( 0 - выкл, 1 - вкл )
int  mode = 3;         // Изначальный режим подсветки

void setup() {
  pinMode(NET1, INPUT);                 // пин кнопки выбоа сети1 как вход
  pinMode(NET2, INPUT);                 // пин кнопки выбоа сети2 как вход

  pinMode(LED, OUTPUT);                 // Пин лампы в режиме выхода
  pinMode(LED1, OUTPUT);                // пин реле лампы

  // Запуск Blynk (подключение,токен,WiFi,пароль) в зависимости от кнопки // выбор сети
  if      (digitalRead(NET1) == 1) Blynk.begin(auth, ssid_phone, pass_phone);   // мобил точка доступа и Blynk-cloud.com
  else if (digitalRead(NET2) == 1) Blynk.begin(auth_server, ssid_phone, pass_phone, ip_net, 8080); // мобил точка доступа и свой сервер через интернет
  else Blynk.begin(auth_server, ssid_home, pass_home, ip_local, 8080);                          // точка доступа wifi  и свой локальный сервер

  LEDS.setBrightness(max_bright);                      // Ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // Настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);                              // Погасить все светодиоды
  LEDS.show();                                         // Отослать команду
}

void loop() {
  Blynk.run();    // работа через интернет
  light_lamp();   // работа света
  mod ();         // работа режимов подсветки
}
