// This program calculates the 10th fibonacci number

int fib(int n) {
    int size, i, j;
    int fib_nums[10];

    if (n < 2) return n;
    size = n + 1;
    
    for (i = 0; i <= n; i = i + 1) {
        fib_nums[i] = 0;
    }
    
    fib_nums[0] = 0;
    fib_nums[1] = 1;
    
    for (i = 2; i <= n; i = i + 1) {
        fib_nums[i] = fib_nums[i - 1] + fib_nums[i - 2];
        fib_nums[j] = 1;
    }
    
    return fib_nums[n];
}

int main() {
    fib(10);
    return 0;
}
