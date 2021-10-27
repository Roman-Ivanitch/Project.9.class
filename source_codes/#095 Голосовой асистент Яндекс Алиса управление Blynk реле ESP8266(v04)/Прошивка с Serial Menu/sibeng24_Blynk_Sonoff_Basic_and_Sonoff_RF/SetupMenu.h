
String SecurityCode="qwerty"; // Код для входа в настройки

int menuState = 0;

// Страница авторизации
void setupMenu(){
    menuState = 0;
    Serial.println (F("*******************************************"));
    Serial.println (F("\t From the Engineering room"));
    Serial.println (F("\t https://www.youtube.com/c/Инженерка"));
    Serial.println (F("*******************************************"));
    Serial.println (F(""));
    Serial.println (F("\t SETUP"));
    Serial.println (F(""));
    Serial.println (F("* CODE - Enter security code to enter the settings menu"));
    Serial.println (F("")); 
    Serial.println (F("* Start - Start Reconnect"));
    Serial.println (F("* Stop  - Stop Reconnect"));
    Serial.println (F(""));    
    Serial.println (F("* CleanEEPROM - Clean UP EEPROM"));
    Serial.println (F("*******************************************"));
}

// Главное меню
void topMenu() {
    menuState = 1;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("\t SETTINGS"));
    Serial.println (F(""));
    Serial.println (F(""));
    
    Serial.print (F("1) SSID:\t\t")); Serial.println (WIFI_SSID); // GO to SSIDMenu()
    Serial.print (F("2) PASS:\t\t")); Serial.println (WIFI_PASS); // GO to PASSMenu()
    Serial.print (F("3) Blynk Auth Code:\t")); Serial.println (AUTH); // GO to AUTHMenu() 

// GO to SERVERMenu()    
    if (defaultServerConnect){
      Serial.print (F("4) Default Connect to Blynk SERVER:\t"));
      Serial.print(defaultSERVER);Serial.print(F(":"));Serial.println(defaultPORT);
    }else{
      Serial.print (F("4) Custom Connect to Blynk SERVER:\t"));
      if(ConnectHost){
        Serial.print(customSERVER);Serial.print(F(":"));Serial.println(customPORT);
      }else{
        Serial.print(customSERVERIP);Serial.print(F(":"));Serial.println(customPORT);
      }
    }
      
    Serial.println (F("* C - Cansel"));
}

// Ввод логина от Wifi
void SSIDMenu() {
    menuState = 2;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("* Enter New SSID"));
    Serial.println (F("* C - Cansel"));    
}

// Ввод пароля от Wifi
void PASSMenu() {
    menuState = 3;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("* Enter New Password"));
    Serial.println (F("* C - Cansel"));    
}

// Ввод ключа авторизации Blynk
void AUTHMenu() {
    menuState = 4;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("* Enter New Blynk Auth Code"));
    Serial.println (F("* C - Cansel"));    
}

// Меню настроек сервера
void SERVERMenu() {
    menuState = 5;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    if (defaultServerConnect){
      Serial.println (F("1) Default Connect"));
      Serial.print   (F("   SERVER:\t")); Serial.println(defaultSERVER);
      Serial.print   (F("   PORT:\t"));   Serial.println(defaultPORT);
    }else{
      Serial.println (F("1) Custom Connect"));
      if(ConnectHost){
      Serial.println (F("2) HOST Connect"));  
      Serial.print   (F("3) SERVER:\t")); Serial.println(customSERVER); // GO menuState = 6;
      Serial.print   (F("4) PORT:\t"));   Serial.println(customPORT); // GO menuState = 7;
      }else{
      Serial.println (F("2) IP Connect"));  
      Serial.print   (F("3) SERVER:\t")); Serial.println(customSERVERIP); // GO menuState = 6;
      Serial.print   (F("4) PORT:\t"));   Serial.println(customPORT); // GO menuState = 7;        
      }
    }
    Serial.println (F("* C - Cansel"));
}

// Ввод сервера
void customServerMenu() {
    menuState = 6;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("* Enter New SERVER"));
    Serial.println (F("* C - Cansel"));
}

// Ввод Порта
void customPortMenu() {
    menuState = 7;
    Serial.println (F(""));
    Serial.println (F("*******************************************"));
    Serial.println (F("* Enter New PORT"));
    Serial.println (F("* C - Cansel"));
}




