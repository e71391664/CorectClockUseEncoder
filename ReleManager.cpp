#include "ReleManager.h"
#include <RTClib.h> // Потрібно для DateTime now;
// No need to include "globals.h" here anymore, as pins are passed to the constructor.

// Constructor implementation
ReleManager::ReleManager()
    : _isBlinkingActive(false),
      _totalBlinksToPerform(0),
      _currentBlinkStep(0),
      _blinkStartMillis(0),
      _normalModeBlinkStart(0)
      {
    // Member initializer list is used to initialize private members.
    _lastBlinkedMinute = -1; // Ініціалізуємо окремо
}

// Initializes the LED indicators.
void ReleManager::init() {
    pinMode(ODD, OUTPUT);
    pinMode(EVEN, OUTPUT);
    digitalWrite(ODD, LOW);
    digitalWrite(EVEN, LOW);
}

// Starts the LED blinking process for offset feedback.
void ReleManager::blinkOffsetFeedback(int offset) {
    if (offset <= 0) {
        digitalWrite(ODD, LOW);
        digitalWrite(EVEN, LOW);
        _isBlinkingActive = false;
        return;
    }

    _totalBlinksToPerform = offset;
    _currentBlinkStep = 0;
    _isBlinkingActive = true;
    _blinkStartMillis = millis();
}

// Updates the LED blinking state (call this in loop()).
void ReleManager::update() {
    blinkOnEncoderUpdate();
}

void  ReleManager::blinkOnEncoderUpdate(){
    unsigned long currentMillis = millis();

    if (!_isBlinkingActive) return;
    if (currentMillis - _blinkStartMillis < BLINK_PHASE_DURATION) return;

    _blinkStartMillis = currentMillis;
   
   // Even steps: LED is ON (either red or green)
  if (_currentBlinkStep % 2 == 0) {
        bool isOdd = ((_currentBlinkStep / 2) % 2 == 0); // Determine if it's a red or green phase

        if (isOdd) {
            digitalWrite(ODD, HIGH); // Red LED (assuming led1Pin is for red)
            digitalWrite(EVEN, LOW);
        } else {
            digitalWrite(ODD, LOW);
            digitalWrite(EVEN, HIGH); // Green LED (assuming led2Pin is for green)
        }
    }
    // Odd steps: Both LEDs are OFF (pause between blinks)
    else {
        digitalWrite(ODD, LOW);
        digitalWrite(EVEN, LOW);

        // Check if all blinks have been completed after the off phase
        if ((_currentBlinkStep + 1) / 2 >= _totalBlinksToPerform) {
            _isBlinkingActive = false;
            return;
        }
    }
  _currentBlinkStep++;
}

void ReleManager::blinkOnNormalMode(DateTime currentDateTime) {
  unsigned long currentMillis = millis();
  // Перевіряємо, що поточна хвилина більше за ту в якій блимали.
  // ми проігноруемо логіку на від'емне зміщення в методі виходу з offset 
    if (currentDateTime > _lastBlinkedMinute) {
    // Починаємо новий цикл блимання
    _normalModeBlinkStart = currentMillis;
    
    // Визначаємо який LED увімкнути
    bool isEven = currentDateTime.minute() % 2 == 0;
    if (isEven) {  
      digitalWrite(EVEN, HIGH);// Зелений LED для парних хвилин
      digitalWrite(ODD, LOW); // Червоний LED для непарних хвилин
    } else {
      digitalWrite(ODD, HIGH);
      digitalWrite(EVEN, LOW);
    }
    _lastBlinkedMinute = currentDateTime; // Update the last blinked minute
  }
  
  // Перевіряємо, чи потрібно вимкнути LED після 2 секунд
  if (currentMillis - _normalModeBlinkStart >= BLINK_PHASE_DURATION) {
    digitalWrite(ODD, LOW);
    digitalWrite(EVEN, LOW);
  }
}

// Returns true if the LED blinking process is active.
bool ReleManager::isBusy() {
    return _isBlinkingActive;
}