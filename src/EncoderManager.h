#ifndef ENCODER_MANAGER_H
#define ENCODER_MANAGER_H

#include <Arduino.h>       // Потрібно для базових функцій Arduino (pinMode, digitalRead, millis, Serial)
#include <Encoder.h> // Для роботи з ротаційним енкодером

// Включаємо заголовочні файли для класів, з якими EncoderManager буде взаємодіяти.
// Припустимо, що DisplayGyver і ReleManager вже є класами.
#include "DisplayGyver.h" // Потрібен для showOffset, showStartProcessing
#include "ReleManager.h"    // Потрібен для blinkOffsetFeedback
#include "RtcManager.h"    // Потрібен для rtcMgr.applyOffset(_offset);

class EncoderManager {
private:
    // Піни енкодера та кнопки
    static const int CLK = 2;
    static const int DT = 3;
    static const int SW = 4;

    // Об'єкт ротаційного енкодера
    Encoder _myEnc;

    // Змінні стану енкодера
    long _lastPos;             // Остання позиція енкодера
    unsigned long _lastDebounce; // Час останнього натискання кнопки (для антидребезгу)
    int _offset;               // Поточне значення зміщення
    bool _inEdit;              // Чи знаходиться енкодер в режимі редагування

    // Посилання на об'єкти дисплея та реле для взаємодії
    DisplayGyver& _displayManager;
    ReleManager& _releManager;
    RtcManager& _rtcManager;

public:
    // Конструктор: приймає піни енкодера та посилання на об'єкти DisplayGyver і ReleManager
    EncoderManager(DisplayGyver& displayMgr, ReleManager& releMgr, RtcManager& rtcMgr);

    // Ініціалізує енкодер.
    void init();

    // Оновлює стан енкодера (зчитування обертання).
    void update();

    // Перевірка/уставка стану енкодера та його налаштуваня
    void encoderCheck();

    // Перевіряє натискання кнопки енкодера (з антидребезгом).
    bool buttonPressed();

    // Повертає true, якщо енкодер знаходиться в режимі редагування.
    bool inEditMode();

    // Входить в режим редагування часу.
    void enterEditMode();

    // Виходить з режиму редагування часу.
    void exitEditMode();

    // Повертає поточне значення зміщення енкодера.
    int getOffset() const; // const означає, що метод не змінює стан об'єкта
};

#endif // ENCODER_MANAGER_H