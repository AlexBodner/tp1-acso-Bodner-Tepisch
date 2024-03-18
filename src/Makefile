EXTRAS_SRC_FILES = $(filter-out $(wildcard tests*.c), $(wildcard HashMap/*.c), $(wildcard utils/*.c))

sim: shell.c sim.c 
	gcc -g -O0 $^ -o $@ $(EXTRAS_SRC_FILES)

.PHONY: clean
clean:
	rm -rf *.o *~ sim
