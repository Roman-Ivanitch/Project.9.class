bool ONReconnect=true;          // Флаг который разрешает переподключение к серверу
bool defaultServerConnect=true; // Флаг который разрешает коннектиться к серверу по умолчанию
bool ConnectHost=true;          // Флаг который разрешает коннектится к серверу по Имени Хоста иначе по IP 

// Сервер по умолчанию
  const char defaultSERVER[64]="blynk-cloud.com"; // Сервер по умолчанию
  const int  defaultPORT=8080;                    // Порт по умолчанию

// Личные данные
  IPAddress customSERVERIP = IPAddress(192, 168, 1, 47); // IP своего сервера пишите
  char customSERVER[64]="AlexBlynk.dlinkddns.com"; // Имя своего хоста (Пример: "AlexBlynk.dlinkddns.com")
  uint16_t customPORT=8080;
  
  // Логин и пароль от Wifi - Wifi Credentials
  char WIFI_SSID[32] = "xxxxxx";  //Имя точки доступа WIFI
  char WIFI_PASS[32] = "xxxxxx"; //пароль точки доступа WIFI
  
  // Авторизационный ключ Blynk - Blynk Auth Code
  char AUTH[33] = "11112222333344445555666677778888";
                   

///////////////////////////////////////////////////////
//          Функции для подключения к Blynk          //

// Преобразование строки в в переменную типа IPAddress
IPAddress StringReadIP(String data){ 
    uint8_t Start = 0;
    uint8_t End   = 0; 
    IPAddress IP;
    
    End = data.indexOf(".", Start); String a = data.substring(Start, End); uint8_t ai = a.toInt(); Start=End+1;
    End = data.indexOf(".", Start); String b = data.substring(Start, End); uint8_t bi = b.toInt(); Start=End+1;
    End = data.indexOf(".", Start); String c = data.substring(Start, End); uint8_t ci = c.toInt(); Start=End+1;
    End = data.indexOf(".", Start); String d = data.substring(Start);      uint8_t di = d.toInt();

    return IP = IPAddress(ai, bi, ci, di);
}


void ConnectBlynk()
{

  //*******************************************************
  // Запускаем WiFi
    if (WiFi.status() != WL_CONNECTED)// Если нет WiFi, то коннектимся
    {
      BLYNK_LOG(WIFI_SSID);
      
      WiFi.begin(WIFI_SSID, WIFI_PASS);

      int8_t count=0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
        Serial.print(WiFi.status());
        count++;
        if (count >= 5){break;Serial.println(F("WiFi not connected"));}
      }
    }
  /*  Дисконект - для справки 
    WiFi.disconnect(); // отключаемся от сети
    Serial.println("Disconnect WiFi.");
  */
  
  //*******************************************************
  // Запускаем Blynk


  if (WiFi.status() == WL_CONNECTED)// Если нет WiFi, то не коннектимся
  {
          // CONNECT TO BLYNK
          if (defaultServerConnect){  // Конектимся к серверу по умолчанию
            Blynk.config(AUTH, defaultSERVER, defaultPORT);
            Blynk.connect();
          }else{                      // Иначе коннектимся к Локальному серверу
            if(ConnectHost){          // Конектимся по имене Хоста
              Blynk.config(AUTH, customSERVER, customPORT);
              Blynk.connect();
            }else{                    // Иначе коннектимся по IP адресу
              Blynk.config(AUTH, customSERVERIP, customPORT);
              Blynk.connect();              
            }
          }
  }

  
  // До бесконечности будем оставаться в цикле while
  // пока не установим связь с сервером
  // while (Blynk.connect() == false) {}
  
}//ConnectBlynk()


// Реконектимся если обрыв связи
void reconnectBlynk() {
  if (!Blynk.connected() && ONReconnect){
    BLYNK_LOG("Disconnected now");
    ConnectBlynk();
    if (Blynk.connected()){
      BLYNK_LOG("Reconnected");      
    } 
    else{
      BLYNK_LOG("Not reconnected");      
    }
  }
}//reconnectBlynk()
