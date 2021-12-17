/** 
 * @brief SV21POTR Course B - C assignment 2
 * @author Joakim Odermalm
 */

/**
 * @file test.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief For VG, all the requirements shall be fulfilled and the module shall be tested fully.
 *        Some suggestions for testing:
  *           1) Test the initialization of the buffer
 *            2) Test the empty/full buffer case
 *            3) Test read/peek/write when the buffer is empty
 *            4) Test read/peek/write when the buffer is full (an overwritten occurres when we write to a full buffer)
 *            5) Test read/peek/write when the buffer is not empty and full
 * 
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <unity.h>
#include "cbuffer.h"

// #define DEBUG
#define VERBOSE true

/**
 * @note Test functions
 *          cbuffer_write        X
 *          cbuffer_clear        X
 *          cbuffer_isfull       T F      
 *          cbuffer_peek         X 
 *          cbuffer_read         X 
 *          cbuffer_available    0 - 8  
 * 
 *       Test special cases
 *          read empty           X
 *          peek empty           X   
 *          write overflow       X
 */


// Test utils
size_t config[3]; 
uint8_t* status[4]; 
void cbuffer_getconfig();
void cbuffer_getstatus();
void cbuffer_harderase();
void print_content() {
    printf("|");
    for (int i = 0; i < CBUFFER_SIZE; i++)
        printf("%c", *((status[3]) + i));
    printf("|");
}
void print_status(bool content) {
    if (VERBOSE) {
        printf("head:%-3llu tail:%-3llu count:%-3llu\t", *(status[0]), *(status[1]), *(status[2]));
        if (content) print_content();
        puts("");
    }
}
void WriteChars(int start, int end) {
    // Write some chars
    #ifdef DEBUG
    printf("Writing from %d to %d\n", start, end);
    #endif
    for (int i = start, j = i; i < end; i++, j++) {
        if (j > 126)
            j = 33;
        cbuffer_write(j);
    }
}

// Unity
void setUp() {
    cbuffer_clear();
    cbuffer_harderase();
}

void tearDown() {}

void Test_cbuffer_write() {
    // Should be clear
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init: cbuffer_available()");
    TEST_ASSERT_EQUAL_MESSAGE(false, cbuffer_isfull(), "Init: cbuffer_isfull()");
    
    size_t num_chars = 'A' + CBUFFER_SIZE < 126 ? CBUFFER_SIZE : 126 - 'A';
    WriteChars('A', 'A' + num_chars); 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE('A' + num_chars - 'A', cbuffer_available(), "Write done: cbuffer_available()");
 
    print_status(true);

    char c = *((status[3]) + config[1]);
    char* msg;
    for (int i = 0; i < num_chars; i++) {
        if ( config[2] ) 
            c = *((status[3]) + ( (config[1] + i) % CBUFFER_SIZE ) );
        else 
            c = *((status[3]) + ( (config[1] + CBUFFER_SIZE - i) % CBUFFER_SIZE ) );
        sprintf(msg, "Writecheck: %c", 'A' + i);
        #ifdef DEBUG
        printf("Writecheck expected:%c  c:%c\n", 'A' + i, c);
        #endif
        TEST_ASSERT_EQUAL_UINT8_MESSAGE('A' + i, c, msg);
    }
}

void Test_cbuffer_read() {
    // Should be clear
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init: cbuffer_available()");
    TEST_ASSERT_EQUAL_MESSAGE(false, cbuffer_isfull(), "Init: cbuffer_isfull()");
    
    // Write some chars
    WriteChars('A', 'A' + CBUFFER_SIZE); 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE('A' + CBUFFER_SIZE - 'A', cbuffer_available(), "Write done: cbuffer_available()");
    print_status(true);

    // Read some chars
    int i, j;
    char c;
    for (i = 'A', j=i; i < 'A' + CBUFFER_SIZE; i++, j++) {
        c = cbuffer_read();
        if (j > 126)
            j = 33;
        TEST_ASSERT_EQUAL_CHAR_MESSAGE(j, c, "cbuffer_read()");
        print_status(true);
    }
    // print_status(true);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "All read: cbuffer_available()");
}

