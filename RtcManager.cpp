#include "RtcManager.h" // Включаємо заголовок нашого класу
#include "DisplayGyver.h" // Додаємо для використання DisplayGyver

// Реалізація конструктора
// Конструктор ініціалізує lastSecondMillis.
// rtc об'єкт буде ініціалізований автоматично з глобальним об'єктом Wire.
RtcManager::RtcManager() : lastSecondMillis(0) {}

// Ініціалізує RTC.
bool RtcManager::init() {
    pinMode(BTN_BACK, INPUT_PULLUP); // D10
    pinMode(BTN_CONF, INPUT_PULLUP); // D11
    
    // Починаємо I2C комунікацію (якщо ще не почато в setup()).
    // Це потрібно, якщо RtcManager::init() викликається до Wire.begin().
    Wire.begin();

    if (!rtc.begin()) {
        #ifdef DEBUG_MODE
        Serial.println(F("RTC err"));
        #endif
        return false;
    }
//    Примітка: Ви можете розкоментувати наступний блок для установки часу,
//    але не забудьте закоментувати його після першої успішної установки.
    // if (rtc.lostPower()) {
    //     rtc.adjust(DateTime(2025, 6, 29, 21, 30, 0));
    // }   

    DateTime checkTime = rtc.now();
    if (!checkTime.isValid()) {
        return false;
    } 
    return true;
}

// Отримує поточний час від RTC.
DateTime RtcManager::now() {
    return rtc.now();
}

// Отримує температуру від RTC.
float RtcManager::getTemperature() { return rtc.getTemperature(); }

// Встановлює час в RTC
void RtcManager::setTime(const DateTime& time) {
    rtc.adjust(time);
}

// Застосовує задане зміщення в хвилинах до поточного часу RTC.
void RtcManager::applyOffset(int offsetMinutes) {
    DateTime currentTime = rtc.now();
    if (!currentTime.isValid()) { return; }
    TimeSpan offsetSpan(0, 0, offsetMinutes, 0); // (дні, години, хвилини, секунди)
    DateTime newTime = currentTime + offsetSpan;
    if (!newTime.isValid()) { return; }

    rtc.adjust(newTime);
}

// Друк поточного часу в Serial-порт один раз в секунду.
void RtcManager::printTimeOncePerSecond() {
    if (millis() - lastSecondMillis >= 1000) {
        lastSecondMillis = millis();
        DateTime currentTime = rtc.now();
    }
}

bool RtcManager::readBack() {
    button1State = (digitalRead(BTN_BACK) == LOW); // true якщо натиснуто
    return button1State;
}

bool RtcManager::readConfirm() {
    confirmState = (digitalRead(BTN_CONF) == LOW);
    return confirmState;
}

// --- Методи для корекції часу ---
// (Видалено: startEdit, updateEdit, updateTimeBlinkView, setDisplayManager, cancelEdit, confirmEdit, isEditing)