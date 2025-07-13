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
    //_lastBlinkedMinute = -1; // Ініціалізуємо окремо
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

//Blink after encoder state offset
void ReleManager::blinkOnEncoderUpdate(){
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

// Функція блимання в звичайному режимі
void ReleManager::blinkOnNormalMode(DateTime currentDateTime) {
  unsigned long currentMillis = millis();

  // Основна умова для запуску блимання:
  // 1. currentDateTime.second() == 0: Перевіряємо, що поточна секунда рівно 00.
  // 2. (currentDateTime - lastBlinkedDateTime).totalseconds() >= 60L:
  //    Перевіряємо, що з моменту останнього блимання пройшло щонайменше 60 секунд.
  //    Це запобігає багаторазовому спрацьовуванню протягом однієї хвилини.
  if (currentDateTime.second() == 0 &&
      (currentDateTime - lastBlinkedDateTime).totalseconds() >= 60L) {

    // Якщо умови виконані, це 00 секунд нової хвилини, починаємо блимання.
    normalModeBlinkStart = currentMillis; // Запам'ятовуємо час (millis()) початку блимання
    lastBlinkedDateTime = currentDateTime; // Оновлюємо повний DateTime останнього успішного блимання

    // Визначаємо, яка хвилина (парна/непарна)
    bool isOdd = !(currentDateTime.minute() % 2 == 0); // Чи це непарна хвилина?
    ReleOnOff(isOdd); // Вмикаємо відповідний світлодіод
    isBlinkingActive = true; // Встановлюємо, що блимання активне
  }

  // Умова для вимкнення LED:
  // Перевіряємо, чи минуло BLINK_PHASE_DURATION (2 секунди) з моменту увімкнення
  // І чи блимання дійсно було активним
  if (isBlinkingActive && (currentMillis - normalModeBlinkStart >= BLINK_PHASE_DURATION)) {
    digitalWrite(ODD, LOW); // Вимикаємо непарний світлодіод
    digitalWrite(EVEN, LOW); // Вимикаємо парний світлодіод
    isBlinkingActive = false; // Скидаємо прапорець, оскільки блимання завершено
  }
}

//Влк\Викл реле === чет\нечет
void ReleManager::ReleOnOff(bool isOdd){
    //не парне значення(1..3..5..7)
    if (isOdd) {
 //     Serial.println("🔴 (Нечетный)");
      digitalWrite(EVEN, LOW);
      digitalWrite(ODD, HIGH);
    } else {
   //   Serial.println("🟢 (Четный)");
      digitalWrite(ODD, LOW);
      digitalWrite(EVEN, HIGH);
    }
}

// Returns true if the LED blinking process is active.
bool ReleManager::isBusy() {
    return _isBlinkingActive;
}