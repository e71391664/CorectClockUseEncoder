#include "RtcManager.h"
#include "DisplayGyver.h"
#include <GyverOLED.h>
#include "EncoderManager.h"
#include "ReleManager.h"

extern GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#include <RTClib.h> // Потрібно для DateTime now;

// Визначаємо DEBUG_MODE для умовної компіляції Serial.print
//#define DEBUG_MODE

// Створюємо глобальний об'єкт класу
RtcManager rtcManager;
ReleManager releManager;
DisplayGyver displayManager(rtcManager);
EncoderManager encoderManager (displayManager, releManager);  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Ініціалізація Serial-порту для виведення
    while (!Serial) { ; // Чекаємо підключення Serial-порту (тільки для деяких плат, напр. Leonardo)
    }

    rtcManager.init();// Ініціалізуємо RTC-модуль через наш клас
    displayManager.init();// Ініціалізація дисплея
    encoderManager.init();
    releManager.init();

    delay(20);  
}

void loop() {
  encoderManager.update();
  releManager.update();
    
  ///Обробка натискання кнопки енкодера
  if (encoderManager.buttonPressed()) {
      if (!encoderManager.inEditMode()) {
          encoderManager.enterEditMode();
      } else {
          rtcManager.applyOffset(encoderManager.getOffset());
          encoderManager.exitEditMode();
      }
  }

  ///Відображення на дисплеї та обробка реле
  if (encoderManager.inEditMode()) {
      displayManager.showOffset(encoderManager.getOffset());
  } else if (releManager.isBusy()) {
    displayManager.showStartProcessing();
  } else {
      displayManager.showTimeAndTemperature();
      releManager.blinkOnNormalMode(rtcManager.now());
  }


  // // // --- Відображення тільки якщо дисплей увімкнений ---
  //  if (displayManager.getDisplayOn()) {
  //    if (displayManager.isEditing()) {
  //       displayManager.updateEdit();
  //     } else {
  //       displayManager.showTimeAndTemperature();
       
  //       if (rtcManager.readConfirm()) { // BTN_CONN
  //       displayManager.startEdit();
  //       displayManager.wakeDisplay();
  //     }
  //   }
  // } else {
  //    // Якщо дисплей вимкнений, перевіряємо кнопки для пробудження
  //    if (rtcManager.readConfirm() || rtcManager.readBack()) {
  //      displayManager.wakeDisplay();
  //    }
  //    //displayManager.autoPowerOff();
  // }
  delay(100);
}
