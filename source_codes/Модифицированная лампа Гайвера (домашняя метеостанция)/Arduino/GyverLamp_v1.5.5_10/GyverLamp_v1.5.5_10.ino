// Ссылка для менеджера плат:
// http://arduino.esp8266.com/stable/package_esp8266com_index.json

// Для NodeMCU выбираем NodeMCU 1.0 (ESP-12E Module) - ядро обязательно 2.5.2!!!!

// ============= НАСТРОЙКИ =============
// -------- КНОПКА -------
#define USE_BUTTON 1    // 1 - использовать кнопку, 0 - нет

// ---------- МАТРИЦА ---------
#define BRIGHTNESS 255         // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 0    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define WIDTH 16              // ширина матрицы
#define HEIGHT 16             // высота матрицы

#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
// при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
// шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/


// ============= ДЛЯ РАЗРАБОТЧИКОВ =============
#define LED_PIN D4             // пин ленты
#define BTN_PIN D5             // пин кнопки
#define MODE_AMOUNT 18

// раздефайнить или задефайнить для использования
//#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define DEBUG(x) Serial.println(x)
#else
#define DEBUG(x)
#endif

#define NUM_LEDS WIDTH * HEIGHT
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
// ---------------- БИБЛИОТЕКИ -----------------
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define NTP_INTERVAL 60 * 1000    // обновление (1 минута)

#include "timer2Minim.h"
#include <FastLED.h>
#include <EEPROM.h>
#include <GyverButton.h>
#include "fonts.h"

// ------------------- ТИПЫ --------------------
CRGB leds[NUM_LEDS];
timerMinim timeTimer(1000);
timerMinim timeStrTimer(120);
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

// ----------------- ПЕРЕМЕННЫЕ ------------------
String inputBuffer;
static const byte maxDim = max(WIDTH, HEIGHT);
struct {
  byte brightness = 50;
  byte speed = 30;
  byte scale = 40;
} modes[MODE_AMOUNT];

struct {
  boolean state = false;
  int time = 0;
} alarm[7];

const byte dawnOffsets[] = {5, 10, 15, 20, 25, 30, 40, 50, 60};
byte dawnMode;
boolean dawnFlag = false;
float thisTime;
boolean manualOff = false;
boolean sendSettings_flag = false;

int8_t currentMode = 0;
boolean loadingFlag = true;
boolean ONflag = true;
uint32_t eepromTimer;
boolean settChanged = false;
// Конфетти, Огонь, Радуга верт., Радуга гориз., Смена цвета,
// Безумие 3D, Облака 3D, Лава 3D, Плазма 3D, Радуга 3D,
// Павлин 3D, Зебра 3D, Лес 3D, Океан 3D,

unsigned char matrixValue[8][16];
String lampIP = "";
byte hrs, mins, secs;
byte days;
String timeStr = "00:00";

//*****************************************************************Tigran-Begin*************************************************************************
#define BLYNK_PRINT Serial //Blynk+MeteoControl
#include <BlynkSimpleEsp8266.h>
#include <ArduinoOTA.h> // Для прошивки по вафле
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#include <IRremoteESP8266.h> //IK Control
#include <IRsend.h>
#include <ir_Coolix.h>  //  replace library based on your AC unit model, check https://github.com/crankyoldgit/IRremoteESP8266

#define SEALEVELPRESSURE_HPA (1013.25) //Blynk+MeteoControl
Adafruit_BME280 bme; // I2C

char auth[] = "";
char* ssid[] = {""}; //Можно указать несколько сетей через запятую 
char* pass[] = {""};

BlynkTimer timer;
WidgetRTC rtc;
WidgetBridge SmartVent(V11); //Это мост! У вас он должен быть свой!!!
WidgetBridge MySmartHumidifier(V12); //Это мост! У вас он должен быть свой!!!

const uint16_t kIrLed = 0;  //IK Control // The ESP GPIO0(D3) pin to use that controls the IR LED.

long my_timer;
int attempt_conect_count=0;

IRsend irsend(kIrLed);
IRCoolixAC ac(kIrLed); 

