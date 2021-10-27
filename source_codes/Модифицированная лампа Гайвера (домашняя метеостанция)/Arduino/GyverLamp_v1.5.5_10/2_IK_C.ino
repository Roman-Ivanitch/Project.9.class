uint16_t AC_Swing[199] = {4346, 4412,  504, 1640,  530, 540,  504, 1642,  502, 1640,  504, 568,  504, 568,  528, 1616,  528, 568,  502, 570,  478, 1638,  504, 568,  528, 542,  502, 1640,  502, 1666,  502, 544,  530, 1614,  504, 592,  504, 1616,  502, 1664,  506, 566,  478, 1640,  504, 592,  502, 1640,  478, 1640,  504, 1640,  502, 594,  478, 568,  502, 1640,  504, 592,  478, 1640,  504, 592,  506, 564,  478, 1638,  504, 1638,  504, 1640,  504, 568,  528, 568,  502, 568,  478, 568,  502, 568,  504, 592,  504, 540,  504, 568,  526, 1616,  504, 1640,  504, 1642,  502, 1640,  502, 1642,  526, 5198,  4370, 4388,  504, 1640,  528, 546,  500, 1642,  502, 1638,  504, 568,  502, 570,  500, 1640,  504, 568,  528, 540,  504, 1640,  530, 542,  530, 542,  502, 1664,  478, 1642,  502, 568,  528, 1640,  480, 592,  478, 1640,  502, 1640,  504, 566,  528, 1616,  502, 592,  478, 1642,  502, 1664,  480, 1664,  480, 592,  478, 592,  478, 1642,  502, 570,  502, 1638,  502, 570,  528, 542,  528, 1614,  504, 1640,  528, 1614,  504, 592,  504, 568,  478, 592,  478, 570,  502, 592,  480, 566,  530, 542,  528, 542,  502, 1642,  504, 1638,  504, 1666,  478, 1666,  504, 1616,  502};  // COOLIX B26BE0 - Swing: Toggle
uint16_t AC_Swing_Dir[99] = {4294, 4468,  472, 1696,  448, 622,  450, 1668,  502, 1666,  474, 598,  448, 596,  474, 1664,  480, 622,  448, 596,  474, 1692,  452, 620,  450, 646,  424, 1694,  474, 1670,  448, 622,  450, 1720,  448, 572,  474, 622,  450, 594,  500, 596,  450, 1668,  476, 1666,  476, 1692,  478, 1668,  450, 1694,  448, 1670,  500, 1618,  498, 1694,  450, 598,  500, 596,  450, 620,  452, 620,  450, 1694,  476, 1642,  476, 1666,  476, 620,  476, 598,  448, 596,  500, 596,  450, 622,  448, 650,  448, 622,  424, 594,  502, 1642,  476, 1720,  448, 1644,  474, 1694,  450, 1694,  448};  // UNKNOWN B0473CC8

//Работа с кондиционером после проверки кода АС Сontrol
int myACPower, myACTemp, myACFanSpeed, myACMode;

BLYNK_WRITE(V23){ //AC Power
  myACPower=param.asInt();
  SendAC();
}

BLYNK_WRITE(V25){ //AC Temp.
 myACTemp=param.asInt();
 SendAC();
}

BLYNK_WRITE(V28){ //AC_Mode{
  String AC_Mode = param.asStr();
  if (AC_Mode=="auto") myACMode=0;
  if (AC_Mode=="cool") myACMode=1;
  if (AC_Mode=="dry") myACMode=2;
  if (AC_Mode=="heat") myACMode=3;
  if (AC_Mode=="fan_only") myACMode=4;
  SendAC();
}

BLYNK_WRITE(V29){ //Fan speed
  String Fan_speed = param.asStr();
  if (Fan_speed=="auto") myACFanSpeed=0;
  if (Fan_speed=="low") myACFanSpeed=1;
  if (Fan_speed=="medium") myACFanSpeed=2;
  if (Fan_speed=="high") myACFanSpeed=3;
  SendAC();
}

void SendAC() {
  MyMsgBox("myACMode: " + String(myACMode) +" myACFanSpeed:" + String(myACFanSpeed));
  if (myACPower) {
    ac.on();
    switch (myACMode) {
      case 0: ac.setMode(2); break;
      case 1: ac.setMode(0); ; break;
      case 2: ac.setMode(1); break;
      case 3: ac.setMode(3); break;
      case 4: ac.setMode(4); break;
    }
    ac.setTemp(myACTemp);
    switch (myACFanSpeed) {
    case 0: ac.setFan(5); break;
    case 1: ac.setFan(4); break;
    case 2: ac.setFan(2); break;
    case 3: ac.setFan(1); break;
    }
  }else{
    ac.off();
  }
  ac.send();
}

BLYNK_WRITE(V27){ //Fan rotation
  if (param.asInt()) {
    irsend.sendRaw(AC_Swing, 199, 38);
  } else {
    irsend.sendRaw(AC_Swing_Dir, 99, 38);
  }
}
