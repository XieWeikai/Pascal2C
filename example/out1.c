#include <stdio.h>
#include <stdlib.h>

const double pi;
 = 3.141590;
int intarray[6];
double realarray[-1];
int i;
int add(int x, int y) {
    int ret_add;/* Auto Generated */
    ret_add = (x + y);
    return ret_add;/* Auto Generated */
}
void swap(/* Is Reference */int x, /* Is Reference */int y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void printarrays() {
    int i;
    printf("Integer array:");
    for (i = 1; i <= 5; i++) {
        printf("intArray[", i, "] = ", intarray[i]);
    }
    printf("Real array:");
    for (i = -2; i <= 2; i++) {
        printf("realArray[", i, "] = ", realarray[i]);
    }
}
// compilertest
int main(int argc, char* argv[]) {
    for (i = 1; i <= 5; i++) {
        intarray[i] = (i * 2);
    }
    realarray[-2] = 0.500000;
    realarray[-1] = 1.500000;
    realarray[0] = 2.500000;
    realarray[1] = 3.500000;
    realarray[2] = 4.500000;
    printf("Before swap:");
    printarrays();
    if ((add(intarray[2], intarray[4]) == 13)) {
        printf("Swapping intArray[2] and intArray[4]");
        swap(&intarray[2], &intarray[4]);
    }
    printf("After swap:");
    printarrays();
    printf(pi);
    return 0;
}
