// Private 3: Fibonacci
#include "minicio.h"

int i;

int fib2(int n) {
    int a,b,start,c;
    a = 0;
    b = 1;
    start = 0;
    while (start < n){
        c = a + b;
        a = b;
        b = c;
        start = start + 1;
    }
    return a;
}

int main() {
    i = 1;
    while (i < 10){
        putint(fib2(i));
        i = i + 1;
    }
    return 0;
}