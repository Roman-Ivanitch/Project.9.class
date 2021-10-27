bool isFirstConnect=true, Alt_temp_mode;
int Auto_temp, Need_main_temp, Delta_temp, time_alt_temp_start, time_alt_temp_stop, currentTime, Need_alt_temp, Auto_humi, Need_main_humi, Delta_humi, TestMode, Auto_CO2, Max_CO2;
float MyH, MyP, MyT, MyCO2, Need_temp;
float OutH, OutP, OutT, OutInDelta=10;

void MultyWiFiBlynkBegin() {
  int ssid_count=0;
  int ssid_mas_size = sizeof(ssid) / sizeof(ssid[0]);
  do {
    DEBUG("Trying to connect to wi-fi " + String(ssid[ssid_count]));
    WiFi.begin(ssid[ssid_count], pass[ssid_count]);   
    int WiFi_timeout_count=0;
    while (WiFi.status() != WL_CONNECTED && WiFi_timeout_count<20) { //waiting 5 sec (Если делать больше 5 сек. и нет вайфая он вываливаеться с ошибкоой!)
      delay(500);
      Serial.print(".");
      ++WiFi_timeout_count;
    }
    if (WiFi.status() == WL_CONNECTED) {
      DEBUG("Connected to WiFi! Now I will check the connection to the Blynk server");
      Blynk.config(auth);
      Blynk.connect(5000); //waiting 5 sec
    }
    if (ssid_count<ssid_mas_size) {DEBUG("Next SSID");}
    ++ssid_count;
  }
  while (!Blynk.connected() && ssid_count<ssid_mas_size);
  if (!Blynk.connected() && ssid_count==ssid_mas_size) {
    DEBUG("I could not connect to blynk =( Ignore and move on. but still I will try to connect to wi-fi " + String(ssid[ssid_count-1]));
  }
}

BLYNK_CONNECTED() {
  if (isFirstConnect) {} //При первом запуске
  isFirstConnect = false;
  rtc.begin(); // Synchronize time on connection
  MyMsgBox("BLYNK_CONNECTED syncVirtual ports");
  Blynk.syncVirtual(V0, V1, V2, V4, V5, V6, V7, V9, V12, V14, V15);
  Blynk.syncVirtual(V23, V25, V27, V28);
  Blynk.syncVirtual(V31, V32, V33, V34, V39, V40, V30);
  SmartVent.setAuthToken(""); //Токены для мостов! У вас они должны быть свои!!!
  MySmartHumidifier.setAuthToken(""); //Токены для мостов! У вас они должны быть свои!!!
}

BLYNK_WRITE(V0){Auto_temp=param.asInt();
  if (Auto_temp==1) {tempblok(true);} else {tempblok(false);}
}
BLYNK_WRITE(V1){Need_main_temp=param.asInt();}
BLYNK_WRITE(V2){Delta_temp=param.asInt();} //V3-temperature
BLYNK_WRITE(V4){TimeInputParam time_alt_temp(param);
  time_alt_temp_start=int(time_alt_temp.getStartHour())*60 + int(time_alt_temp.getStartMinute());
  time_alt_temp_stop=int(time_alt_temp.getStopHour())*60 + int(time_alt_temp.getStopMinute());
  MyMsgBox("Set time start: " + String(time_alt_temp_start) + ",   time stop: " + String(time_alt_temp_stop));
}
BLYNK_WRITE(V5){Need_alt_temp=param.asInt();}
BLYNK_WRITE(V6){Auto_humi=param.asInt();
  if (Auto_humi==1) {humiblok(true);} else {humiblok(false);}
}
BLYNK_WRITE(V7){Need_main_humi=param.asInt();} //V8-Humidity
BLYNK_WRITE(V9){Delta_humi=param.asInt();} //V10-Pressure, V11 - terminal
BLYNK_WRITE(V12){TestMode=param.asInt();} //V12 - test mode; V13 - PPM CO2
BLYNK_WRITE(V14){Auto_CO2=param.asInt();}
BLYNK_WRITE(V15){Max_CO2=param.asInt();} 

BLYNK_WRITE(V41) {OutT=param.asInt();} //Получение даных от клапана спальне (температура на улице)
BLYNK_WRITE(V42) {OutH=param.asInt();} 
BLYNK_WRITE(V43) {OutP=param.asInt();}

void main_check() { //Уже нужно учитывать приоритеты между командами
  if (Auto_CO2==1) {
    MyMsgBox("Автоматический котроль CO2 включен. СО2:" + String(MyCO2) + "ppm СО2_макс:" + String(Max_CO2) + "ppm");
    if (MyCO2>Max_CO2) {SmartVent.virtualWrite(V0, 5); MyMsgBox("Включить приточную вентилцию");} //Если уровень СО2 выше допустимого, то открываем клапан и вкл. приточку
    if (MyCO2<Max_CO2-100.0) {SmartVent.virtualWrite(V0, 4); MyMsgBox("Отключить приточную вентилцию");} //Если уровень СО2 ниже допустимого на 100ppm, то отключаем приточку
  }
  
  if (Auto_temp==1) {
    MyMsgBox("Автоматический котроль тепературы включен");
    currentTime = hour()*60 + minute();
    if (time_alt_temp_start<time_alt_temp_stop) {
      if (time_alt_temp_start<=currentTime && time_alt_temp_stop>currentTime) {
        Alt_temp_mode=true; MyMsgBox("В депазоне");} else {Alt_temp_mode=false; MyMsgBox("Вне алт. временного депазона");
      }
    }else{
      if ((time_alt_temp_start<=currentTime && 1439>=currentTime) || (time_alt_temp_stop>=currentTime && 0<=currentTime)) {
        Alt_temp_mode=true; MyMsgBox("В депазоне");} else {Alt_temp_mode=false; MyMsgBox("Вне алт. временного депазона");
      }
    }
    if (Alt_temp_mode) {Need_temp=Need_alt_temp;} else {Need_temp=Need_main_temp;}
    //Теперь когда мы обпрелделились с тепературой по временному диопазону, теперь корректируем по влажности
    Need_temp=Need_temp-(MyH*2)/100;
    Blynk.virtualWrite(V16, Need_temp);
    MyMsgBox("Need_temp " + String(Need_temp));
    if (MyT>Need_temp+Delta_temp*0.1) {Temp_Down();}
    if (MyT<Need_temp-Delta_temp*0.1) {Temp_Up();}
    if (MyT>Need_temp-Delta_temp*0.1 && MyT<Need_temp+Delta_temp*0.1) {Temp_Hold();}
  }
  
  if (Auto_humi==1) {
    MyMsgBox("Автоматический котроль влажности включен");
    if (MyH>Need_main_humi+Delta_humi*0.1) {Humi_Down();}
    if (MyH<Need_main_humi-Delta_humi*0.1) {Humi_Up();}
  }  
}

