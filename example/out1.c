#include <stdio.h>
#include <stdlib.h>

const double pi = 3.141590;
int intarray[6];
double realarray[5];
int i;
int add(int x, int y) {
    int ret_add;/* Auto Generated */
    ret_add = (x + y);
    return ret_add;/* Auto Generated */
}
void swap(/* Is Reference */int *x, /* Is Reference */int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void printarrays() {
    int i;
    printf("%s\n", "Integer array:");
    for (i = 1; i <= 5; i++) {
        printf("%s%d%s%d\n", "intArray[", i, "] = ", intarray[i]);
    }
    printf("%s\n", "Real array:");
    for (i = 3; i <= 5; i++) {
        printf("%s%d%s%f\n", "realArray[", i, "] = ", realarray[i]);
    }
}
// compilertest
int main(int argc, char* argv[]) {
    for (i = 1; i <= 5; i++) {
        intarray[i] = (i * 2);
    }
    realarray[3] = 0.500000;
    realarray[4] = 1.500000;
    realarray[5] = 2.500000;
    realarray[6] = 3.500000;
    realarray[7] = 4.500000;
    printf("%s\n", "Before swap:");
    printarrays();
    if ((add(intarray[2], intarray[4]) == 13)) {
        printf("%s\n", "Swapping intArray[2] and intArray[4]");
        swap(&intarray[2], &intarray[4]);
    }
    printf("%s\n", "After swap:");
    printarrays();
    printf("%f\n", pi);
    return 0;
}
