obj = main.o control.o set.o draw.o

snake: $(obj)
	cc -o snake $(obj) -lcurses

.PHONY : clean
clean:
	-rm snake $(obj)
