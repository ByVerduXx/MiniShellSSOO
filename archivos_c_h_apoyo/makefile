CC = gcc
CFLAGS = -g -Wall

minishell: minishell.o internas.o parser.o entrada_minishell.o ejecutar.o libmemoria.o
	$(CC) $(CFLAGS) minishell.o internas.o parser.o entrada_minishell.o ejecutar.o libmemoria.o -o minishell
	
minishell.o: minishell.c parser.h ejecutar.h libmemoria.h
	$(CC) $(CFLAGS) -c minishell.c
	
entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	$(CC) $(CFLAGS) -c entrada_minishell.c
	
ejecutar.o: ejecutar.c ejecutar.h parser.h libmemoria.h
	$(CC) $(CFLAGS) -c ejecutar.c
	
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
