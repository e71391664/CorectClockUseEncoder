#ifndef RTC_H
#define RTC_H

#include <Arduino.h> // Потрібен для базових функцій Arduino, таких як Serial, millis()
#include <RTClib.h>  // Для класу RTC_DS3231, DateTime, TimeSpan
#include <Wire.h>    // Для I2C комунікації
#include "DisplayGyver.h" // Додаємо для використання DisplayGyver

/// Клас для керування RTC-модулем (DS3231 або сумісним)
class RtcManager {
private:
    RTC_DS3231 rtc; ///< Об'єкт RTC, ініціалізується з глобальним об'єктом Wire (I2C)
    unsigned long lastSecondMillis; ///< Для printTimeOncePerSecond
    static const uint8_t DS_ADDRESS = 0x68; ///< I2C адреса для більшості RTC (DS1307, DS3231)
    static const uint8_t BTN_CONF = 5; ///< D6 CONFIRM on display plate
    static const uint8_t BTN_BACK = 6; ///< D7 BACK on display plate

    bool backState = false; ///< Стан кнопки BTN_BACK
    bool confirmState = false; ///< Стан кнопки BTN_CONF
public:
    /// Конструктор
    RtcManager();
    /// Ініціалізує RTC
    bool init();
    /// Повертає поточний час з RTC
    DateTime now();
    /// Повертає температуру з RTC
    float getTemperature ();
    /// Застосовує зміщення в хвилинах до поточного часу RTC
    void applyOffset(int offsetMinutes);
    /// Встановлює час у RTC
    void setTime(const DateTime& time);
    /// Друкує поточний час у Serial-порт раз на секунду
    void printTimeOncePerSecond();
    /// Зчитує стан кнопки BTN_BACK
    bool readBack();
    /// Зчитує стан кнопки BTN_CONF
    bool readConfirm();
};

#endif // RTC__H