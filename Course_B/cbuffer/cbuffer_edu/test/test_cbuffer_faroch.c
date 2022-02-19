/**
 * @file test.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief For VG, all the requirements shall be fulfilled and the module shall be tested fully.
 *        Some suggestions for testing:
 *            1) Test the clear functionality
 *            2) Test read/peek/write when the buffer is empty
 *            3) Test read/peek/write when the buffer is full (an overwrite occurres when we write to a full buffer)
 *            4) Test read/peek/write when the buffer is not empty and full
 *
 * @version 0.1
 * @date 2021-03-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <unity.h>
#include "cbuffer.h"


// Test utils
size_t config[3]; 
uint8_t* status[4]; 
void cbuffer_getconfig();
void cbuffer_getstatus();
void cbuffer_harderase();

void setUp() {}

void tearDown()
{
    cbuffer_clear();
}

void test_cbuffer_clear(void)
{
    cbuffer_write(100);
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    cbuffer_write(110);
    TEST_ASSERT_EQUAL_UINT8(2, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(100, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    cbuffer_clear();

    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
}

void test_read_write_buffer(void)
{
    for (uint8_t i = 0; i < CBUFFER_SIZE - 3; i++)
    {
        cbuffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(i + 1, cbuffer_available());
        TEST_ASSERT_FALSE(cbuffer_isfull());
    }

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 3, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 4, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(2, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 5, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(3, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 6, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    for (uint8_t i = 0; i < 4; i++)
    {
        cbuffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 5U + i, cbuffer_available());
        TEST_ASSERT_FALSE(cbuffer_isfull());
    }

    TEST_ASSERT_EQUAL_UINT8(4, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 3, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());
}

void test_read_empty_buffer(void)
{
    cbuffer_read();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    cbuffer_write(100);
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    cbuffer_write(110);
    TEST_ASSERT_EQUAL_UINT8(2, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(100, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(110, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    cbuffer_read();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());

    cbuffer_write(100);
    TEST_ASSERT_EQUAL_UINT8(100, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());
}

void test_read_full_buffer(void)
{
    for (uint8_t i = 0; i < CBUFFER_SIZE; i++)
    {
        cbuffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(i + 1, cbuffer_available());
    }
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE, cbuffer_available());
    TEST_ASSERT_TRUE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - 1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());
}

void test_write_empty_buffer(void)
{
    cbuffer_write(1);
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    cbuffer_write(1);
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());
}

void test_write_full_buffer(void)
{
    for (uint8_t i = 0; i < CBUFFER_SIZE; i++)
    {
        cbuffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(i + 1, cbuffer_available());
    }
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE, cbuffer_available());

    cbuffer_write(10);
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE, cbuffer_available());

    cbuffer_write(20);
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE, cbuffer_available());

    for (uint8_t i = 0; i < CBUFFER_SIZE - 2; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i + 3, cbuffer_peek());
        TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - i, cbuffer_available());

        TEST_ASSERT_EQUAL_UINT8(i + 3, cbuffer_read());
        TEST_ASSERT_EQUAL_UINT8(CBUFFER_SIZE - i - 1, cbuffer_available());
        TEST_ASSERT_FALSE(cbuffer_isfull());
    }

    TEST_ASSERT_EQUAL_UINT8(10, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(2, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(10, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(20, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(1, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(20, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0, cbuffer_available());
    TEST_ASSERT_FALSE(cbuffer_isfull());
}

int main(void)
{
    cbuffer_getconfig(config);
    // cbuffer_getstatus(status);

    if (true) {
        puts("=== Buffer configuration ===========================================");
        printf("=   Size: %-3llu   Initial index: %-3llu   Direction: %-16s   =\n", config[0], config[1], config[2] ? "clockwise" : "counterclockwise");
        puts("====================================================================");
    }

    UNITY_BEGIN();

  

    

    RUN_TEST(test_cbuffer_clear);
    RUN_TEST(test_read_write_buffer);
    RUN_TEST(test_read_empty_buffer);
    RUN_TEST(test_read_full_buffer);
    RUN_TEST(test_write_empty_buffer);
    RUN_TEST(test_write_full_buffer);

    return UNITY_END();
}
