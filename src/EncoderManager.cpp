#include "EncoderManager.h" // Включаємо заголовок нашого класу

// Реалізація конструктора
EncoderManager::EncoderManager(DisplayGyver& displayMgr, ReleManager& releMgr, RtcManager& rtcMgr)
    : _myEnc(DT, CLK), // Ініціалізуємо об'єкт RotaryEncoder
      _lastPos(0),
      _lastDebounce(0),
      _offset(0),
      _inEdit(false),
      _displayManager(displayMgr), // Ініціалізуємо посилання на DisplayGyver
      _releManager(releMgr),      // Ініціалізуємо посилання на ReleManager
      _rtcManager(rtcMgr)      // Ініціалізуємо посилання на ReleManager
{
    // Тіло конструктора, якщо потрібна додаткова логіка ініціалізації
}

// Ініціалізує енкодер.
void EncoderManager::init() {
    pinMode(SW, INPUT_PULLUP); // Кнопка енкодера з внутрішнім підтягуючим резистором
    _lastPos = _myEnc.read(); // Зчитуємо початкову позицію
    _lastDebounce = millis();
}

// Обробка натискання кнопки енкодера
void EncoderManager::encoderCheck(){
    if (buttonPressed()) {
      if (!inEditMode()) {
        enterEditMode();
      } else {
        _rtcManager.applyOffset(_offset);
          exitEditMode();
      }
    }
}

// Перевіряє натискання кнопки енкодера (з антидребезгом).
bool EncoderManager::buttonPressed() {
    int state = digitalRead(SW);
    if (state == LOW && (millis() - _lastDebounce) > 300) { // Якщо кнопка натиснута і пройшов час антидребезгу
        _lastDebounce = millis();
        return true;
    }
    return false;
}

// Повертає true, якщо енкодер знаходиться в режимі редагування.
bool EncoderManager::inEditMode() { return _inEdit; }

// Входить в режим редагування часу.
void EncoderManager::enterEditMode() {
    _offset = 0; // Скидаємо зміщення
    _myEnc.write(0); // Скидаємо позицію енкодера
    _lastPos = 0;
    _inEdit = true;
    _displayManager.showOffset(_offset); // Відображаємо нульове зміщення
}

// Виходить з режиму редагування часу.
void EncoderManager::exitEditMode() {
    _inEdit = false;

    if (_offset != 0) { // Якщо було встановлено зміщення
        _displayManager.showStartProcessing(); // Показуємо "Обробка зміщення!"
        _releManager.blinkOffsetFeedback(_offset); // Передаємо зміщення для зворотного зв'язку реле
    }
    _offset = 0; // Скидаємо зміщення після виходу
}

// Оновлює стан енкодера (зчитування обертання)
void EncoderManager::update() {
    if (_inEdit) {
        long current = _myEnc.read() / 4; // Ділимо на 4, якщо енкодер генерує 4 імпульси на крок
        if (current != _lastPos) {
            _offset = constrain(current, -120, 120); // Обмежуємо зміщення від -120 до 120
            _displayManager.showOffset(_offset); // Виводимо зміщення на дисплей

            _lastPos = current;
        }
    }
}
// Повертає поточне значення зміщення енкодера.
int EncoderManager::getOffset() const { return _offset; }