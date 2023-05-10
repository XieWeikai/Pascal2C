#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int n = 10;
int ans[10];
int i;
void inc(/* Is Reference */int *a) {
    *a = (*a + 1);
}
void mergesort(int l, int r) {
    int mid;
    int i;
    int ptr_1;
    int ptr_2;
    int tmp[1000];
    if ((r <= l)) {
        return;
    } else {
        mid = ((l + r) div 2);
        mergesort(l, mid);
        mergesort((mid + 1), r);
        ptr_1 = l;
        ptr_2 = (mid + 1);
        for (i = l; i <= r; i++) {
            if (((ptr_1 < (mid + 1)) && ((ptr_2 > r) || (ans[ptr_1 - 1] < ans[ptr_2 - 1])))) {
                tmp[i - 1] = ans[ptr_1 - 1];
                inc(&ptr_1);
            } else {
                tmp[i - 1] = ans[ptr_2 - 1];
                inc(&ptr_2);
            }
        }
        for (i = l; i <= r; i++) {
            ans[i - 1] = tmp[i - 1];
        }
    }
}
// merge_sort
int main(int argc, char* argv[]) {
    for (i = 1; i <= n; i++) {
        ans[i - 1] = (n - i);
    }
    mergesort(1, n);
    for (i = 1; i <= n; i++) {
        printf("%d\n", ans[i - 1]);
    }
    return 0;
}
