void web () {
  // запускаем точку доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAP("WiFi Config");

  // запускаем портал
  portal.attachBuild(build);
  portal.start(WIFI_AP);

  while (true) {
    portal.tick();  // работа портала
    process();      // обработка и сохранение данных 
    light_lamp();   // работа света
    mod ();         // работа режимов подсветки
  }
}
void process () {       // обработка ввода
  if (portal.click()) {
    // управление лампой web-interface
    if (portal.click("btnLight")) {   // вкл выкл света
      light = portal.getCheck("btnLight"); // получаем состояние кнопки
    }
    if (portal.click("btnColor")) {   // вкл выкл подсветки
      lightnes = portal.getCheck("btnColor");
    }
    if (portal.click("brightness")) {    // яркость света
      int brightnes1_web = portal.getInt("brightness"); // получаем данные ползунка
      brightnes1 = map(brightnes1_web,0,100,0,1023);
    }
    if (portal.click("brightnessColor")) {    // яркость подсветки
      int brightnes_web = portal.getInt("brightnessColor");
      brightnes = map(brightnes_web,0,100,0,255);
    }

    // обработка и сохранение учетных данных wifi и Blynk
    if (portal.click("token")) {   // обработка токена Blynk
      portal.copyStr("token", data.token); // копируем себе
    }
    if (portal.click("ssid")) {   // обработка названия wifi
      portal.copyStr("ssid", data.ssid);
    }
    if (portal.click("pass")) {    // обработка пароля wifi
      portal.copyStr("pass", data.pass);
    }
    if (portal.click("server")) {    // обработка сервера
      portal.copyStr("server", data.server);
    }
    if (portal.click("port")) {    // обработка порта сервера
      data.port = portal.getInt("port");
    }

    // обработка данных для второго режжима подключения 
    if (portal.click("token1")) {   // обработка токена Blynk
      portal.copyStr("token1", data.token1);
    }
    if (portal.click("ssid1")) {   // обработка названия wifi
      portal.copyStr("ssid1", data.ssid1);
    }
    if (portal.click("pass1")) {    // обработка пароля wifi
      portal.copyStr("pass1", data.pass1);
    }
    if (portal.click("server1")) {    // обработка сервера
      portal.copyStr("server1", data.server1);
    }
    if (portal.click("port1")) {    // обработка порта сервера
      data.port1 = portal.getInt("port1");
    }

    if (portal.click("save")) {
      EEPROM.put(0, data);              // сохраняем
      EEPROM.commit();                  // записываем    в EEPROM
    }
  }
}


void build() {    // сама web страница
  String s;                     // создать строку
  BUILD_BEGIN(s);               // запустить конструктор
  add.THEME(GP_DARK);           // применить тему
  
  add.FORM_BEGIN("/btn");       // начать форму, передать имя
  add.TITLE("Web-setings");     // заголовок 
  add.LABEL("Light:");          // подпись
  add.SWITCH("btnLight",light); // кнопка
  add.LABEL(" lighting:");
  add.SWITCH("btnColor",lightnes);
  add.BREAK();                  // перенос строки
  add.LABEL("Light: ");
  add.SLIDER("brightness", brightnes1, 0, 100, 1); //ползунок
  add.BREAK();
  add.LABEL("lighting: ");
  add.SLIDER("brightnessColor", brightnes, 0, 100, 1);
  add.BREAK();
  add.SELECT("sel", "режим 1,режим 2,режим 3" );
  add.FORM_END();               // завершить форму



  add.FORM_BEGIN("/login");      
  add.TITLE("Data 1");
  add.TEXT("token", "token", data.token); // ввод текста 
  add.BREAK();              
  add.TEXT("ssid", "ssid", data.ssid); 
  add.BREAK();                   
  add.PASS("pass", "pass", data.pass);
  add.BREAK();
  add.TEXT("server", "server", data.server);
  add.BREAK();
  add.NUMBER("port", "port", data.port);
  add.BREAK();

  add.TITLE("Data 2");
  add.TEXT("token1", "token", data.token1);
  add.BREAK();           
  add.TEXT("ssid1", "ssid", data.ssid1); 
  add.BREAK();                
  add.PASS("pass1", "pass", data.pass1);
  add.BREAK();
  add.TEXT("server1", "server", data.server1);
  add.BREAK();
  add.NUMBER("port1", "port", data.port1);
  add.BREAK();
  add.BUTTON("save","save");
  add.FORM_END();               

  BUILD_END();                  // завершить построение
}

/*
 * EmbUI https://github.com/DmytroKorniienko/EmbUI
Асинхронная, стабильная, с открытым исходным кодом, есть встроенный клиент mqtt, чутка своеобразен в написании кода, с авто построением интерфейса по коду, можно писать кастомные модули, есть обратная связь, есть авторизация при входе
Написанные на нем популярные для своего круга проект лампы https://github.com/DmytroKorniienko/FireLamp_JeeUI

CRMui https://github.com/WonderCRM/CRMui3
Асинхронная, стабильная, с полу открытым исходным кодом, есть баннер не для коммерческого использования, есть подобие pwa и прост в написании кода, с авто построением интерфейса по коду, красивый с темной темой возможно менять цвета фона, нет возможности писать кастомные модули, есть обратная связь, есть авторизация при входе
На его старом интерфейсе написаны часы https://cloud.mail.ru/public/5eHE/dCHUyqrr1/WiFi-CLOCK/

JeeUI https://github.com/jeecrypt/JeeUIFramework
Прародитель EmbUI CRMui
На нем был написан прикольный проектик часов который перешел в CRMui
Все возможности как EmbUI, не полностью доделанная есть кое какие баги, первая версия красивая в плане css, с авто построением интерфейса по коду, нет возможности писать кастомные модули, есть приложение

WiFiManager https://github.com/tzapu/WiFiManager
Нет асинхронности, большей части ее используют для написания своего проекта для упрощения кода, интерфейс нужно писать или изменить предлагаемый, но есть авто построение меню. Похоже на GyverPortal

Если развивается то в правильную сторону
 */
