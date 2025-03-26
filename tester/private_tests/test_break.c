#include "minicio.h"

int main() {
    int i;
    int j;

    i = 0;
    j = 0;

    for (j=0; j < 20; j = j+1) {
        putint(j);
        break;
    }
    putnewline();
    i = i + 1;
    return 1;
}