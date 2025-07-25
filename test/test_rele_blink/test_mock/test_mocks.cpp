// test/test_rele_blink/test_mocks.cpp
#include "test_mocks.h" // Include our mock declarations
#include <Arduino.h>    // Include Arduino.h for HIGH/LOW, etc. (even in mocks)

// Define mock variables
unsigned long __test_millis_val = 0;

PinState __test_pin_states[6];
int __test_pin_state_idx = 0;

// Define mock functions
// Make sure these match the declarations in test_mocks.h
unsigned long millis() {
    return __test_millis_val;
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (__test_pin_state_idx < 6) {
        __test_pin_states[__test_pin_state_idx].pin = pin;
        __test_pin_states[__test_pin_state_idx].state = val;
        __test_pin_state_idx++;
    }
}

void pinMode(uint8_t pin, uint8_t mode) {
    // No-op for tests
}