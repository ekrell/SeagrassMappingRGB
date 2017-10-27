filename = uwcProc_driver.cpp
filenameLib = uwcProc.cpp

all: compile run

compile: $(filename)
	g++ -ggdb `pkg-config --cflags opencv` -o `basename $(filename) .cpp` $(filename) $(filenameLib) `pkg-config --libs opencv`

run:
	./`basename $(filename) .cpp`
