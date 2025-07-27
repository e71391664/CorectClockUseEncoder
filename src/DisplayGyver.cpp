/// Реалізація класу DisplayGyver для роботи з OLED-дисплеєм та режимом редагування часу
#include "DisplayGyver.h"
#include "RtcManager.h"
#include <GyverOLED.h>

// Глобальний об'єкт дисплея (можна перенести в private, якщо потрібно)
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

/// Конструктор з посиланням на RtcManager
DisplayGyver::DisplayGyver(RtcManager& rtc) : rtcManager(rtc) {}

/// Бітмап для анімації
const uint8_t bitmap_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x70, 0x30, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0x70, 0x30, 0x30, 0x20, 0x00, 0x00,
  0x00, 0x30, 0x78, 0xFC, 0x7F, 0x3F, 0x0F, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0x40, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0x7F, 0x33, 0x13, 0x1E, 0x1C, 0x1C, 0x0E, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xF7, 0xEF, 0x5F, 0x3F, 0x7F, 0xFE, 0xFD, 0xFB, 0xF1, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
};

/// Ініціалізує дисплей (OLED)
void DisplayGyver::init() {
    oled.init();
    //oled.setPower(true);
    oled.setContrast(230);
    oled.clear();
    lastUserActionMillis = millis();
}

/// Відображає поточний час великим шрифтом у верхній частині дисплея
void DisplayGyver::showTime(int hour, int minute, int second) {
    char buffer[9]; // Буфер для часу "HH:MM:SS" + null-термінатор
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hour, minute, second);

    oled.clear();
    oled.setScale(2);
   // oled.setFont(FONT_8x16);
    oled.setCursor(16, 0);
    oled.print(buffer);
    oled.update();
}

/// Відображає температуру під часом на дисплеї
void DisplayGyver::showTemperature(float tempC) {
    oled.clear();
    oled.setScale(1);
    oled.setCursorXY(0, 32);
    oled.print("Температура:");
    oled.setScale(2);
    oled.setCursorXY(24, 48);
    oled.print(tempC);
    oled.circle(105, 48, 2, OLED_STROKE);// окружность с центром в (x,y, с радиусом)
    oled.setCursorXY(110, 48);
    oled.print("C");
    oled.update();
}

/// Відображає два рядки: 'Уставка' та значення зміщення великим шрифтом
void DisplayGyver::showOffset(int offset) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%s%d", (offset >= 0 ? "+" : ""), offset);
    oled.clear();
    // Перший рядок — слово 'Уставка'
    oled.setScale(2);
    oled.setCursorXY(20, 0); // як було раніше, по центру
    oled.print(F("Уставка"));
    // Другий рядок — значення великим шрифтом, по центру, внизу
    oled.setScale(3);
    oled.setCursorXY(5, 40); // Y=40 — максимально низько для scale 3
    oled.print(buffer);
    oled.setScale(2);
    oled.setCursorXY(92, 48); // Y=40 — максимально низько для scale 3
    oled.print("хв.");
    oled.update();
}

