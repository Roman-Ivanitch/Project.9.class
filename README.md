# Project.9.class 
![logo](/images/IoT.jpg)
___
## Проект 9 класс Рабочая лампа.
## Проект 10 класс Умная лампа V2.
Красикова Романа RassirHakerCorporation@mail.ru 
+ [Страница проекта эектронной начинки в EasyEDA](https://easyeda.com/RassirHaker/10-class)
+ [Сборка лампы](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/dimensional_drawing/README.md)
+ [Настройки в прошивке](https://github.com/Roman-Ivanitch/Project.9.class/tree/main/program_code#readme) 
+ [Руководство по настройке](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/user_manual.md)
+ [Источники информации](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/sources_of_information/README.md)
+ [Исходники кода](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/source_codes/README.md)
## :bulb: Что это такое
Лампа, крепящаяся на стенку над столом. На лампе 2 ряда светодиодной 12-24 вт. Ленты и ряд адресной светодиодной ленты (60 LED/м) отрезками 730 мм. 
Управляется через телефон приложением Blynk (не новым Blynk IoT). Лента уложена в продольные канавки (пазы), 
тем самым не ослепляет и освещает нужную рабочую зону. Изготовлена из "Планкена" 90мм. Питается от внешнего блока питания 12в 3а.

В папке elecktronika находиться гербер файл для заказа плат (я заказывал на [JLCPCB](https://jlcpcb.com/)). 
На плате находиться модуль(микроконтроллер) ESP12f(даташит в папке материалы, источники),
2 mosfet ключа, реле 5V, понижающий регулируемый(калибровочный)) DC-DC преобразователь, стабилизаторы напряжения  3,3 и 12 вольт.

Плату можно питать в диапазоне 10-18 вольт. на плате лишний стабилизатор напряжения на 12 вольт, если его убрать то рекомендую питать от БП 12В, если оставить, то от 14 вольт, но лучше наверно будет поставить вместо стабилизатора стабилитрон. в течении последующего года это страница будет обновляться, скорее всего разведу новую более совершенную универсальную плату. Актуальную плату лучше смотреть на [EasyEDA](https://easyeda.com/RassirHaker/10-class). 

Более подробное описание: 
+ [Проект_9_класс.docx](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/%D0%9F%D1%80%D0%BE%D0%B5%D0%BA%D1%82_9_%D0%BA%D0%BB%D0%B0%D1%81%D1%81.docx) проект на олимпиаду по технологии 9 класс
+ [Проект_10_класс.docx](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/%D0%9F%D1%80%D0%BE%D0%B5%D0%BA%D1%82_10_%D0%BA%D0%BB%D0%B0%D1%81%D1%81.docx) проект на олимпиаду по технологии 10 класс
![3d](https://github.com/Roman-Ivanitch/Project.9.class/blob/main/images/3D_model.png)
## QR-code страницы проекта на github 
![Qr-code](/images/QR-code.png)
___

Репозиторий не переименован, потому что на печатных пплатах я указал эту ссылку. Если переименую ссылка по url будет не доступна, а платы уже изготовлены. 

Первый опыт работы в гите...
