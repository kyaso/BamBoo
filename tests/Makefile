SRC ?= test

BUILD = build/$(SRC)

all: check $(BUILD).bin.tcl

gcc: check $(SRC).c
	riscv32-unknown-elf-gcc $(SRC).c -march=rv32i -mabi=ilp32 -o $(BUILD)
	riscv32-unknown-elf-objdump -d $(BUILD) > $(BUILD).dmp

$(BUILD).s: $(SRC).c
	riscv32-unknown-elf-gcc $(SRC).c -march=rv32i -mabi=ilp32 -S -o $(BUILD).s

$(BUILD): $(BUILD).s
	riscv32-unknown-elf-as $(BUILD).s -march=rv32i -mabi=ilp32 -o $(BUILD)
	riscv32-unknown-elf-objdump -d $(BUILD) > $(BUILD).dmp

$(BUILD).bin: $(BUILD)
	riscv32-unknown-elf-objcopy -O binary $(BUILD) $(BUILD).bin
	hexdump $(BUILD).bin > $(BUILD).bin.dmp

$(BUILD).bin.tcl: $(BUILD).bin
	python tcl.py $(BUILD).bin

check:
	@if [ ! -d "./build" ]; then mkdir build; fi

clean:
	rm -rf build