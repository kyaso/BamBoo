SRC ?= test

BUILD = build/$(SRC)

all: asm bin tcl

gcc: check $(SRC).c
	riscv32-unknown-elf-gcc $(SRC).c -march=rv32i -mabi=ilp32 -o $(BUILD)
	riscv32-unknown-elf-objdump -d $(BUILD) > $(BUILD).dmp

asm: check $(SRC).s
	riscv32-unknown-elf-as $(SRC).s -march=rv32i -mabi=ilp32 -o $(BUILD)
	riscv32-unknown-elf-objdump -d $(BUILD) > $(BUILD).dmp

bin: asm
	riscv32-unknown-elf-objcopy -O binary $(BUILD) $(BUILD).bin
	hexdump $(BUILD).bin > $(BUILD).bin.dmp

tcl: bin
	python tcl.py $(BUILD).bin

check:
	@if [ ! -d "./build" ]; then mkdir build; fi

clean:
	rm -rf build