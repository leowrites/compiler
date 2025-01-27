// This program finds the sum of all pairs of numbers from 0 to 9
int main() {
    int size, sum, u, i, j;
    size = 10;
    sum = 0;
    for (i = 0; i < size; i = i + 1) {
        for (j = i; j < size; j = j + 1) {
            sum = sum + i + j;
        }
    }

    return 1;
}