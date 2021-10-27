/*
 Справка
  addr - адрес байта
  size - Размер данных должен быть в диапазоне от 4 до 4096 байт.
 EEPROM.begin(size);          // Каждый раз объявляем если хотим прочесть или записать для ESP
 EEPROM.write(addr,  val);   // записали переменную val(размером int8_t) по адресу addr 
 EEPROM.read(addr,  val);    // прочитать из адреса addr в переменную val(размером int8_t)

// Внимание Размеры должны совпадать иначе могут быть проблемы
// записали число val(библиотека "EEPROMAnything.h" размер определит сама) по адресу addr
 EEPROM_writeAnything(addr, val);
// прочитать из адреса addr в переменную val(библиотека "EEPROMAnything.h" размер определит сама) 
 EEPROM_readAnything(addr, val);  
  
 EEPROM.commit();        // закончить запись
 EEPROM.end(); тоже производит запись данных,
 а также освобождает оперативную память от данных,
 запись которых произведена.

   Размеры переменных
   8бит = 1 байт
   u - перед int значит что число положительное
   
 int8_t     |  char                |   от -128 до 127               | 1 byte
 uint8_t    |  byte, unsigned char |   от 0 до 255                  | 1 byte
 int16_t    |  int                 |   от -32768 до 32767           | 2 byte
 uint16_t   |  unsigned int, word  |   от 0 до 65535                | 2 byte
 int32_t    |  long                |  от  -2147483648 до 2147483647 | 4 byte
 uint32_t   |  unsigned long       |  от 0 до 4294967295            | 4 byte
 
 и еще бывают int64_t и  uint64_t
*/

#include <EEPROM.h>         
#include "EEPROMAnything.h" // http://playground.arduino.cc/Code/EEPROMWriteAnything
 
  bool Recorded = false;
  int addr = 0;
  int a[9];
  enum n_addr {a_Recorded, a_defaultServerConnect, a_ConnectHost, a_customSERVERIP,
  a_customSERVER, a_customPORT, a_WIFI_SSID, a_WIFI_PASS, a_AUTH};

void setupAddrEEPROM(){
  a[a_Recorded]=addr;               addr+=sizeof(Recorded); 
  a[a_defaultServerConnect]=addr;   addr+=sizeof(defaultServerConnect);
  a[a_ConnectHost]=addr;            addr+=sizeof(ConnectHost);  
  a[a_customSERVERIP]=addr;         addr+=sizeof(customSERVERIP);
  a[a_customSERVER]=addr;           addr+=sizeof(customSERVER);
  a[a_customPORT]=addr;             addr+=sizeof(customPORT);
  a[a_WIFI_SSID]=addr;              addr+=sizeof(WIFI_SSID);
  a[a_WIFI_PASS]=addr;              addr+=sizeof(WIFI_PASS);
  a[a_AUTH]=addr;                   addr+=sizeof(AUTH); 

  // Теперь в переменной addr находится размер данных для чтения или записи
  
  Serial.print(a[n_addr::a_Recorded]);             Serial.print(" Recorded ");             Serial.println(sizeof(Recorded));
  Serial.print(a[n_addr::a_defaultServerConnect]); Serial.print(" defaultServerConnect "); Serial.println(sizeof(defaultServerConnect));
  Serial.print(a[n_addr::a_ConnectHost]);          Serial.print(" ConnectHost ");          Serial.println(sizeof(ConnectHost));  
  Serial.print(a[n_addr::a_customSERVERIP]);       Serial.print(" customSERVERIP ");       Serial.println(sizeof(customSERVERIP));
  Serial.print(a[n_addr::a_customSERVER]);         Serial.print(" customSERVER ");         Serial.println(sizeof(customSERVER));
  Serial.print(a[n_addr::a_customPORT]);           Serial.print(" customPORT ");           Serial.println(sizeof(customPORT));
  Serial.print(a[n_addr::a_WIFI_SSID]);            Serial.print(" WIFI_SSID ");            Serial.println(sizeof(WIFI_SSID));
  Serial.print(a[n_addr::a_WIFI_PASS]);            Serial.print(" WIFI_PASS ");            Serial.println(sizeof(WIFI_PASS));
  Serial.print(a[n_addr::a_AUTH]);                 Serial.print(" AUTH ");                 Serial.println(sizeof(AUTH));
  
  Serial.println(addr);
}  
// Сохраняем в EEPROM базовые настройки Подключения
void EEPROM_READ_CONNECT(){
    
// Считаем из адреса 0 данные в строку Recorded  

  EEPROM.begin(512);
    EEPROM_readAnything(0, Recorded); // Прочитаем из EEPROM состониие переменной Recorded
  

// Если в переменной Recorded ненаходится слова true то запишем в EEPROM базовые настройки
  if (Recorded == false){
    Recorded=true;

    // Записываем базовые настройки в память

      EEPROM_writeAnything(a[a_Recorded], Recorded);
      EEPROM_writeAnything(a[a_defaultServerConnect], defaultServerConnect);
      EEPROM_writeAnything(a[a_ConnectHost], ConnectHost);      
      EEPROM_writeAnything(a[a_customSERVERIP], customSERVERIP);
      EEPROM_writeAnything(a[a_customSERVER], customSERVER);
      EEPROM_writeAnything(a[a_customPORT], customPORT); 
      EEPROM_writeAnything(a[a_WIFI_SSID], WIFI_SSID); 
      EEPROM_writeAnything(a[a_WIFI_PASS], WIFI_PASS); 
      EEPROM_writeAnything(a[a_AUTH], AUTH);

  }else{
    // Иначе считаем данные из памяти

      EEPROM_readAnything(a[a_Recorded], Recorded);
      EEPROM_readAnything(a[a_defaultServerConnect], defaultServerConnect);
      EEPROM_readAnything(a[a_ConnectHost], ConnectHost);
      EEPROM_readAnything(a[a_customSERVERIP], customSERVERIP);
      EEPROM_readAnything(a[a_customSERVER], customSERVER);
      EEPROM_readAnything(a[a_customPORT], customPORT);
      EEPROM_readAnything(a[a_WIFI_SSID], WIFI_SSID);
      EEPROM_readAnything(a[a_WIFI_PASS], WIFI_PASS);
      EEPROM_readAnything(a[a_AUTH], AUTH);
    
  }
  EEPROM.end();
}//EEPROM_READ_CONNECT()
