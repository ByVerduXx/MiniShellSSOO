CC = gcc
CFLAGS = -g -Wall

libshell.a: parser.o internas.o
	ar -rcs libshell.a parser.o internas.o 

minishell: minishell.o libshell.a entrada_minishell.o ejecutar.o libmemoria.o
	$(CC) $(CFLAGS) minishell.o entrada_minishell.o ejecutar.o libmemoria.o -L./ -lshell -o minishell
	
minishell.o: minishell.c parser.h ejecutar.h libmemoria.h
	$(CC) $(CFLAGS) -c minishell.c
		
entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	$(CC) $(CFLAGS) -c entrada_minishell.c
	
ejecutar.o: ejecutar.c ejecutar.h libshell.a libmemoria.h
	$(CC) $(CFLAGS) -c ejecutar.c -L./ -lshell
	
libmemoria.o: libmemoria.c libmemoria.h	
	$(CC) $(CFLAGS) -c libmemoria.c
	
.PHONY: clean
clean:
	rm -f ejecutar.o 
	rm -f minishell.o
	rm -f libmemoria.o 
	rm -f redirecciones.o 
	rm -f entrada_minishell.o
	rm -f minishell
