// test/test_rele_blink/test_mocks.h
#ifndef TEST_MOCKS_H
#define TEST_MOCKS_H

#include <stdint.h> // For uint8_t, etc.

// Declare external mock variables
extern unsigned long __test_millis_val;

// Structure to capture pin states
struct PinState {
    uint8_t pin;
    uint8_t state;
};
extern PinState __test_pin_states[6]; // Sufficient size for our tests
extern int __test_pin_state_idx;

// Declare mock functions
#ifdef __cplusplus
extern "C" { // Ensure C linkage for mock functions if called from C++
#endif

// Mock Arduino functions
unsigned long millis();
void digitalWrite(uint8_t pin, uint8_t val);
void pinMode(uint8_t pin, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif // TEST_MOCKS_H