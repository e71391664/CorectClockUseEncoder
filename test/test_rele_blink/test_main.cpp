// test/test_rele_blink/test_main.cpp
#include <unity.h>
#include "Rele.h" 
#include <RTClib.h> 
#include "test_mocks.h" // Include our mock declarations

// Global constants for pins (must match Rele.cpp)
const uint8_t EVEN = 2; 
const uint8_t ODD = 3;  

// Create Rele instance for tests
Rele testRele; 

// Setup function for each test
void setUp(void) {
    __test_millis_val = 0; 
    __test_pin_state_idx = 0; 
    testRele = Rele(); // Reset Rele object

    // After constructor call, reset mock pin state index to ignore initial LOW settings
    TEST_ASSERT_EQUAL(2, __test_pin_state_idx); // Expect 2 calls from constructor
    __test_pin_state_idx = 0; 
}

void tearDown(void) {
    // Clean up if needed
}

// --- Tests (as before) ---

void test_rele_blink_no_action_if_not_second_0() {
    DateTime testTime(2025, 1, 1, 10, 30, 15); 
    testRele.blink(testTime);

    TEST_ASSERT_EQUAL(0, __test_pin_state_idx); 
}

void test_rele_blink_on_even_minute_second_0() {
    DateTime testTime(2025, 1, 1, 10, 30, 0); 
    testRele.blink(testTime);
    
    TEST_ASSERT_EQUAL(2, __test_pin_state_idx); 
    TEST_ASSERT_EQUAL(EVEN, __test_pin_states[0].pin);
    TEST_ASSERT_EQUAL(HIGH, __test_pin_states[0].state);
    TEST_ASSERT_EQUAL(ODD, __test_pin_states[1].pin);
    TEST_ASSERT_EQUAL(LOW, __test_pin_states[1].state);
}

void test_rele_blink_on_odd_minute_second_0() {
    setUp(); 
    DateTime testTime(2025, 1, 1, 10, 31, 0); 
    testRele.blink(testTime);
    
    TEST_ASSERT_EQUAL(2, __test_pin_state_idx);
    TEST_ASSERT_EQUAL(EVEN, __test_pin_states[0].pin);
    TEST_ASSERT_EQUAL(LOW, __test_pin_states[0].state);
    TEST_ASSERT_EQUAL(ODD, __test_pin_states[1].pin);
    TEST_ASSERT_EQUAL(HIGH, __test_pin_states[1].state);
}

void test_rele_blink_turns_off_after_2_seconds() {
    setUp(); 
    DateTime testTime(2025, 1, 1, 10, 30, 0); 
    testRele.blink(testTime); 
    
    __test_pin_state_idx = 0; 

    __test_millis_val += 1000; 
    testRele.blink(testTime); 
    TEST_ASSERT_EQUAL(0, __test_pin_state_idx); 

    __test_millis_val += 1000; 
    testRele.blink(testTime); 
    
    TEST_ASSERT_EQUAL(2, __test_pin_state_idx); 
    TEST_ASSERT_EQUAL(EVEN, __test_pin_states[0].pin); 
    TEST_ASSERT_EQUAL(LOW, __test_pin_states[0].state);
    TEST_ASSERT_EQUAL(ODD, __test_pin_states[1].pin); 
    TEST_ASSERT_EQUAL(LOW, __test_pin_states[1].state);
}

void test_rele_blink_not_retriggered_on_same_minute_second_0() {
    setUp(); 
    DateTime testTime(2025, 1, 1, 10, 30, 0);
    testRele.blink(testTime); 

    __test_pin_state_idx = 0; 

    __test_millis_val += 500; 
    testRele.blink(testTime); 

    TEST_ASSERT_EQUAL(0, __test_pin_state_idx); 
}

// --- Main Test Runner ---
void setup() {
    UNITY_BEGIN(); 

    RUN_TEST(test_rele_blink_no_action_if_not_second_0);
    RUN_TEST(test_rele_blink_on_even_minute_second_0);
    RUN_TEST(test_rele_blink_on_odd_minute_second_0);
    RUN_TEST(test_rele_blink_turns_off_after_2_seconds);
    RUN_TEST(test_rele_blink_not_retriggered_on_same_minute_second_0);

    UNITY_END(); 
}

void loop() {
}