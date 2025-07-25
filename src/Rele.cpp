// src/Rele.cpp
#include "Rele.h"
#include <RTClib.h> 

// Global constants for pins
const uint8_t EVEN = 2; 
const uint8_t ODD = 3;  

#ifdef ARDUINO
// When building for Arduino, use real Arduino functions
#include <Arduino.h>
#else
// When building for test, use mocks
#include "../test/test_rele_blink/test_mocks.h" // Adjust path as needed
#endif

Rele::Rele() : releOffTime(0), releIsOn(false) {
    pinMode(EVEN, OUTPUT);
    digitalWrite(EVEN, LOW);
    pinMode(ODD, OUTPUT); // Moved ODD pinMode here for logical grouping
    digitalWrite(ODD, LOW);
}

void Rele::OnOff(bool state) {
    if (state) {
        digitalWrite(EVEN, HIGH);
        digitalWrite(ODD, LOW); 
    } else {
        digitalWrite(EVEN, LOW);
        digitalWrite(ODD, HIGH); 
    }
}

void Rele::blink(const DateTime& time) {
    unsigned long currentMillis = millis();

    if (time.second() == 0 && !releIsOn) {
        OnOff(time.minute() % 2 == 0);
        releIsOn = true;
        releOffTime = currentMillis + 2000;
        #ifdef DEBUG_MODE
        Serial.println("LEDs ON");
        #endif
    }
    
    if (releIsOn && currentMillis >= releOffTime) {
        digitalWrite(EVEN, LOW);
        digitalWrite(ODD, LOW);
        releIsOn = false;
        #ifdef DEBUG_MODE
        Serial.println("LEDs OFF");
        #endif
    }
}