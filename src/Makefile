EXTRAS_SRC_FILES = $(filter-out $(wildcard tests*.c), $(wildcard HashMap/*.c))

sim: shell.c sim.c 
	gcc -g -O0 $^ -o $@ $(EXTRAS_SRC_FILES) utils/mathOps.c utils/shift.c utils/branch.c utils/load_store.c

.PHONY: clean
clean:
	rm -rf *.o *~ sim 
