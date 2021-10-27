/***************************************************************************
 * 
 * Автор Перевода: Обушенков Алексей Андреевич    
 * Группа в ВК https://vk.com/engineer24
 * Канал на YouTube https://www.youtube.com/channel/UCih0Gcl9IEdkR8deDJCiodg
 * Инженерка Engineering room
 * Милости просим в Телеграмм для обсуждения проектов ))) https://t.me/Engineering_room24
 * 
 ***************************************************************************
 *
 *  Этот скетч это базавая прошивка для DIY проектов. Как просто модулей ESP8266
 *  так и устройств от https://www.itead.cc/ 
 *  А в частности Sonoff Basic и Sonoff Basic RF (это те модули с которыми я точно проверял работу прошивки)
 *
 *  В этой прошивке считай все то же самое что и в прошивке из видео ниже
 *  #088 Меню настройки подключения к Blynk через Терминал Serial Menu Blynk Connect
 *  https://youtu.be/BVte6w9K9vg
 *  Но есть малые изменения которые добавили стабильности.
 *  
 *  Зачем вообще ломать модули Sonoff и заливать свою прошивку? Да черт знает зачем...
 *  Хочется просто...
 */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "Connect.h"
#include "SetEEPROM.h"
#include "SetupMenu.h"

// Подключаем вкладку Variables.h там хранятся переменные
#include "Variables.h" 

BlynkTimer timer;       // Объявляем класс Таймер



/////////////////////////////////////////
//            Код программы            //


// Считываем нажатие кнопки
void button()
{
    f_light = !f_light;
    digitalWrite(PIN_RELAY, f_light);

}

//// Считываем значение виджета BUTTON (в режиме PUSH) из приложения Блинк
//BLYNK_WRITE(VPIN_BUTTON_ZAL)
//{
//  if (param.asInt()){
//  f_light = !f_light;
//  digitalWrite(PIN_RELAY, f_light);
//  }
//}

// Считываем значение виджета BUTTON (в режиме SWITCH) из приложения Блинк
BLYNK_WRITE(VPIN_BUTTON)
{
  f_light = param.asInt();
  digitalWrite(PIN_RELAY, f_light);
}

  // Считываем и отправляем в Блинк состояние реле
void buttonBlynk()
{
    // Благодаря Last_f_light отправка в Блинк будет происходить, только после
    // изменения флага f_light с прошлого значения на новое
  if (Last_f_light != f_light)
      {
        Last_f_light = f_light;
        Blynk.virtualWrite(VPIN_BUTTON, f_light); //отправляем состояние реле в виртуальный пин V0
      }
}

void setupMenuStart(){
  SETUPmenu();
}



void setup() {
// Настраиваем Входы Выходы МК
  pinMode(PIN_RELAY, OUTPUT); //настраиваем пин Реле на выход
  digitalWrite(PIN_RELAY, f_light);
// Открываем Сериал монитор
  Serial.begin(9600);

// Задаем прерывания  
/*  Шпаргал по прерываниям http://arduino.ru/Reference/AttachInterrupt */
  
  // Прикрепляем прерывание Для ESP8266
  // У ESP8266 Прерывания могут быть назначены на любые GPIO кроме GPIO16.
  
  // Два способа:
  // 1-й  настраиваем PIN Кнопки на вход с подтяжкой к плюсу 
  // и тип прерывания FALLING (при смене значения на порту с HIGH на LOW)
      //pinMode(PIN_BUTTON, INPUT_PULLUP);// В Sonoff RF этот вывод и так подтянут к плюсу
      pinMode(PIN_BUTTON, INPUT);       // Потому просто INPUT (без PULLUP)
      attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), button, FALLING);
  
  // 2-й  настраиваем пин Кнопки на вход без подтяжки
  // в этом случае лучше этот пин через резистор на 10кОм притянуть к земле
  // и тип прерывания RISING (при смене значения на порту с LOW на HIGH)
//    pinMode(PIN_BUTTON, INPUT); 
//    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), button, RISING);




  
  Serial.begin(9600);
  delay(1000);
  setupAddrEEPROM();      // Создаем массив адресов для работы с EEPROM
  EEPROM_READ_CONNECT();  // Читаем данные из EEPROM, но проводим проверку не надо ли перезаписать зовадские настройки
                          // Если надо, то перезапишем в EEPROM заводские настройки 


// Настраеваем таймеры
// Задаем интервальные таймеры
  timer.setInterval(10000, reconnectBlynk);// Проверяем есть ли связь с сервером
  timer.setInterval(1000L, buttonBlynk);  // Считываем и отправляем в Блинк состояние реле 
  timer.setInterval(1000L, setupMenuStart);

// Вызываем функцию подключения к Blynk
  reconnectBlynk();
}//setup

void loop() {
  if (ONReconnect){ // Если мы зашли в SETUPmenu, то работа программы остановится
    if (Blynk.connected()){ Blynk.run();} 
    timer.run();
  }
  else{SETUPmenu();}
}//loop() 
