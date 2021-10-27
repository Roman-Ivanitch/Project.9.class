boolean brightDirection;

void buttonTick() {
  touch.tick();
  if (touch.isSingle()) {
    if (dawnFlag) {
      manualOff = true;
      dawnFlag = false;
      loadingFlag = true;
      changePower();
    } else {
      if (ONflag) {
        ONflag = false;
        changePower();
      } else {
        ONflag = true;
        changePower();
      }
      Blynk.virtualWrite(V30,ONflag);
    }
  }

  if (ONflag && touch.isDouble()) {
    if (++currentMode >= MODE_AMOUNT) currentMode = 0;
    loadingFlag = true;
    settChanged = true;
    eepromTimer = millis();
    FastLED.clear();
    delay(1);
    Blynk.virtualWrite(V31,currentMode);
  }
  
  if (ONflag && touch.isTriple()) {
    if (--currentMode < 0) currentMode = MODE_AMOUNT - 1;
    loadingFlag = true;
    settChanged = true;
    eepromTimer = millis();
    FastLED.clear();
    delay(1);
    Blynk.virtualWrite(V31,currentMode);
  }

  // вывод IP на лампу
  if (ONflag && touch.hasClicks()) {
    if (touch.getClicks() == 5) {
      if (!Blynk.connected()) { //Tigran перезагурить если нет соедниенеи с blyank
        ESP.deepSleep(1e6);     //Заснуть (перегрузить) 
      }
      resetString();
      while (!fillString(lampIP, CRGB::Green, true)) {
        delay(1);
        ESP.wdtFeed();   // пнуть собаку
        yield();  // ещё раз пнуть собаку
      }
    }
  }

  if (ONflag && touch.isHolded()) {
    brightDirection = !brightDirection;    
  }
  if (ONflag && touch.isStep()) {
    if (brightDirection) {
      if (BlynkBrightness < 20) BlynkBrightness += 1;
      else if (BlynkBrightness < 255) BlynkBrightness += 5;
      else BlynkBrightness = 255;
    } else {
      if (BlynkBrightness > 20) BlynkBrightness -= 5;
      else if (BlynkBrightness > 1) BlynkBrightness -= 1;
      else BlynkBrightness = 1;
    }
    CurBrightness=BlynkBrightness;
    FastLED.setBrightness(CurBrightness);
    Blynk.virtualWrite(V32,BlynkBrightness);
    settChanged = true;
    eepromTimer = millis();
  }
}
