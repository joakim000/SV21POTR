#include "calculator.h"

uint32_t *strtouint(const char *str, uint32_t *ptr)
{
    uint32_t *result = ptr;

    *ptr = 0;

    for (char *d = (char *)str; *d != '\0'; d++)
    {
        if ((*d < '0') || (*d > '9'))
        {
            result = NULL;
            break;
        }

        *ptr *= 10;
        *ptr += (*d - '0');
    }

    return result;
}

uint32_t add(uint32_t m, uint32_t n)
{
    return (m + n);
}

int32_t subtract(uint32_t m, uint32_t n)
{
    return (m - n);
}

uint32_t multiply(uint32_t m, uint32_t n)
{
    return (m * n);
}

uint32_t divide(uint32_t m, uint32_t n)
{
    return (m / n);
}

uint32_t modulus(uint32_t m, uint32_t n)
{
    return (m % n);
}
