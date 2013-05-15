objects = virtualmem.o algo.o

virtualmem: $(objects)
	g++ -o virtualmem $(objects)

virtualmem.o: virtualmem.h algo.h

algo.o: algo.h virtualmem.h


.PHONY: clean
clean:
	-rm edit $(objects)
