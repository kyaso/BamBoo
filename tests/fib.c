unsigned int fib(int n) {
    unsigned int fib_i = 1;
    unsigned int fib_ii = 1;
    unsigned int fib = 0;

    if(n == 1) return 1;
    if(n == 2) return 1;

    int i;
    for(i = 3; i <= n; i++) {
        fib = fib_i + fib_ii;
        fib_i = fib_ii;
        fib_ii = fib;
    }

    return fib;
}

int main() {
    fib(10);

    while(1);
}