/// Відображає повідомлення про початок обробки з анімацією
void DisplayGyver::showStartProcessing() {
    static uint8_t dots = 0;
    static unsigned long lastUpdate = 0;
    unsigned long now = millis();

    if (now - lastUpdate >= 250) { // 1000 мс / 4 = 250 мс на стан
        dots = (dots + 1) % 4;
        lastUpdate = now;
    }

    char buf[20];
    snprintf(buf, sizeof(buf), "Обробка");
    oled.clear();
    oled.setScale(2);
    oled.setCursorXY(8, 0);
    oled.print(buf);
    for (uint8_t i = 0; i < dots; i++) oled.print(".");
    oled.drawBitmap(40, 32, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
    oled.update();
}

/// Відображає час і температуру у прямокутнику з розірваною верхньою лінією
void DisplayGyver::showTimeAndTemperature(int hour, int minute, int second, float tempC) {
    char timeBuf[9];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d", hour, minute, second);

    oled.clear();
    // Час у верхній частині
    oled.setScale(2);
    oled.setCursorXY(16, 0);
    oled.print(timeBuf);

    // Параметри прямокутника
    int x0 = 8;   // Лівий край
    int x1 = 120; // Правий край
    int y0 = 32;  // Верхній край
    int y1 = 63;  // Нижній край

    // Слово "Температура" (scale 1) — вписано в розірвану верхню лінію
    oled.setScale(1);
    int tempTextX = x0 + 4;
    int tempTextY = y0 - 4; // Трохи вище лінії
    oled.setCursorXY(tempTextX, y0 - 4);
    oled.print("Температура");
    // Довжина слова (11 символів × 6 = 66 пікселів)
    int textEnd = tempTextX + 66;

    // Верхня лінія: зліва до слова, справа після слова
    oled.line(x0, y0, tempTextX - 2, y0); // Ліва частина
    oled.line(textEnd + 2, y0, x1, y0);   // Права частина

    // Бокові лінії
    oled.line(x0, y0, x0, y1);
    oled.line(x1, y0, x1, y1);

    // Значення температури по центру прямокутника
    oled.setScale(2);
    int tempValX = x0 + ((x1 - x0) - 48) / 2 - 20; // Зміщено вліво на один символ (12 пікселів)
    int tempValY = y0 + ((y1 - y0) - 16) / 2 + 2; // 16 — висота символу scale 2
    oled.setCursorXY(tempValX, tempValY);
    if (tempC >= 0) oled.print("+");
    oled.print(tempC);
    oled.circle(97, 40, 2, OLED_STROKE); // коло зміщено вгору на 4 пікселі та вліво на 6 пікселів
    oled.setCursorXY(100, tempValY);
    oled.print("C");

    // Нижня лінія (після тексту, щоб не перекривалась)
    oled.line(x0, y1, x1, y1);

    oled.update();
}

/// Відображає час і температуру з маскою блимання
void DisplayGyver::showTimeAndTemperature(int hour, int minute, int second, float tempC, uint8_t blinkMaskHour, uint8_t blinkMaskMin, uint8_t blinkMaskSec) {
    char timeBuf[9];
    char hBuf[3], mBuf[3], sBuf[3];
    snprintf(hBuf, sizeof(hBuf), "%02d", hour);
    snprintf(mBuf, sizeof(mBuf), "%02d", minute);
    snprintf(sBuf, sizeof(sBuf), "%02d", second);
    if (blinkMaskHour & 1) hBuf[0] = ' ';
    if (blinkMaskHour & 2) hBuf[1] = ' ';
    if (blinkMaskMin & 1) mBuf[0] = ' ';
    if (blinkMaskMin & 2) mBuf[1] = ' ';
    if (blinkMaskSec & 1) sBuf[0] = ' ';
    if (blinkMaskSec & 2) sBuf[1] = ' ';
    snprintf(timeBuf, sizeof(timeBuf), "%s:%s:%s", hBuf, mBuf, sBuf);

    oled.clear();
    oled.setScale(2);
    oled.setCursorXY(16, 0);
    oled.print(timeBuf);
    // Час у верхній частині
    // Параметри прямокутника
    int x0 = 8;
    int x1 = 120;
    int y0 = 32;
    int y1 = 63;
    // Слово "Температура" (scale 1) — вписано в розірвану верхню лінію
    oled.setScale(1);
    int tempTextX = x0 + 4;
    int tempTextY = y0 - 4; // Трохи вище лінії
    oled.setCursorXY(tempTextX, y0 - 4);
    oled.print("Температура");
    // Довжина слова (11 символів × 6 = 66 пікселів)
    int textEnd = tempTextX + 66;
    // Верхня лінія: зліва до слова, справа після слова
    oled.line(x0, y0, tempTextX - 2, y0); // Ліва частина
    oled.line(textEnd + 2, y0, x1, y0);   // Права частина
    // Бокові лінії
    oled.line(x0, y0, x0, y1);
    oled.line(x1, y0, x1, y1);
    // Значення температури по центру прямокутника
    oled.setScale(2);
    int tempValX = x0 + ((x1 - x0) - 48) / 2 - 20; // Зміщено вліво на один символ (12 пікселів)
    int tempValY = y0 + ((y1 - y0) - 16) / 2 + 2; // 16 — висота символу scale 2
    oled.setCursorXY(tempValX, tempValY);
    if (tempC >= 0) oled.print("+");
    oled.print(tempC);
    oled.circle(97, 40, 2, OLED_STROKE); // коло зміщено вгору на 4 пікселі та вліво на 6 пікселів
    oled.setCursorXY(100, tempValY);
    oled.print("C");
    // Нижня лінія (після тексту, щоб не перекривалась)
    oled.line(x0, y1, x1, y1);
    oled.update();
}

/// Відображає час і температуру з маскою блимання (структура)
void DisplayGyver::showTimeAndTemperature(const TimeBlinkView& view) {
    showTimeAndTemperature(view.hour, view.minute, view.second, view.tempC, view.blinkMaskHour, view.blinkMaskMin, view.blinkMaskSec);
}

/// Відображає поточний час і температуру (без параметрів)
void DisplayGyver::showTimeAndTemperature() {
    DateTime time = rtcManager.now();
    if (!time.isValid()) return;
    
    float tempC = rtcManager.getTemperature();
    showTimeAndTemperature(time.hour(), time.minute(), time.second(), tempC);
}

// --- Методи для корекції часу ---
/// Починає режим редагування часу
void DisplayGyver::startEdit() {
    DateTime time = rtcManager.now();
    if (!time.isValid()) return;
    
    editState = EDIT_HOUR;
    editHour = time.hour();
    editMinute = time.minute();
    editSecond = time.second();
    lastBlink = millis();
    blink = false;
    lastEditActionMillis = millis();
    firstEditEntry = true;
}

/// Оновлює стан редагування часу
void DisplayGyver::updateEdit() {
    static bool waitRelease = false;
    static unsigned long pressStart = 0;
    int *editField = nullptr;
    int maxValue = 0;
    EditState nextState = NORMAL;
    switch (editState) {
        case EDIT_HOUR:
            editField = &editHour;
            maxValue = 24;
            nextState = EDIT_MINUTE;
            break;
        case EDIT_MINUTE:
            editField = &editMinute;
            maxValue = 60;
            nextState = EDIT_SECOND;
            break;
        case EDIT_SECOND:
            editField = &editSecond;
            maxValue = 60;
            nextState = NORMAL;
            break;
        default:
            break;
    }
    if (editField) {
        if (millis() - lastBlink > 500) {
            blink = !blink;
            lastBlink = millis();
        }
        
        updateTimeBlinkView();

        if (millis() - lastEditActionMillis > 3000) {
            confirmEdit(rtcManager.now());
            return;
        }

        if (rtcManager.readConfirm()) { // BTN_CONN
            if (!waitRelease) {
                if (firstEditEntry) {
                    firstEditEntry = false;
                } else {
                    *editField = (*editField + 1) % maxValue;
                }
                waitRelease = true;
                lastEditActionMillis = millis();
            }
        } else if (waitRelease) {
            waitRelease = false;
        }
        if (rtcManager.readBack()) { // BTN_BACK
            editState = NORMAL;
            lastEditActionMillis = millis();
        }
        if (digitalRead(11) == LOW) { // BTN_CONF
            if (pressStart == 0) pressStart = millis();
            if (millis() - pressStart > 1000) {
                editState = nextState;
                blink = false;
                lastBlink = millis();
                pressStart = 0;
                waitRelease = false;
                lastEditActionMillis = millis();
                firstEditEntry = true;
                if (editState == NORMAL) confirmEdit(rtcManager.now());
            }
        } else {
            pressStart = 0;
        }
    }
}

/// Оновлює відображення часу з блиманням
void DisplayGyver::updateTimeBlinkView() {
    TimeBlinkView view;
    view.hour = editHour;
    view.minute = editMinute;
    view.second = editSecond;
    view.tempC = rtcManager.getTemperature();
    view.blinkMaskHour = (editState == EDIT_HOUR && blink) ? 2 : 0;
    view.blinkMaskMin = (editState == EDIT_MINUTE && blink) ? 2 : 0;
    view.blinkMaskSec = (editState == EDIT_SECOND && blink) ? 2 : 0;
    
    showTimeAndTemperature(view);
}

/// Скасовує режим редагування часу
void DisplayGyver::cancelEdit() { 
    editState = NORMAL; 
}

/// Підтверджує редагування часу
void DisplayGyver::confirmEdit(const DateTime& time) {
    // Зберігаємо новий час у RTC
    DateTime newTime(time.year(), time.month(), time.day(), editHour, editMinute, editSecond);
    rtcManager.setTime(newTime);
    editState = NORMAL;
}

/// Перевіряє, чи активний режим редагування часу
bool DisplayGyver::isEditing() const { 
    return editState != NORMAL; 
}

/// Отримує температуру з RtcManager
float DisplayGyver::getTemperature() {
    return rtcManager.getTemperature();
}

/// Вмикає дисплей
void DisplayGyver::wakeDisplay() {
  lastUserActionMillis = millis();
  if (!displayOn) {
    displayOn = true;
    init(); // Оновлюємо вміст
  }
}

/// Автоматичне вимкнення дисплея
void DisplayGyver::autoPowerOff(uint16_t timeout) {
  if (displayOn && (millis() - lastUserActionMillis > timeout)) {
    displayOn = false;
    oled.setPower(false); // вимикаємо дисплей
  }
}

/// Повертає посилання на час останньої дії користувача
unsigned long& DisplayGyver::getLastUserActionMillis() { return lastUserActionMillis; }

/// Повертає посилання на стан дисплея
bool& DisplayGyver::getDisplayOn() { return displayOn; }