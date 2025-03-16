#include "minicio.h"

// This program finds the sum of all pairs of numbers from 0 to 9
int main() {
    int size, sum, i, j;
    size = getint();
    sum = 0;
    for (i = 0; i < size; i = i + 1) {
        for (j = i; j < size; j = j + 1) {
            putcharacter('C');
            putint(i);
            putint(j);
            putnewline();
            sum = sum + i + j;
        }
    }

    putint(sum);

    return 1;
}