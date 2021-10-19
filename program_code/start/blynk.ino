BLYNK_CONNECTED(){              // обновление 
  Blynk.syncAll();
}
BLYNK_WRITE(V1) {               // Вкл/выкл света
  light = param.asInt();
}
BLYNK_WRITE(V2) {               // Вкл/выкл подсветки
  lightnes = param.asInt();
}
BLYNK_WRITE(V3) {               // Контраст подсветки
  thissat = param.asInt();
}
BLYNK_WRITE(V4) {               // Режим подсветки
  mode =  param.asInt();
}
BLYNK_WRITE(V5) {               // Яркость подсветки
  int brightnes_web = param.asInt();
  brightnes = map(brightnes_web,0,100,0,255);
}
BLYNK_WRITE(V6) {                // Яркость света
  int brightnes1_web = param.asInt();
  brightnes1 = map(brightnes1_web,0,100,0,1023);  // если у тебя вопрос зачем это, то это для Алисы 
}
BLYNK_WRITE(V7) {               // Режим зебра
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
}
