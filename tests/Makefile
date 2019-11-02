RISCV_GCC_OPTS ?= -DPREALLOCATE=1 -mcmodel=medany -march=rv32i -mabi=ilp32 -static -std=gnu99 -O0 -ffast-math -fno-common -fno-builtin-printf
RISCV_LINK_OPTS ?= -march=rv32i -mabi=ilp32 -static -nostdlib -nostartfiles -lm -lgcc
RISCV_OBJDUMP_OPTS ?= -d

all: fib

.PHONY: check
check:
	@if [ ! -d "./build" ]; then mkdir build; fi

%: %.c check
	riscv32-unknown-elf-gcc crt.s $< $(RISCV_GCC_OPTS) -o build/$@ $(RISCV_LINK_OPTS)
	@riscv32-unknown-elf-objdump $(RISCV_OBJDUMP_OPTS) build/$@ > build/$@.dmp
	@riscv32-unknown-elf-objcopy -O binary build/$@ build/$@.bin
	@hexdump build/$@.bin > build/$@.bin.dmp
	@python tcl.py build/$@.bin

.PHONY: clean
clean:
	rm -rf build