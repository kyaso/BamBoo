SRC ?= test

all: check $(SRC).c
	riscv32-unknown-elf-gcc $(SRC).c -march=rv32i -mabi=ilp32 -o build/$(SRC)
	riscv32-unknown-elf-objdump -d build/$(SRC) > build/$(SRC).dmp

asm: check $(SRC).s
	riscv32-unknown-elf-as $(SRC).s -march=rv32i -mabi=ilp32 -o build/$(SRC)
	riscv32-unknown-elf-objdump -d build/$(SRC) > build/$(SRC).dmp

check:
	@if [ ! -d "./build" ]; then mkdir build; fi

clean:
	rm -rf build