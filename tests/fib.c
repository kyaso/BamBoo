#define FIB_N 12

/*
void _start() {
    __asm__ (
        "li sp,4096\n"
        "j main"
    );
}
*/

int main() {
    int* result = 0xB00;
    unsigned int fib_i = 1;
    unsigned int fib_ii = 1;
    unsigned int fib = 0;

    int i;
    for(i = 3; i <= FIB_N; i++) {
        fib = fib_i + fib_ii;
        fib_i = fib_ii;
        fib_ii = fib;
    }

    (*result) = fib;

    while(1);
}
