#ifndef RELE_H
#define RELE_H

#include <Arduino.h> 

// Оголошуємо глобальні константи, якщо вони визначені деінде.
// Якщо вони визначені у main.cpp, то для тестування їх доведеться
// також перевизначити або мокнути.
// Наприклад:
// const uint8_t EVEN = 2; 
// const uint8_t ODD = 3; 

class DateTime; // Попередня декларація

class Rele {
public:
    Rele(); // Конструктор без параметрів
    void blink(const DateTime& time);
    void OnOff(bool state); 

private:
    // Ці члени класу більше не потрібні, якщо EVEN та ODD глобальні
    // uint8_t EVEN; 
    // uint8_t ODD;
    
    unsigned long releOffTime;
    bool releIsOn;
};

#endif // RELE_H