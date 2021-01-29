BLYNK_WRITE(V1) {               // Вкл/выкл света
  light = param.asInt();
  //Serial.print("light");
  Serial.println (light);
}
BLYNK_WRITE(V2) {               // Вкл/выкл подсветки
  lightnes = param.asInt();
  //Serial.print("lightnes  ");
  //Serial.println (lightnes);
}
BLYNK_WRITE(V3) {               // Контраст подсветки
  thissat = param.asInt();
  //Serial.print("thissat  ");
  //Serial.println (thissat);
}
BLYNK_WRITE(V4) {               // Режим подсветки
  mode =  param.asInt();
  //Serial.println (mode);
}

BLYNK_WRITE(V5) {               // Яркость подсветки
  brightnes = param.asInt();
  //Serial.print("brightnes  ");
  //Serial.println (brightnes);
}
BLYNK_WRITE(V6) {                // Яркость света
  brightnes1 = param.asInt();
  Serial.print("b");
  Serial.println (brightnes1);
}
