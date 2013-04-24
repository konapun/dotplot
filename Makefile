all: genplot
	
genplot: generate_dotplot.c dotplot.o
	gcc -o genplot dotplot.o generate_dotplot.c -lgd

dotplot.o: dotplot.c dotplot.h
	gcc -c dotplot.c -lgd
	
clean:
	rm genplot *.o

