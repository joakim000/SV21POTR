#include "unity.h"
#include "calculator.h"

void test_strtouint(void)
{
    uint32_t num = 0;
    TEST_ASSERT_NULL(strtouint("123xc", &num));
    TEST_ASSERT_NULL(strtouint("xu123", &num));
    TEST_ASSERT_NULL(strtouint("u123c", &num));

    TEST_ASSERT_NOT_NULL(strtouint("123", &num));
    TEST_ASSERT_EQUAL_UINT32(123, num);
}

void test_addition(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, add(0, 0));
    TEST_ASSERT_EQUAL_UINT32(10, add(3, 7));
}

void test_subtraction(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, subtract(0, 0));
    TEST_ASSERT_EQUAL_INT32(4, subtract(7, 3));
    TEST_ASSERT_EQUAL_INT32(-4, subtract(3, 7));
}

void test_multiplication(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, multiply(0, 0));
    TEST_ASSERT_EQUAL_UINT32(21, multiply(7, 3));
}

void test_division(void)
{
    TEST_ASSERT_EQUAL_UINT32(7, divide(21, 3));
    TEST_ASSERT_EQUAL_UINT32(0, divide(3, 21));
}

void test_modulus(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, modulus(21, 3));
    TEST_ASSERT_EQUAL_UINT32(3, modulus(3, 21));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_strtouint);
    RUN_TEST(test_addition);
    RUN_TEST(test_subtraction);
    RUN_TEST(test_multiplication);
    RUN_TEST(test_division);
    RUN_TEST(test_modulus);

    return UNITY_END();
}
