import sys
import numpy as np

# Define name of binary executable to load
filename = sys.argv[1]

# Load binary executable
binary = np.fromfile(filename, dtype='<u4')

start_addr = 0x0

with open(filename+".tcl", 'w') as f:
    f.write("# Create new write transaction\n")
    f.write("reset_hw_axi [get_hw_axis hw_axi_1]\n")
    f.write("create_hw_axi_txn wr_txn [get_hw_axis hw_axi_1] -address "+str(hex(start_addr))+" -data affeaffe -len 1 -type write\n\n")

    # Write instructions to BRAM
    f.write("# Write instructions to BRAM\n")
    for i in range(0,binary.shape[0]):
        f.write("set_property CMD.ADDR "+str(hex(start_addr))+" [get_hw_axi_txns wr_txn]\n")
        f.write("set_property DATA "+"{0:#0{1}x}".format(binary[i],10)+" [get_hw_axi_txns wr_txn]\n")
        #f.write("set_property DATA "+hex(binary[i]).rstrip("L")+" [get_hw_axi_txns wr_txn]\n")
        f.write("run_hw_axi [get_hw_axi_txns wr_txn]\n\n")

        start_addr = start_addr+4
