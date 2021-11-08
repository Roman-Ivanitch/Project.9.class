# Program code 
Код писал в Arduino EDA, как настраивать читайте [тут](https://alexgyver.ru/arduino-first/)

Мои ссылки менеджера библиотек:
> https://dl.espressif.com/dl/package_esp32_index.json

> http://arduino.esp8266.com/stable/package_esp8266com_index.json

Код в папке start потому, что первый файл кода назван start, особенность Arduino EDA.

## Settings 
___
Для работы с блинком надо свой токен, но так как у лампы есть тумблер переключения сети, что бы можно задать несколько серверов и wifi сети, то для каждого сервера и wifi сети надо задать свои настройки. 

Если мы используем несколько серверов то для каждого сервера надо указать свой токен
```C++
char auth[] = "токен";          // лампа Blynk-cloud.com // Токен приложения Blynk
char auth_server[] = "токен";   // лампа local-server    // Токен приложения Blynk
```
Так же если используем свой сервер, то надо и задать его ip адрес, в моём случае, для доступа из сети интернет я подключил статический ip адрес (для оф.сервера задавать ip не нужно)
```C++
char ip_local[] = "ip адрес";             // свой server - local ip   // к примеру 192.168.88.200
char ip_net[] = "ip адрес";               // свой server - internet ip
```
Так же в коде надо задать параметры wifi сети, пароль и ssid
```C++
char ssid_home[] = "wi-fi-home-0000575";  // сеть wi-fi
char pass_home[] = "7851xdwifi";          // Пароль WiFi

char ssid_phone[] = "A30";                // Мобильная точка доступа WiFi
char pass_phone[] = "87651234";           // Пароль WiFi

```
Пины подключения ленты/тумблера/обвязки (пины подключены как на печатной плате)
```C++
#define LED_DT  4               // Пин, куда подключен DIN ленты
#define LED     14              // Пин подключения лампы
#define LED1    12              // Пин подключения реле 

#define NET1 5                  // пин подключения кнопки выбора сети 1              
#define NET2 13                 // пин подключения кнопки выбора сети 2
```
Так же надо указать число светодиодов в подключеной адресной ленте
```C++
#define LED_COUNT 50            // Число светодиодов в кольце/ленте
```
### Вот собственно все настройки! 
___
Единственное правда можно поиграть с назначениями положения тумблера
```C++
  // Запуск Blynk (подключение,токен,WiFi,пароль) в зависимости от кнопки // выбор сети

  if (digitalRead(NET1) == 1) Blynk.begin(auth, ssid_phone, pass_phone); 
  // мобил точка доступа и Blynk-cloud.com

  else if (digitalRead(NET2) == 1) Blynk.begin(auth_server, ssid_phone, pass_phone, ip_net, 8080);
  // мобил точка доступа и свой сервер через интернет

  else Blynk.begin(auth_server, ssid_home, pass_home, ip_local, 8080);
  // точка доступа wifi  и свой локальный сервер

```
## tabs 
___
+ start.ino - настройки, функции setup и loop
+ blynk.ino - назначение виртуальных пинов приложения блинк
+ function.ino - функции обработки и воспроизведения режимов, писал не я, брал у [AlexGyver](https://alexgyver.ru/ws2812b-fx/)
+ leds.ino - эффекты для адресной ленты

## libraries
___
+ [FastLED](https://github.com/FastLED/FastLED) 
+ [blynk](https://github.com/blynkkk/blynk-library/releases/tag/v0.5.1), документацию читайте [тут](http://docs.blynk.cc/)
