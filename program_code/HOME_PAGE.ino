#include <BlynkSimpleEsp8266.h>  // Библиотека для работы с сервисом Blynk
#include <ESP8266WiFi.h>         // Библиотека для работы c ESP
#include <FastLED.h>             // Библиотека для работы с адресной лентой

//------------------------------------НАСТРОЙКИ------------------------------------
#define LED_COUNT 76             // Число светодиодов в кольце/ленте
#define LED_DT 4                 // Пин, куда подключен DIN ленты
#define LED D0                   // Пин подключения лампы
#define LED1 D3 

char auth[] = "gJU47e6pI6OwdGFMbdg4TGuCzSstQ7CH";   // Токен приложения Blynk
char ssid[] = "AndroidAP2435";                      // Название WiFi(Точки доступа)
char pass[] = "5.12.2018";                          // Пароль WiFi  (Точки доступа)

int brightnes = 127.5; // Изначальная ярколсть подсветки
int brightnes1 = 255;  // Изначальная ярколсть лампы
int lightnes = 0;      // Изначальное вкл/выкл подсветки
int light = 0;         // Изначальное вкл/выкл лампы
int  mode = 3;         // Изначальный режим подсветки

////////////////////////////////Служебные переменные////////////////////////////////
struct CRGB leds[LED_COUNT];

int max_bright = 201;
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

void setup() {
  Serial.begin(115200);                                // Запуск сериал порта
  Blynk.begin(auth, ssid, pass);                       // Запуск Blynk (подключение,токен,WiFi,пароль);
  LEDS.setBrightness(max_bright);                      // Ограничить максимальную яркость
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // Настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
  one_color_all(0, 0, 0);                              // Погасить все светодиоды
  LEDS.show();                                         // Отослать команду
  pinMode(LED, OUTPUT);                                // Пин лампы в режиме выхода
  pinMode(LED1, OUTPUT);
}

void loop() {
  Blynk.run();
  if (brightnes1 >= 250){
    analogWrite (LED1, 255);
  } else { analogWrite (LED1, 0);
  }
  analogWrite(LED, light ? brightnes1 : 0);
  if (lightnes == 1) {
    switch (mode) {
      case 1: one_color_all(0, 0, 0); LEDS.show(); break;  // Сбросить цвет
      case 2: break;                                       // Паза
      case 3: rainbow_fade(); break;                       // Плавная смена цветов
      case 4: rainbow_loop(); break;                       // Радуга
      case 5: ems_lightsALL(); break;                      // Вращается половина красных и половина синих
      case 6: ems_lightsSTROBE(); break;                   // Полицейская мигалка
      case 7: one_color_all(0, 0, brightnes); LEDS.show(); break;        // Синий
      case 8: one_color_all(brightnes, 0, 0); LEDS.show(); break;        // Красный
      case 9: one_color_all(0, brightnes, 0); LEDS.show(); break;        // Зелёный
      case 10: one_color_all(brightnes, brightnes, 0); LEDS.show(); break; // Жёлтый
      case 11: one_color_all(0, brightnes, brightnes); LEDS.show(); break; // Берюзовый
      case 12: one_color_all(brightnes, 0, brightnes); LEDS.show(); break; // Фиолетовый
      case 13: one_color_all(brightnes, brightnes, brightnes); LEDS.show(); break;// Белый
      case 14: Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000); break;       // Стробоскоп
    }
  } else {
    one_color_all(0, 0, 0);
    LEDS.show();

  }
}
