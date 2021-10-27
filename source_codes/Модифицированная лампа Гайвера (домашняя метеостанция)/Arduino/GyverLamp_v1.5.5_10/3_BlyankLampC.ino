int BlynkCurrentMode, BlynkAutoChange, BlynkBrightness,  BlynkAutoBr,  BlynkAutoOff,  MyBr;
float F_Br, CurBrightness;
uint8_t zeRGBa[3];         //output: V2, zeRGBa data (set to "merge")
uint16_t speed, scale;

BLYNK_WRITE(V30){  //Power
  if (!param.asInt()) {
    ONflag = false;
    Blynk.virtualWrite(V33,0);
    changePower();
  } else {
    ONflag = true;
    changePower();
  }
}
BLYNK_WRITE(V31){ //Mode
    BlynkCurrentMode=param.asInt();
    if (BlynkCurrentMode >= MODE_AMOUNT) BlynkCurrentMode = 0;
    currentMode=BlynkCurrentMode;
    loadingFlag = true;
    settChanged = true;
    eepromTimer = millis();
    FastLED.clear();
    delay(1);
    Blynk.virtualWrite(V36,modes[BlynkCurrentMode].speed);
    Blynk.virtualWrite(V38,modes[BlynkCurrentMode].scale);
}

BLYNK_WRITE(V32){   //Brightness
  BlynkBrightness=param.asInt(); 
  Blynk.virtualWrite(V44,map(BlynkBrightness,1,255,1,100));
}

BLYNK_WRITE(V33){BlynkAutoChange=param.asInt();} //Auto change
BLYNK_WRITE(V34){  
  zeRGBa[0] = param[0].asInt();  //red
  zeRGBa[1] = param[1].asInt();  //green
  zeRGBa[2] = param[2].asInt();  //blue
}
BLYNK_WRITE(V35){ // Включить ночник
   Blynk.virtualWrite(V31,17);
   Blynk.virtualWrite(V30,param.asInt());
   Blynk.syncVirtual(V31,V34,V30);
}
BLYNK_WRITE(V36){
  modes[BlynkCurrentMode].speed=param.asInt();
  speed = modes[BlynkCurrentMode].speed; // speed is set dynamically once we've started up
} //Настройка эффекта

BLYNK_WRITE(V37){  //Тепература света
  zeRGBa[0] = map(param.asInt(), 2600, 6500, 255, 150);   //red
  zeRGBa[1] = map(param.asInt(), 2600, 6500, 200, 150);   //green
  zeRGBa[2] = map(param.asInt(), 2600, 6500, 100, 255);   //blue
}
BLYNK_WRITE(V38){  //Настройка эффекта
  modes[BlynkCurrentMode].scale=param.asInt();
  scale = modes[BlynkCurrentMode].scale; // scale is set dynamically once we've started up
}

BLYNK_WRITE(V39){ //Автояроскость эффекта
  BlynkAutoBr=param.asInt();
  F_Br=BlynkBrightness; // Начальная освещенность
  if (BlynkAutoOff==1 && BlynkAutoBr==1) { //Если автоотключение яроскость, включено, то его нужно отключить!
    Blynk.virtualWrite(V40,0);
    Blynk.syncVirtual(V40);
    Blynk.setProperty(V32, "color", "#555555");
  }else{
    Blynk.setProperty(V32, "color", "#23C48E"); 
  }
}

BLYNK_WRITE(V40){
  BlynkAutoOff=param.asInt();
  if (BlynkAutoOff==1 && BlynkAutoBr==1) { //Если автоотключение яроскость, включено, то его нужно отключить!
    Blynk.virtualWrite(V39,0);
    Blynk.syncVirtual(V39);
  }
}  //Автоотключение ночника

BLYNK_WRITE(V44){ // Яроксть с Яндекса
  BlynkBrightness=map(param.asInt(),0,100,1,255);
  Blynk.virtualWrite(V32,BlynkBrightness);
}

void lamp_mode_ch() { //Вызываеться раз в 30 сек. (см. таймер SendAirProperties)
  if (BlynkAutoChange) {
    Blynk.virtualWrite(V31,BlynkCurrentMode+1);
    Blynk.syncVirtual(V31);
  }
  if (BlynkAutoOff==1 && ONflag) {
    if (analogRead(A0)>40) { //Значение яроксоти автоотключения
      Blynk.virtualWrite(V30,0);
      Blynk.syncVirtual(V30);
    }
  }
  FanCheck();
}
void FanCheck() {
  if (ONflag) {
    if (BlynkBrightness>30) digitalWrite(12,1); else digitalWrite(12,0); // Включить вентилятор 
  }else{
    digitalWrite(12,0); // Выключить вентилятор 
  }
}

void lamp_AutoBr() { //Вызываеться раз в 0.1 сек. (см. таймер в setup)
  float FILTER_COEF=0.01; // настройка плавности автояркости
  float FILTER_COEF2=0.05; // настройка плавности смены яркости
  if (ONflag) {
    if (BlynkAutoBr==1) {
      F_Br = analogRead(A0) * FILTER_COEF + F_Br * (1 - FILTER_COEF);
      BlynkBrightness=constrain(map(F_Br,2,300,8,255),8,255);  //Настройка яроксоти в зависимости от освещенности
      CurBrightness=BlynkBrightness;
      FastLED.setBrightness(CurBrightness);
      if (abs(BlynkBrightness-MyBr)>4) { // Шаг обновления ползунка
        MyBr=BlynkBrightness;
        Blynk.virtualWrite(V32,MyBr);
      }
    }else{
      if (abs(BlynkBrightness*1.0-CurBrightness)>0.5) {
        CurBrightness = BlynkBrightness * FILTER_COEF2 + CurBrightness * (1 - FILTER_COEF2);
        FastLED.setBrightness(CurBrightness);
      }
    }
  }
}
