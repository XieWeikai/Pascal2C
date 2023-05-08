#include <stdio.h>
#include <stdlib.h>

const double pi = 3.141590;
int intarray[6];
double realarray[-1];
int i;
int add(int x, int y) {
    int add;/* Auto Generated */
    add = (x + y);
    return add;/* Auto Generated */
}
void swap(/* Is Reference */int x, /* Is Reference */int y) {
    int temp;
    temp = x;
    x = y;
    y = temp;
}
void printarrays() {
    int i;
        writeln("Integer array:");
    for (i = 1; i <= 5; i++) {
                writeln("intArray[", i, "] = ", intarray[i]);
    }
        writeln("Real array:");
    for (i = -2; i <= 2; i++) {
                writeln("realArray[", i, "] = ", realarray[(i + 1)]);
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
        writeln("Before swap:");
        printarrays();
    if ((        add(intarray[2], intarray[4]) == 13)) {
                writeln("Swapping intArray[2] and intArray[4]");
                swap(intarray[2], intarray[4]);
    }
        writeln("After swap:");
        printarrays();
    return 0;
}
