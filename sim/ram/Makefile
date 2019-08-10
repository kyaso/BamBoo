top = ram
vsrc = ../../src/$(top).v
simsrc = tb.cpp

verilate: $(vsrc) $(simsrc)
	verilator -Wall --cc $(vsrc) --exe $(simsrc)

compile: verilate
	make -j -C obj_dir -f V$(top).mk V$(top)

run: compile
	./obj_dir/V$(top)
