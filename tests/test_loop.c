int main() {
    char *addr = 100;
    for(int i = 0; i < 100; i++) {
        (*addr) = i;
        addr++;
    }

    while(1);
}