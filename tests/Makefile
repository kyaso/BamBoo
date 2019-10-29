RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -march=rv32i -mabi=ilp32 -static -std=gnu99 -O0 -ffast-math -fno-common -fno-builtin-printf
RISCV_LINK_OPTS ?= -march=rv32i -mabi=ilp32 -static -nostdlib -nostartfiles -lm -lgcc
RISCV_OBJDUMP_OPTS ?= -d

all: fib

check:
	@if [ ! -d "./build" ]; then mkdir build; fi

fib: check
	riscv32-unknown-elf-gcc crt.s fib.c $(RISCV_GCC_OPTS) -o build/fib $(RISCV_LINK_OPTS)
	@riscv32-unknown-elf-objdump $(RISCV_OBJDUMP_OPTS) build/fib > build/fib.dmp
	@riscv32-unknown-elf-objcopy -O binary build/fib build/fib.bin
	@hexdump build/fib.bin > build/fib.bin.dmp
	@python tcl.py build/fib.bin

clean:
	rm -rf build