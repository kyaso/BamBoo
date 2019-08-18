top = top
vsrc = ../../src/$(top).v ../../src/cpu_top.v ../../src/ram.v ../../src/alu.v ../../src/bitselect.v ../../src/decoder.v ../../src/execute.v ../../src/fetch.v ../../src/regfile.v
simsrc = tb_$(top).cpp

verilate: $(vsrc) $(simsrc)
	verilator -Wall --cc $(vsrc) --exe $(simsrc)

compile: verilate
	make -j -C obj_dir -f V$(top).mk V$(top)

run: compile
	./obj_dir/V$(top)

clean:
	rm -rf obj_dir
