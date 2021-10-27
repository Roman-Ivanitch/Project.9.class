/////////////////////////////////////////
//               База GPIO             //

  #define PIN_BUTTON1  D1  // PIN к которому подключена кнопка кнопка
  #define PIN_BUTTON2  D2  // PIN к которому подключена кнопка кнопка
  #define PIN_BUTTON3  D3  // PIN к которому подключена кнопка кнопка
  #define PIN_BUTTON4  D4  // PIN к которому подключена кнопка кнопка  
  
  #define PIN_RELAY1   D5 // PIN к которому подключено Реле1.
  #define PIN_RELAY2   D6 // PIN к которому подключено Реле1.
  #define PIN_RELAY3   D7 // PIN к которому подключено Реле1.    
  #define PIN_RELAY4   D8 // PIN к которому подключено Реле1.
  
/////////////////////////////////////////
//      База Виртуальных PIN -ов       //

  #define VPIN_BUTTON1 V1 // VPIN к которому подключен виджет Button
  #define VPIN_BUTTON2 V2 // VPIN к которому подключен виджет Button
  #define VPIN_BUTTON3 V3 // VPIN к которому подключен виджет Button
  #define VPIN_BUTTON4 V4 // VPIN к которому подключен виджет Button

/////////////////////////////////////////
//      База глобальных переменных     //

// Тут все Флаги
  volatile bool StateRelay1 = false;  //объявляем Флаг StateRelay для записи состояния освещения ON or OFF
  volatile bool StateRelay2 = false;  //объявляем Флаг StateRelay для записи состояния освещения ON or OFF
  volatile bool StateRelay3 = false;  //объявляем Флаг StateRelay для записи состояния освещения ON or OFF
  volatile bool StateRelay4 = false;  //объявляем Флаг StateRelay для записи состояния освещения ON or OFF
  
  volatile bool LStateRelay1 = true; //объявляем Флаг LStateRelay для записи последнего состояния освещения ON or OFF
  volatile bool LStateRelay2 = true; //объявляем Флаг LStateRelay для записи последнего состояния освещения ON or OFF
  volatile bool LStateRelay3 = true; //объявляем Флаг LStateRelay для записи последнего состояния освещения ON or OFF
  volatile bool LStateRelay4 = true; //объявляем Флаг LStateRelay для записи последнего состояния освещения ON or OFF
