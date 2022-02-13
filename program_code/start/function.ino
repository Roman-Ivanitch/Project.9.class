void light_lamp () { // свет
  analogWrite(LED, light ? brightnes1 : 0);
}

void mod () {                             // выбор режима свечния
  if (lightnes == 1) {
    switch (mode) {
      case 1: one_color_all(0, 0, 0); LEDS.show(); break;  // Сбросить цвет
      case 2: break;                                       // Пауза, например на режиме смены цветов или что бы зебру не сбить)))
      case 3: rainbow_fade(); break;                       // Плавная смена цветов
      case 4: rainbow_loop(); break;                       // Радуга
      case 5: ems_lightsALL(); break;                      // Вращается половина красных и половина синих
      case 6: ems_lightsSTROBE(); break;                   // Полицейская мигалка

      case 7: one_color_all(brightnes, 0, 0); LEDS.show(); break;             // Красный
      case 8: one_color_all(brightnes, brightnes / 2, 0); LEDS.show(); break; // оранжевый
      case 9: one_color_all(brightnes, brightnes, 0); LEDS.show(); break;     // Жёлтый
      case 10: one_color_all(0, brightnes, 0); LEDS.show(); break;            // Зелёный
      case 11: one_color_all(0, brightnes, brightnes); LEDS.show(); break;    // Берюзовый
      case 12: one_color_all(0, 0, brightnes); LEDS.show(); break;            // Синий
      case 13: one_color_all(brightnes, 0, brightnes); LEDS.show(); break;    // Фиолетовый
      case 14: one_color_all(brightnes, brightnes, brightnes); LEDS.show(); break;// Белый
      
      case 15: Strobe(0xff, 0xff, 0xff, 10, thisdelay, 1000); break;       // Стробоскоп 
      /* изначально скачал исходники у AlexGyver, который тоже изначально брал с каких то сайтов,
      разобрался как работает режим стробоскоп, но не понял зачем в функции изначально первые три 
      переменные указывать в шестнацатиричном виде. это как то оптимизирует его? */
      
      case 16: rgb_propeller();break;// пропеллер
      case 17: new_rainbow_loop();break; // другая радуга
      case 18: one_color_all(red, green, blue); LEDS.show(); break;  // зебра
    }
  } else {
    one_color_all(0, 0, 0);   // если не включен ни один режим, то лента не горит
    LEDS.show();

  }
}
