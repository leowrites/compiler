// Test while loops

int x;
int arr[10];

int main() {
    int i;
    while (true) {
        if (i > 9)
            break;
        i = i + 1;
        arr[i] = i;
        if (i / 2 == 0) {
            x = 3;
            while (x > 0) {
                arr[i] = arr[i] * x;
                x = x - 1;
            }
        }
    }
    return 0;
}
