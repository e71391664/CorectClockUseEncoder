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
EncoderManager encoderManager (displayManager, releManager, rtcManager);  

/// Обробка логіки коли дисплей увімкнений
void handleDisplayOn() {
  if (displayManager.isEditing()) {
    // Режим редагування часу
    displayManager.updateEdit();
  } else {
    // Звичайний режим - перевіряємо вхід в редагування
    if (rtcManager.readConfirm()) {
      displayManager.startEdit();
      displayManager.wakeDisplay();
    }
  }
}

/// Обробка логіки коли дисплей вимкнений
void handleDisplayOff() {
  // Перевіряємо кнопки для пробудження
  if (rtcManager.readConfirm() || rtcManager.readBack()) {
    displayManager.wakeDisplay();
  }
}

//Відображення на дисплеї, реакція на уставку через енкодер та обробка реле
void normalWorkStage(){
  if (encoderManager.inEditMode()) {
    displayManager.showOffset(encoderManager.getOffset());
  } else if (releManager.isBusy()) {
    displayManager.showStartProcessing();
  } else {
      displayManager.showTimeAndTemperature();
      releManager.blinkOnNormalMode(rtcManager.now());
  }
}

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
  // --- Оновлення всіх менеджерів ---
  encoderManager.update();
  releManager.update();
  encoderManager.encoderCheck();
  
  // --- Керування живленням дисплея ---
  displayManager.autoPowerOff();
  
  // --- Основна логіка роботи ---
  if (displayManager.isDisplayOn()) {
    handleDisplayOn();
  } else {
    handleDisplayOff();
  }
  
  // --- Відображення стану ---
  normalWorkStage();
  
  delay(50);
}