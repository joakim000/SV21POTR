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

#include "../lib/unity/unity.h"
#include "cbuffer.h"

void cbuffer_print();
void setUp()
{

}

void tearDown()
{
}

int main(void)
{
    printf("Write A:");     cbuffer_write('A');          cbuffer_print();
    printf("peek|%c|\n",    cbuffer_peek());
    printf("Write A:");     cbuffer_write('B');          cbuffer_print();
    printf("peek|%c|\n",    cbuffer_peek());

    // printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    // printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    
    printf("Write C:");     cbuffer_write('C');          cbuffer_print();
    printf("read|%c|\n",    cbuffer_read());             cbuffer_print();
    // printf("read|%d|\n",    cbuffer_read());             cbuffer_print();
    // printf("read|%d|\n",    cbuffer_read());             cbuffer_print();
    printf("read|%d|\n",    cbuffer_read());             cbuffer_print();
    printf("Write D:");     cbuffer_write('D');          cbuffer_print();
    printf("Write E:");     cbuffer_write('E');          cbuffer_print();
    // printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("Write F:");     cbuffer_write('F');          cbuffer_print();
    printf("peek|%c|\n",    cbuffer_peek());
    // printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("Write G:");     cbuffer_write('G');          cbuffer_print();
    printf("Write H:");     cbuffer_write('H');          cbuffer_print();
    printf("Write I:");     cbuffer_write('I');          cbuffer_print();
    printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    printf("read|%c|\n", cbuffer_read());     cbuffer_print();
    
    
    // UNITY_BEGIN();

    // return UNITY_END();
}


