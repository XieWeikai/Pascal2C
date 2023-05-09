#include <stdio.h>
#include <stdlib.h>

const int n = 10;
int mas[11];
int i;
int random_(int a) {
    int ret_random_;/* Auto Generated */
    ret_random_ = 3;
    return ret_random_;/* Auto Generated */
}
void inc(/* Is Reference */int *a) {
    *a = (*a + 1);
}
void mergesort(int a, int c) {
    int x;
    int j;
    int i;
    int n1;
    int n2;
    int rez[1000];
    x = ((a + c) / 2);
    if (a == c) return;
    mergesort(a, x);
    mergesort((x + 1), c);
    n1 = a;
    n2 = (x + 1);
    for (i = a; i <= c; i++) {
        if (((n1 < (x + 1)) & ((n2 > c) | (mas[n1 - 1] < mas[n2 - 1])))) {
            rez[i - 1] = mas[n1 - 1];
            inc(&n1);
        } else {
            rez[i - 1] = mas[n2 - 1];
            inc(&n2);
        }
    }
    for (j = a; j <= c; j++) {
        mas[j - 1] = rez[j - 1];
    }
}
// merge_sort
int main(int argc, char* argv[]) {
    for (i = 1; i <= n; i++) {
        mas[i - 1] = random_(20);
    }
    printf("%d\n", mas[1]);
    mergesort(1, n);
    printf("%d\n", mas[1]);
    return 0;
}