void Temp_Down() {
  MyMsgBox("Начать уменьшение температруы");
  if (OutT+OutInDelta<MyT){SmartVent.virtualWrite(V0, 4);  MyMsgBox("открыть клапан");}//Если темп. на улице ниже чем в квартире на 10 гр. и в команате жарко то открываем клапан
}

void Temp_Up() {
  MyMsgBox("Начать увелечение температруы");
  if (OutT+OutInDelta<MyT){SmartVent.virtualWrite(V0, 0);  MyMsgBox("закрыть клапан");}//Если темп. на улице ниже чем в квартире на 10 гр. и в команате холодно то прикрываем клапан
}

void Temp_Hold() {
  MyMsgBox("Удерживание температруы");
  if (OutT+OutInDelta<MyT){SmartVent.virtualWrite(V0, 2);  MyMsgBox("ставим клапан в среднее положение");}//Если темп. на улице ниже чем в квартире на 10 гр. и в команате номарльно то ставим клапан в среднее положение
}

void Humi_Down() {
  MyMsgBox("Начать увелечение влажности");
  MySmartHumidifier.virtualWrite(V0, 0);
}

void Humi_Up() {
  MyMsgBox("Начать увелечение влажности");
  MySmartHumidifier.virtualWrite(V0, 1);
}

void SendAirProperties() {
  //BME280 (Temperature, Pressure, Humidity)
  MyT=bme.readTemperature()+0.7;
  DEBUG("Temperature = " + String(MyT) + " *C");
  MyP=bme.readPressure() / 100.0F *0.750062;
  DEBUG("Pressure = " + String(MyP) + " mmHg");
  MyH=bme.readHumidity();
  DEBUG("Humidity = " + String(MyH) +" %");
  DEBUG();
  Blynk.virtualWrite(V3,MyT); //LED Widget status
  Blynk.virtualWrite(V8,MyH); //LED Widget status
  Blynk.virtualWrite(V10,MyP); //LED Widget status

  //MHZ-Z19 (PPM CO2) 
  #define mySerial Serial // Serial битрейд обязтельно 9600!!!
  static byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; //команда чтения
  byte response[9];
  byte crc = 0;
  while (mySerial.available())mySerial.read();//очистка буфера UART перед запросом 
  memset(response, 0, 9);// очистка ответа
  mySerial.write(cmd,9);// запрос на содержание CO2
  mySerial.readBytes(response, 9);//читаем 9 байт ответа сенсора
  //расчет контрольной суммы
  crc = 0;
  for (int i = 1; i <= 7; i++)  {crc += response[i];}
  crc = ((~crc)+1);
  {
  //проверка CRC
    if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) )  {
      DEBUG("CRC error");
    }else{
       //расчет значеия CO2
       MyCO2 = (((unsigned int) response[2])<<8) + response[3];
       DEBUG("CO2: " + String(MyCO2) + "ppm");
       Blynk.virtualWrite(V13,MyCO2); //LED Widget status
    }
  }
  main_check();
  lamp_mode_ch();
}

void tempblok(boolean myval) {
  if (myval) {
    Blynk.setProperty(V1, "color", "#23C48E");
    Blynk.setProperty(V2, "color", "#23C48E");
    Blynk.setProperty(V4, "color", "#23C48E");
    Blynk.setProperty(V5, "color", "#23C48E");
  }else{
    Blynk.setProperty(V1, "color", "#555555");
    Blynk.setProperty(V2, "color", "#555555");
    Blynk.setProperty(V4, "color", "#555555");
    Blynk.setProperty(V5, "color", "#555555");
  }
}

void humiblok(boolean myval){
  if (myval) {
    Blynk.setProperty(V7, "color", "#04C0F8");
    Blynk.setProperty(V9, "color", "#04C0F8");
  }else{
    Blynk.setProperty(V7, "color", "#555555");
    Blynk.setProperty(V9, "color", "#555555");
  }
}

#define FILTER_STEP 500 //Ограничение по выводу данных по времени
unsigned long filter_timer;

void MyMsgBox(String my_text){
  if (TestMode==1) {
    if (millis() - filter_timer > FILTER_STEP) {
      filter_timer = millis();
      String CT = DigToStrF(hour()) + ":" + DigToStrF(minute()) + ":" + DigToStrF(second()) + "   " ;
      Blynk.virtualWrite(V11, CT + my_text + "\n");
      DEBUG(my_text);
    }
  }
}

String DigToStrF(int A) {
  String s="";
  if (A<10) s="0";
  return (s+A);
}
