#include <stdio.h>

#define PI 3.14159

typedef int TIntArray[5];
typedef double TRealArray[5];

TIntArray intArray;
TRealArray realArray;
int i;

int Add(int x, int y) {
    return x + y;
}

void Swap(int *x, int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void PrintArrays() {
    int i;
    printf("Integer array:\n");
    for (i = 0; i < 5; i++)
        printf("intArray[%d] = %d\n", i + 1, intArray[i]);

    printf("Real array:\n");
    for (i = -2; i <= 2; i++)
        printf("realArray[%d] = %f\n", i, realArray[i + 2]);
}

int main() {
    for (i = 0; i < 5; i++)
        intArray[i] = (i + 1) * 2;

    realArray[0] = 0.5;
    realArray[1] = 1.5;
    realArray[2] = 2.5;
    realArray[3] = 3.5;
    realArray[4] = 4.5;

    printf("Before swap:\n");
    PrintArrays();

    if (Add(intArray[1], intArray[3]) == 13) {
        printf("Swapping intArray[2] and intArray[4]\n");
        Swap(&intArray[1], &intArray[3]);
    }

    printf("After swap:\n");
    PrintArrays();

    printf("PI = %.5f\n", PI);
    return 0;
}
