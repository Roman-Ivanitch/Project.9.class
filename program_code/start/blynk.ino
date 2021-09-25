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
  brightnes = param.asInt();
}
BLYNK_WRITE(V6) {                // Яркость света
  brightnes1 = param.asInt();
}
BLYNK_WRITE(V7) {               // Режим зебра
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
}
