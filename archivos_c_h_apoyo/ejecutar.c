#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
      return(-1);
   }
   pid = fork(); //Creamos la minishell hija
   if (pid == 0){ //Si es la minishell hija
      execvp(args[0],args);//Ejecuta la orden
      printf("Esa orden no existe.\n");
      exit(-1);

   }
   else{ 
      free_argumentos(args); //Libera la memoria dinamica
      return pid; //Devuelve el pid del hijo
   }

   /* Si la linea de ordenes posee tuberias o redirecciones, podra incluir */
   /* aqui, en otras fases de la practica, el codigo para su tratamiento.  */
	
}
 
void ejecutar_linea_ordenes(const char *orden)
{
   pid_t pid;
   int backgr;


   /* Si la orden es compuesta, podra incluir aqui, en otra fase de la */
   /* practica, el codigo para su tratamiento                          */
 

   pid = ejecutar_orden(orden, &backgr);
   if (backgr == 0 && pid > 0){ //Si es el padre y es una orden en primer plano espera al hijo
      wait(0);
   }
}   
