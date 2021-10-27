uint32_t effTimer;

void effectsTick() {
  if (!dawnFlag) {
    if (ONflag && millis() - effTimer >= ((currentMode < 5 || currentMode > 13) ? modes[currentMode].speed : 50) ) {
      effTimer = millis();
      switch (currentMode) {
        case 0: sparklesRoutine();
          break;
        case 1: fireRoutine();
          break;
        case 2: rainbowVertical();
          break;
        case 3: rainbowHorizontal();
          break;
        case 4: colorsRoutine();
          break;
        case 5: madnessNoise();
          break;
        case 6: cloudNoise();
          break;
        case 7: lavaNoise();
          break;
        case 8: plasmaNoise();
          break;
        case 9: rainbowNoise();
          break;
        case 10: rainbowStripeNoise();
          break;
        case 11: zebraNoise();
          break;
        case 12: forestNoise();
          break;
        case 13: oceanNoise();
          break;
        case 14: attractRoutine();
          break;
        case 15: snowRoutine();
          break;
        case 16: matrixRoutine();
          break;
        case 17: night_light();
          break;
      }
      FastLED.show();
    }
  }
}

void changePower() {
  if (ONflag) {
    effectsTick();
    for (int i = 0; i < BlynkBrightness; i += 1) {
      if (i > 20) i += 10;
      if (i>255) i=255;
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    delay(2);
    FastLED.show();
  } else {
    effectsTick();
    for (int i = BlynkBrightness; i > 1; i -= 1) {
      if (i > 20) i -= 10;
      if (i<1) i=1;
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.clear();
    delay(2);
    FastLED.show();
  }
}
