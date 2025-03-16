// This program calculates the 10th fibonacci number
#include "minicio.h"

int fib(int n) {
    int i;
    int fib_nums[51];

    if (n < 2) return n;

    for (i = 0; i <= n; i = i + 1) {
        fib_nums[i] = 0;
    }

    fib_nums[0] = 0;
    fib_nums[1] = 1;

    for (i = 2; i <= n; i = i + 1) {
        fib_nums[i] = fib_nums[i - 1] + fib_nums[i - 2];
    }
    
    return fib_nums[n];
}

int main() {
    int res;
    res = fib(11);
    putint(res);
    putnewline();
    return 0;
}
