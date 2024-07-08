#include <stdio.h>

int main() {
    // Integer types
    int integerVar = 42;
    printf("Integer variable: %d\n", integerVar);

    // Floating-point types
    float floatVar = 3.14;
    double doubleVar = 2.71828;
    printf("Float variable: %f\n", floatVar);
    printf("Double variable: %lf\n", doubleVar);

    // Character type
    char charVar = 'A';
    printf("Character variable: %c\n", charVar);

    // Boolean type (C99 and later)
    _Bool boolVar = 1; // true
    printf("Boolean variable: %d\n", boolVar);

    // Unsigned integer types
    unsigned int uintVar = 100;
    printf("Unsigned integer variable: %u\n", uintVar);

    // Long types
    long longVar = 1234567890L;
    printf("Long variable: %ld\n", longVar);

    // Pointer type
    int* ptrVar = &integerVar;
    printf("Pointer variable value: %d\n", *ptrVar);

    return 0;
}
