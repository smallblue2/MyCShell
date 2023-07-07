# This Makefile compiles the source code in the src/ directory
# and places the executable in the bin/ directory.
#
# To compile the source code, run the command: make compile
# To remove the executable, run the command: make clean

# The .PHONY directive tells make that the targets
# compile and clean are not files.
.PHONY: compile clean

# The compile target compiles the source code in the src/
compile:
	gcc -Wall src/*.c -o bin/mycshell

# The clean target removes the executable from the bin/
clean:
	rm -f bin/mycshell

