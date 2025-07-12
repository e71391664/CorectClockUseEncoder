#include "RtcManager.h"
#include "DisplayGyver.h"
#include <GyverOLED.h>

extern GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#include <RTClib.h> // Потрібно для DateTime now;

// Визначаємо DEBUG_MODE для умовної компіляції Serial.print
//#define DEBUG_MODE

// Створюємо глобальний об'єкт класу
RtcManager rtcManager;
DisplayGyver displayManager(rtcManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Ініціалізація Serial-порту для виведення
    while (!Serial) {
        ; // Чекаємо підключення Serial-порту (тільки для деяких плат, напр. Leonardo)
    }

    rtcManager.init();// Ініціалізуємо RTC-модуль через наш клас
    displayManager.init();// Ініціалізація дисплея

    delay(20);  
}

void loop() {
//  DateTime time = rtcManager.now();
//  if (!time.isValid()) return;

  // // --- Відображення тільки якщо дисплей увімкнений ---
   if (displayManager.getDisplayOn()) {
     if (displayManager.isEditing()) {
        displayManager.updateEdit();
      } else {
        displayManager.showTimeAndTemperature();
        if (rtcManager.readConfirm()) { // BTN_CONN
        displayManager.startEdit();
        displayManager.wakeDisplay();
      }
    }
   } else {
     // Якщо дисплей вимкнений, перевіряємо кнопки для пробудження
     if (rtcManager.readConfirm() || rtcManager.readBack()) {
       displayManager.wakeDisplay();
     }
     // Додайте тут перевірку енкодера, якщо є
   }
}
