
all: lzjag

ERRHAND.o: ERRHAND.C makefile
	gcc $< -c -Ofast -o $@

LZSS.o: LZSS.C makefile
	gcc $< -c -Ofast -o $@

BITIO.o: BITIO.C makefile
	gcc $< -c -Ofast -o $@
	
MAIN.o: MAIN.C makefile
	gcc $< -c -Ofast -o $@

OBJS = ERRHAND.o LZSS.o BITIO.o MAIN.o

lzjag:	ERRHAND.c LZSS.c BITIO.c MAIN.c $(OBJS) makefile
	gcc -Ofast -o $@ ERRHAND.c BITIO.c MAIN.c LZSS.c
 
#lzjag: $(OBJ) makefile
#	ld -o $@ -lc

clean:
	rm $(OBJS) lzjag
