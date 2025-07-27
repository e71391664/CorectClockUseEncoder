#pragma once
#include <GyverOLED.h>
class RtcManager;
class DateTime;

/// Структура для передачі часу та масок блимання
struct TimeBlinkView {
    int hour;
    int minute;
    int second;
    float tempC;
    uint8_t blinkMaskHour;
    uint8_t blinkMaskMin;
    uint8_t blinkMaskSec;
};

class DisplayGyver {
private:
    RtcManager& rtcManager; ///< Посилання на менеджер RTC
    unsigned long lastUserActionMillis = 0; ///< Час останньої дії користувача
    bool displayOn = true; ///< Стан дисплея (увімкнений/вимкнений)
    
    // --- Для режиму корекції часу ---
    enum EditState {
        NORMAL, ///< Звичайний режим
        EDIT_HOUR, ///< Редагування годин
        EDIT_MINUTE, ///< Редагування хвилин
        EDIT_SECOND ///< Редагування секунд
    };
    
    EditState editState = NORMAL; ///< Поточний стан редагування
    unsigned long lastBlink = 0; ///< Час останнього перемикання блимання
    bool blink = false; ///< Стан блимання
    int editHour = 0, editMinute = 0, editSecond = 0; ///< Значення для редагування часу
    unsigned long lastEditActionMillis = 0; ///< Час останньої дії при редагуванні
    bool firstEditEntry = true; ///< Перший вхід у режим редагування
public:
    /// Конструктор з посиланням на RtcManager
    DisplayGyver(RtcManager& rtc);
    /// Ініціалізує дисплей (OLED)
    void init();
    /// Відображає два рядки: 'Уставка' та значення зміщення великим шрифтом
    void showOffset(int offset);
    /// Відображає повідомлення про початок обробки
    void showStartProcessing();
    /// Відображає температуру під часом на дисплеї
    void showTemperature(float tempC);
    /// Відображає поточний час великим шрифтом у верхній частині дисплея
    void showTime(int hour, int minute, int second);
    /// На одному екрані відображає час і температуру у прямокутнику з розірваною верхньою лінією
    void showTimeAndTemperature(int hour, int minute, int second, float tempC);
    /// На одному екрані відображає час і температуру з маскою блимання
    void showTimeAndTemperature(int hour, int minute, int second, float tempC, uint8_t blinkMaskHour, uint8_t blinkMaskMin, uint8_t blinkMaskSec);
    /// На одному екрані відображає час і температуру з маскою блимання (структура)
    void showTimeAndTemperature(const TimeBlinkView& view);
    /// На одному екрані відображає поточний час і температуру (без параметрів)
    void showTimeAndTemperature();
    /// Запускає автоматичне вимкнення дисплея
    void autoPowerOff(uint16_t timeout = 60000);
    /// Вмикає дисплей
    void wakeDisplay();
    /// Отримує температуру з RtcManager
    float getTemperature();
    /// Повертає посилання на час останньої дії користувача
    unsigned long& getLastUserActionMillis();
    /// Повертає посилання на стан дисплея
    bool& getDisplayOn();
    // --- Методи для корекції часу ---
    /// Починає режим редагування часу
    void startEdit();
    /// Оновлює стан редагування часу
    void updateEdit();
    /// Оновлює відображення часу з блиманням
    void updateTimeBlinkView();
    /// Скасовує режим редагування часу
    void cancelEdit();
    /// Підтверджує редагування часу
    void confirmEdit(const DateTime& time);
    /// Перевіряє, чи активний режим редагування часу
    bool isEditing() const;
}; 