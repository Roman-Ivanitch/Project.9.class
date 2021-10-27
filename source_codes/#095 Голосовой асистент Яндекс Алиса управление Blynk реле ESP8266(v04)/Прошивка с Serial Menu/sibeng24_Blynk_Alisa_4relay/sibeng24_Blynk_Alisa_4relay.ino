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
 *  В этой прошивке организованно управление 4 реле
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


// Оброботчик прерывания. Эта функция выполниться если сработает прерывание
void buttonInterrupt1(){StateRelay1 = !StateRelay1; digitalWrite(PIN_RELAY1, StateRelay1);}
void buttonInterrupt2(){StateRelay2 = !StateRelay2; digitalWrite(PIN_RELAY2, StateRelay2);}
void buttonInterrupt3(){StateRelay3 = !StateRelay3; digitalWrite(PIN_RELAY3, StateRelay3);}
void buttonInterrupt4(){StateRelay4 = !StateRelay4; digitalWrite(PIN_RELAY4, StateRelay4);}



// Считываем значение виджета BUTTON (в режиме SWITCH) из приложения Блинк
BLYNK_WRITE(VPIN_BUTTON1){StateRelay1 = param.asInt(); digitalWrite(PIN_RELAY1, StateRelay1);}
BLYNK_WRITE(VPIN_BUTTON2){StateRelay2 = param.asInt(); digitalWrite(PIN_RELAY2, StateRelay2);}
BLYNK_WRITE(VPIN_BUTTON3){StateRelay3 = param.asInt(); digitalWrite(PIN_RELAY3, StateRelay3);}
BLYNK_WRITE(VPIN_BUTTON4){StateRelay4 = param.asInt(); digitalWrite(PIN_RELAY4, StateRelay4);}

  // Считываем и отправляем в Блинк состояние реле
void buttonBlynk()
{
    // Благодаря LStateRelay отправка в Блинк будет происходить, только после
    // изменения флага StateRelay с прошлого значения на новое
  if (LStateRelay1 != StateRelay1){LStateRelay1 = StateRelay1; Blynk.virtualWrite(VPIN_BUTTON1, StateRelay1);}
  if (LStateRelay2 != StateRelay2){LStateRelay2 = StateRelay2; Blynk.virtualWrite(VPIN_BUTTON2, StateRelay2);}
  if (LStateRelay3 != StateRelay3){LStateRelay3 = StateRelay3; Blynk.virtualWrite(VPIN_BUTTON3, StateRelay3);}
  if (LStateRelay4 != StateRelay4){LStateRelay4 = StateRelay4; Blynk.virtualWrite(VPIN_BUTTON4, StateRelay4);}
}

void setupMenuStart(){
  SETUPmenu();
}



void setup() {
// Настраиваем Входы Выходы МК
  pinMode(PIN_RELAY1, OUTPUT); //настраиваем пин Реле на выход
  pinMode(PIN_RELAY2, OUTPUT); 
  pinMode(PIN_RELAY3, OUTPUT); 
  pinMode(PIN_RELAY4, OUTPUT);
  digitalWrite(PIN_RELAY1, StateRelay1);
  digitalWrite(PIN_RELAY2, StateRelay2);
  digitalWrite(PIN_RELAY3, StateRelay3);
  digitalWrite(PIN_RELAY4, StateRelay4);
  
// Задаем прерывания  
/*  Шпаргал по прерываниям http://arduino.ru/Reference/AttachInterrupt */
  
  // Прикрепляем прерывание Для ESP8266
  // У ESP8266 Прерывания могут быть назначены на любые GPIO кроме GPIO16.
  
  // Два способа:
  // 1-й  настраиваем PIN Кнопки на вход с подтяжкой к плюсу 
  // и тип прерывания FALLING (при смене значения на порту с HIGH на LOW)
      pinMode(PIN_BUTTON1, INPUT_PULLUP);// объявляем выход и подтягиваем его к плюсу
      pinMode(PIN_BUTTON2, INPUT_PULLUP);// объявляем выход и подтягиваем его к плюсу
      pinMode(PIN_BUTTON3, INPUT_PULLUP);// объявляем выход и подтягиваем его к плюсу
      pinMode(PIN_BUTTON4, INPUT_PULLUP);// объявляем выход и подтягиваем его к плюсу
      attachInterrupt(digitalPinToInterrupt(PIN_BUTTON1), buttonInterrupt1, FALLING);
      attachInterrupt(digitalPinToInterrupt(PIN_BUTTON2), buttonInterrupt2, FALLING);
      attachInterrupt(digitalPinToInterrupt(PIN_BUTTON3), buttonInterrupt3, FALLING);
      attachInterrupt(digitalPinToInterrupt(PIN_BUTTON4), buttonInterrupt4, FALLING);
  
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