///////////////////////////////////////////////////////////////
//        Реакция на полученные команды через Serial         //
///////////////////////////////////////////////////////////////
void SETUPmenu(){
  char b;
  String data;
  while (Serial.available()) {  //сохраняем входные данные в строку data
    b = Serial.read();
    data += char(b);
    delay(50);
    }
    
  if (data!=""){ // Проверяем строку data на содержание данных (не пустая ли она сейчас)
    
    //Serial.println(data);
    //int Lengh  = data.length();
    //Serial.print("Lengh="); Serial.println(Lengh);
    data.trim(); // Удаляем лишнии знаки (так называемые пробелы)
    //Lengh  = data.length();
    //Serial.print("Lengh of data.trim()="); Serial.println(Lengh);

// setupMenu
  if (menuState == 0) {
        if (data == "setup" || data == "stop") {       
          ONReconnect=false;
          WiFi.disconnect(); // отключаемся от сети
          Serial.println(F("Stop Reconnect. Disconnect WiFi."));
          data="";
          setupMenu();
        }
        else if (data == SecurityCode) {    
          Serial.println (F("OK"));
          data="";
          topMenu();
        }
        else if (data == "Start") {    
          Serial.println(F("Start Reconnect. Connect WiFi."));
          ONReconnect=true;
          reconnectBlynk();
          data=""; 
        }
        else if (data == "CleanEEPROM") {    
          Serial.println(F("Clean up EEPROM ... Done!!!"));
          Recorded = false;
          EEPROM.begin(addr);
          EEPROM_writeAnything(a[a_Recorded], Recorded);
          EEPROM.end();
          data="";
          setupMenu();
        }        
  }

// topMenu   
  if (menuState == 1) {
        if (data == "C"){data="";
          setupMenu();     
        }  
        else if (data == "1") {data="";  
          SSIDMenu();
        }
        else if (data == "2") {data="";    
          PASSMenu();
        }
        else if (data == "3") {data="";       
          AUTHMenu();
        }
        else if (data == "4") {data="";       
          SERVERMenu();
        }        
        
  }

// SSIDmenu
  if (menuState == 2) {
      if (data == "C"){
        topMenu();      
      }
      else if (data.length()>=6){
        data.toCharArray(WIFI_SSID, 32);
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_WIFI_SSID], WIFI_SSID);
        EEPROM.end();
        data="";
        topMenu();
      }
    
  } 

// PASSmenu
  if (menuState == 3) {
      if (data == "C"){
        topMenu();      
      }
      else if (data.length()>=6){
        data.toCharArray(WIFI_PASS, 32);
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_WIFI_PASS], WIFI_PASS);
        EEPROM.end();
        data="";
        topMenu();
      }
    
  } 

// AUTHmenu
  if (menuState == 4) {
      if (data == "C"){
        topMenu();      
      }
      else if (data.length()==32){
        data.toCharArray(AUTH, 33);
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_AUTH], AUTH);
        EEPROM.end();
        data="";
        topMenu();
      }
    
  } 

// SERVERMenu
  if (menuState == 5) {
      if (data == "C"){
        topMenu();      
      }
      else if (data == "1"){
        defaultServerConnect=!defaultServerConnect;
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_defaultServerConnect], defaultServerConnect);
        EEPROM.end();
        data="";
        SERVERMenu();
      }
    if (!defaultServerConnect){
      if (data == "2"){data="";
        ConnectHost=!ConnectHost;
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_ConnectHost], ConnectHost);
        EEPROM.end();
        data="";
        SERVERMenu();
      }      
      else if (data == "3"){data="";
        customServerMenu();
      }
      else if (data == "4"){data="";
        customPortMenu();
      }                    
    }//if (defaultServerConnect)

    
  } 

// customServerMenu
  if (menuState == 6) {
      if (data == "C"){data="";
        SERVERMenu();      
      }
      else if (ConnectHost==true && data.length()>6){
        data.toCharArray(customSERVER, 64);
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_customSERVER], customSERVER);
        EEPROM.end();
        data="";
        SERVERMenu();
      }
      else if (ConnectHost==false && data.length()>7){
        // Прочитать введенный IP
        customSERVERIP = StringReadIP(data);
        EEPROM.begin(addr);
        EEPROM_writeAnything(a[a_customSERVERIP], customSERVERIP);
        EEPROM.end();
        data="";
        SERVERMenu();        
      }    
  } 

// customPortMenu
  if (menuState == 7) {
      if (data == "C"){data="";
        SERVERMenu();      
      }
      else if (data.length()>=1 && data.length()<8){
        if (data.toInt()>0){
          customPORT = data.toInt();
          EEPROM.begin(addr);
          EEPROM_writeAnything(a[a_customPORT], customPORT);
          EEPROM.end();
        }
        data="";
        SERVERMenu();
      }
    
  }
        
  }//if (data!="")
}//SETUPmenu()  
