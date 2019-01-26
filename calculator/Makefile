objs = main.o expr.o queue.o stack.o

cal : $(objs)
	cc -o cal $(objs)

.PHONY : clean
clean:
	-rm cal $(objs)
