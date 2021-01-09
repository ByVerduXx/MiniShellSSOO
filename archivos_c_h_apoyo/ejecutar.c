#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"



int ** crear_pipes (int nordenes)
{
   int ** pipes = NULL ;
   int i ;
   pipes = ( int **) malloc (sizeof(int*) * (nordenes-1));
   for ( i = 0; i < ( nordenes - 1); i ++)
   {
      pipes [i] = ( int *) malloc ( sizeof ( int ) * 2);

      if(pipe(pipes[i]) == -1)
      {
         perror("Pipe error");
         exit(-1);
      }
   }
   return pipes;
}


pid_t ejecutar_orden(const char *orden,int entrada,int salida, int *pbackgr)
{
   char **args;
   pid_t pid;
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
   int entrada_estandar = dup(0),salida_estandar = dup(1);  //guardamos los descriptores de entrada y salida estandar
   char * nombre;       //copiamos el nombre del archivo de salida en caso de que exista
   
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL)
   {
      return(-1);
   }
   if(indice_ent!=-1)
   {
      redirec_entrada(args,indice_ent,&entrada);  //si hay una redireccion, llama a la funcion
      
   }
   if(indice_sal!=-1){
      nombre = args[indice_sal + 1];   //le asignamos el nombre del archivo de salida
      redirec_salida(args,indice_sal,&salida);  //si hay una redireccion, llama a la funcion
   }
      pid = fork(); //Creamos la minishell hija

      if (pid == 0){ //Si es la minishell hija

         if(entrada!= 0){

            dup2(entrada, STDIN_FILENO);   //cambia la entrada estandar por el archivo
         }

         if(salida!=1)
         {
            dup2(salida,STDOUT_FILENO);   //cambia la salida estandar por el archivo
         }

         execvp(args[0],args);//Ejecuta la orden
         dup2(salida_estandar,STDOUT_FILENO);  //si falla porque no existe la orden vuelve a poner los descriptores de entrada y salida como estaban para 
         dup2(entrada_estandar,STDIN_FILENO);  //escribir el error por pantalla
         printf("Esa orden no existe.\n");
         if(indice_sal){   //si habia redireccion de salida
            remove(nombre); //borra el archivo creado
         }
         exit(-1);

      }
      else{ 
         free_argumentos(args); //Libera la memoria dinamica
         if(salida != 1){
            dup2(salida_estandar, STDOUT_FILENO);  //vuelve a poner la salida estandar
         }
         if(entrada != 0){
            dup2(entrada_estandar,STDIN_FILENO);   //vuelve a poner la entrada estandar
         }
         return pid; //Devuelve el pid del hijo
      }
   
  

   /* Si la linea de ordenes posee tuberias o redirecciones, podra incluir */
   /* aqui, en otras fases de la practica, el codigo para su tratamiento.  */
	

}
 
void ejecutar_linea_ordenes(const char *orden)
{
   pid_t *pids=NULL;
   int backgr;
   char **ordenes ;
   int nordenes;
   int **pipes ;
   int salida,entrada;

   ordenes = parser_pipes(orden,&nordenes);
   pipes = crear_pipes(nordenes);


   for(int i=0;i<(nordenes-1);i++)
   {
      if(i==0)
      {
         if(nordenes>1)
         {
            salida = pipes[0][1];

         }else
         {
            salida = STDOUT_FILENO;
         }
         
         pids[0]=ejecutar_orden(ordenes[0],STDOUT_FILENO,salida,&backgr);

      }
      else if(i==nordenes-1 && nordenes>1)
      {
         entrada=pipes[nordenes-2][0];
         pids[i] = ejecutar_orden(ordenes[i],entrada,STDOUT_FILENO,&backgr);
      }
      else
      {
         entrada=pipes[i-1][0];
         salida = pipes[i][1];
         pids[i] = ejecutar_orden(ordenes[i],entrada,salida,&backgr);

      }


   }


   if (backgr == 0 && pids[nordenes-1]>0)
   { //Si es el padre y es una orden en primer plano espera al hijo
      waitpid(pids[nordenes-1],0,0);
   }
   free(pids);
   free_ordenes_pipes(ordenes,pipes,nordenes);

}   








