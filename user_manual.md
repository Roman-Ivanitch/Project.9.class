# Руководство по эксплуатации 
1. Скачать приложение [Blynk](https://play.google.com/store/apps/details?id=cc.blynk)

![blynk_pleymarket](/images/blynk_pleymarket.jpg)

1. Зарегистрироваться/войти на сервер (blync-cloud.com или свой локальный)

![blynk_login](/images/blynk_login.jpg)

1. Создать устройство esp и тип подключения WIfi

![blynk_adddevice](/images/blynk_adddevice.jpg)

1. Получить токен устройства на почту указуную при регистрации

![токен](/images/blynk_token.jpg)

1. Скачать и установить [arduino IDE](https://www.arduino.cc/en/software), [гайд](https://alexgyver.ru/arduino-first/) ![]()

2. Скачать репозиторий с прошивкой с [github](https://github.com/Roman-Ivanitch/Project.9.class)

![Qr-code](/images/QR-code.png)

1. Открыть файл, и указать имя и пароль Wifi и токен который получили на почту

```C++
char ssid_home[] = "wi-fi-home-0000575";  // сеть wi-fi
char pass_home[] = "7851xdwifi";          // Пароль WiFi

char ssid_phone[] = "A30";                // Мобильная точка доступа WiFi
char pass_phone[] = "87651234";           // Пароль WiFi

```

```C++
char auth[] = "токен";          // лампа Blynk-cloud.com // Токен приложения Blynk
char auth_server[] = "токен";   // лампа local-server    // Токен приложения Blynk
```
Так же если используем свой сервер, то надо и задать его ip адрес, в моём случае, для доступа из сети интернет я подключил статический ip адрес (для оф.сервера задавать ip не нужно)
```C++
char ip_local[] = "ip адрес";             // свой server - local ip   // к примеру 192.168.88.200
char ip_net[] = "ip адрес";               // свой server - internet ip
```

8. Подключить плату к ПК чере TTL переходник(прагроматор) ![]()

9. Загружаем прошивку ![]()

10. В приложении настроить виджеты и виртуальные пины

![blynk_wigets](/images/blynk_wigets.jpg)

1.  Тумблером на устройстве выбрать сеть wifi настроеные в прошивке ![]()

2.  Включить устьройство кнопкой взади

Готово!