void Test_cbuffer_peek() {
    // Should be clear
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init: cbuffer_available()");
    TEST_ASSERT_EQUAL_MESSAGE(false, cbuffer_isfull(), "Init: cbuffer_isfull()");
    
    // Write some chars
    WriteChars('A', 'A' + CBUFFER_SIZE); 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE('A' + CBUFFER_SIZE - 'A', cbuffer_available(), "Write done: cbuffer_available()");
    print_status(true);

    // Peek a char
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('A', cbuffer_peek(), "Once: cbuffer_peek()");
    print_status(true);

    // Peek again, should be same
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('A', cbuffer_peek(), "Again: cbuffer_peek()");
    print_status(true);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(CBUFFER_SIZE, cbuffer_available(), "After peek: cbuffer_available()");
}


void Test_cbuffer_clear() {
    WriteChars('A', 'D');   // Write some chars
    print_status(true);       // Print status
    cbuffer_clear();        // Call function

    // Should be clear
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_EQUAL(false, cbuffer_isfull());
}

void Test_cbuffer_isfull() {
    WriteChars('A', 'A' + CBUFFER_SIZE); // Fill buf exactly
    print_status(true);
    TEST_ASSERT_EQUAL(true, cbuffer_isfull());
    cbuffer_read();
    TEST_ASSERT_EQUAL(false, cbuffer_isfull());
}

void Test_cbuffer_available() { 
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Init");

    for (char i = 0; i < CBUFFER_SIZE; i++) {
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, cbuffer_available(), "Writing");
        cbuffer_write(i+65); // Printable chars
        print_status(true);

    }
    for (char i = CBUFFER_SIZE; i >= 0; i--) {
        print_status(true);
        TEST_ASSERT_EQUAL_UINT8_MESSAGE(i, cbuffer_available(), "Reading");
        cbuffer_read();
    }

    // Read empty
    cbuffer_read();
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, cbuffer_available(), "Exit");
}

void Test_case_empty() {
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_peek());

    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_read());
}

void Test_case_write_overflow() {
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    WriteChars('A', 'A' + CBUFFER_SIZE); // Fill buf exactly
    print_status(true);
    TEST_ASSERT_EQUAL(true, cbuffer_isfull());

    // One more write should evict first write (A)
    cbuffer_write('!'); 
    print_status(true);
    // Next read should then be B
    TEST_ASSERT_EQUAL_CHAR_MESSAGE('B', cbuffer_read(), "A should be evicted"); 
}

int main(void)
{
    cbuffer_getconfig(config);
    cbuffer_getstatus(status);

    if (VERBOSE) {
        puts("=== Buffer configuration ===================================");
        printf("Size: %llu   Initial index: %llu   Direction: %s\n", config[0], config[1], config[2] ? "clockwise" : "counterclockwise");
        puts("============================================================");
    }

    UNITY_BEGIN();
    RUN_TEST(Test_cbuffer_write);
    RUN_TEST(Test_cbuffer_read);
    RUN_TEST(Test_cbuffer_peek);
    RUN_TEST(Test_cbuffer_clear);
    RUN_TEST(Test_cbuffer_isfull);
    RUN_TEST(Test_cbuffer_available);
    RUN_TEST(Test_case_empty);
    RUN_TEST(Test_case_write_overflow);
    return UNITY_END();
}


/*
    typedef void (*test_p_t)();
    test_p_t tests[] = { Test_cbuffer_write, Test_cbuffer_read, Test_cbuffer_peek };
    for (int i = 0; i < sizeof(tests)/sizeof(test_p_t) ; i++)
        RUN_TEST(tests[i]);
*/
