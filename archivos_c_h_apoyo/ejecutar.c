#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"


pid_t ejecutar_orden(const char *orden, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1,entrada = 0,salida = 1; /* por defecto, no hay < ni > */
   
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
      return(-1);
   }
   if(indice_ent!=-1)
   {
      redirec_entrada(args,indice_ent,&entrada);
      
   }
   if(indice_sal!=-1){

      redirec_salida(args,indice_sal,&salida);

   }
   

      pid = fork(); //Creamos la minishell hija

      if (pid == 0){ //Si es la minishell hija

         if(entrada!= 0){

            dup2(entrada, STDIN_FILENO);
         }

         if(salida!=1)
         {
            dup2(salida,STDOUT_FILENO);
         }

         execvp(args[0],args);//Ejecuta la orden
         dup2(0,STDIN_FILENO);   //si falla porque no existe la orden vuelve a poner los descriptores de entrada y salida como estaban para 
         dup2(1,STDOUT_FILENO);  //escribir el error por pantalla
         printf("Esa orden no existe.\n");
         exit(-1);

      }
      else{ 
         free_argumentos(args); //Libera la memoria dinamica
         if(salida != 1){
            dup2(1,STDOUT_FILENO);
         }
         if(entrada != 0){
            dup2(0,STDIN_FILENO);
         }
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