void setup() {
  ESP.wdtDisable();

  // ЛЕНТА
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection( TypicalLEDStrip )*/;
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.show();

  touch.setStepTimeout(100);
  touch.setClickTimeout(500);
  
  Serial.begin(9600);

  // EEPROM
  DEBUG("EEPROM");
  EEPROM.begin(202);
  delay(50);
  if (EEPROM.read(198) != 20) {   // первый запуск
    EEPROM.write(198, 20);
    EEPROM.commit();

    for (byte i = 0; i < MODE_AMOUNT; i++) {
      EEPROM.put(3 * i + 40, modes[i]);
      EEPROM.commit();
    }
    for (byte i = 0; i < 7; i++) {
      EEPROM.write(5 * i, alarm[i].state);   // рассвет
      eeWriteInt(5 * i + 1, alarm[i].time);
      EEPROM.commit();
    }
    EEPROM.write(199, 0);   // рассвет
    EEPROM.write(200, 0);   // режим
    EEPROM.commit();
  }
  for (byte i = 0; i < MODE_AMOUNT; i++) {
    EEPROM.get(3 * i + 40, modes[i]);
  }
  for (byte i = 0; i < 7; i++) {
    alarm[i].state = EEPROM.read(5 * i);
    alarm[i].time = eeGetInt(5 * i + 1);
  }
  dawnMode = EEPROM.read(199);
  currentMode = (int8_t)EEPROM.read(200);

  DEBUG("отправляем настройки");
  
  memset(matrixValue, 0, sizeof(matrixValue));
  randomSeed(micros());

  //*****************************************************************Tigran-Setup*************************************************************************
  DEBUG("Tigran-Setup");
  // D1,D2 - I2C by BME280-Module
  // D3 - IR
  // D4 - led
  // D5 - buttion
  pinMode(12,OUTPUT); //D6 Вентилятор
  ArduinoOTA.setHostname("MeteoControl");
  ArduinoOTA.begin();
  MultyWiFiBlynkBegin();
  timer.setInterval(1000*30, SendAirProperties);
  timer.setInterval(49, lamp_AutoBr);
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  if (!bme.begin()) {DEBUG("Could not find a valid BME280 sensor, check wiring!");}
  my_timer = millis(); 
  ac.begin();
  irsend.begin();
}

void loop() {
//  parseUDP();
  effectsTick();
  eepromTick();
//  timeTick();
#if (USE_BUTTON == 1)
  buttonTick();
#endif
  ESP.wdtFeed();   // пнуть собаку
  yield();  // ещё раз пнуть собаку

  //*****************************************************************Tigran-Loop*************************************************************************
  ArduinoOTA.handle();
  if (Blynk.connected()) {
    Blynk.run();
    my_timer = millis(); // "сбросить" таймер
    attempt_conect_count=0;  // "сбросить" счетчик попыток перподключиться
  } else {
    if (millis() - my_timer > 60*1000) { // Таймер чтобы не чаще чем раз в 60 сек. пытался законектиться
      DEBUG("Потерена связь с Блянком, делаю повторную попытку");
      Blynk.run();
      if (!Blynk.connected()) {
        DEBUG("Нет результата пробую преподключться... попытка №"+String(attempt_conect_count));
        MultyWiFiBlynkBegin(); // 
        attempt_conect_count++;
        if (attempt_conect_count>3) {ESP.deepSleep(1e6);} //Заснуть на 1 сек. (Перегрузить)
      }
      my_timer = millis();   // "сбросить" таймер
    }
  }  
  timer.run();
}

void eeWriteInt(int pos, int val) {
  byte* p = (byte*) &val;
  EEPROM.write(pos, *p);
  EEPROM.write(pos + 1, *(p + 1));
  EEPROM.write(pos + 2, *(p + 2));
  EEPROM.write(pos + 3, *(p + 3));
  EEPROM.commit();
}

int eeGetInt(int pos) {
  int val;
  byte* p = (byte*) &val;
  *p        = EEPROM.read(pos);
  *(p + 1)  = EEPROM.read(pos + 1);
  *(p + 2)  = EEPROM.read(pos + 2);
  *(p + 3)  = EEPROM.read(pos + 3);
  return val;
}
