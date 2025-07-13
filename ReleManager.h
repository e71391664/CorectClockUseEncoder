#ifndef RELE_MANAGER_H
#define RELE_MANAGER_H

#include <Arduino.h> // Include Arduino core for pinMode, digitalWrite, millis, etc.
#include <RTClib.h> // Для DateTime

// // Клас для керування RTC-модулем (DS3231 або сумісним)
class ReleManager {
public:
    // Constructor: Initializes the Rele object with LED pin numbers.
    ReleManager();

    // Initializes the LED indicators.
    void init();

    // Starts the LED blinking process for offset feedback.
    void blinkOffsetFeedback(int offset);

    // Updates the LED blinking state (call this in your loop()).
    void update();

    // Updates the LED blinking state on set state Encoder.
    void blinkOnEncoderUpdate();

    // Вмикає/вимикає реле в робочому режимі
    void blinkOnNormalMode(DateTime minute);

    // Вимикачь реле в залежності від парності хвилини
    void ReleOnOff(bool isOdd);

    // Returns true if the LED blinking process is active.
    bool isBusy();

private:   
    bool _isBlinkingActive;   // Flag to indicate if blinking is active
    int _totalBlinksToPerform; // Total number of blinks for the current sequence
    int _currentBlinkStep;    // Current step in the blinking sequence
    unsigned long _blinkStartMillis; // Timestamp when the current blink phase started

    unsigned long  _normalModeBlinkStart; // Timestamp when the current blink phase started
    // Constant for the duration of each blink phase (e.g., 2000ms for on/off)
    static const unsigned int BLINK_PHASE_DURATION = 2000;// Тривалість горіння LED (2 секунди)

    static const int ODD = 5;             // Pin for the first LED
    static const int EVEN = 6;             // Pin for the second LED
 
    // Змінна для відстеження останньої хвилини блимання
   // DateTime _lastBlinkedMinute;
    unsigned long normalModeBlinkStart = 0;//час початку блимання в звичайному режимі
    DateTime lastBlinkedDateTime = -1;// Змінна для відстеження останньої хвилини блимання
    bool isBlinkingActive = false; // Нова змінна для відстеження активності блимання
  
};

#endif // RELE_H