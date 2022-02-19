/**
 * @file main.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief Make a program using functions to read an expression including two operands and an operator as 3 arguments
 *        from command line and print the value of the evaluated expression to the output. If the expression is not valid,
 *        print an error message to the output.
 *        The expression shall be in the format of operand1 operator operand2. E.g. 23 + 45.
 *        The operands shall be two unsigned integers and the operator shall be +, -, *, / or %
 *        The code for operators (add, subtract, multiply, divide and modulus functions) shall be impelemented in a module (calculator)
 * 
 * @version 0.1
 * @date 2021-03-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include "calculator.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        (void)printf("Invalid number of arguments.\n");
        (void)printf("The expression shall include two unsigned intergers and an operator(\"+\", \"-\", \"* \", \"//\" or \"%%\").\n");
        (void)printf("Example: 23 + 12\n");
        exit(1);
    }

    char op = *argv[2];
    uint32_t a = 0, b = 0;




    if (NULL == strtouint(argv[1], NULL, 10))
    {
        (void)printf("The firs operand is invalid. The operands shall be unsigned integers.\n");
        exit(1);
    }

    if (NULL == strtouint(argv[3], &b))
    {
        (void)printf("The second operand is invalid. The operands shall be unsigned integers.\n");
        exit(1);
    }

    (void)printf("%u %c %u: ", a, op, b);
    switch (op)
    {
    case '+':
        (void)printf("%u\n", add(a, b));
        break;

    case '-':
        (void)printf("%d\n", subtract(a, b));
        break;

    case '*':
        (void)printf("%u\n", multiply(a, b));
        break;

    case '/':
        if (b == 0)
        {
            (void)printf("INFINITY\n");
        }
        else
        {
            (void)printf("%u\n", divide(a, b));
        }
        break;

    case '%':
        if (b == 0)
        {
            (void)printf("INFINITY\n");
        }
        else
        {
            (void)printf("%u\n", modulus(a, b));
        }
        break;

    default:
        (void)printf("Invalid operator! The operator shall be \"+\", \"-\", \"* \", \"//\" or \"%%\".\n");
        break;
    }

    return 0;
}
