#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"



int ** crear_pipes (int nordenes)
{
   int ** pipes = NULL ;
   int i ;
   pipes = ( int **) malloc (sizeof(int*) * (nordenes-1));  //reserva memoria para tantas tuberias como nordenes-1 haya
   for ( i = 0; i < ( nordenes - 1); i ++)
   {
      pipes [i] = ( int *) malloc ( sizeof ( int ) * 2); //reserva memoria para cada tuberia (de la forma int pipe[2])

      if(pipe(pipes[i]) == -1)         //crea la tuberia
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
   char * nombre;       //nombre del archivo de salida en caso de que exista
   int fd;     //comprobador de la existencia de un archivo
   
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
      fd = open(nombre, O_RDONLY);     //intentamos abrir el archivo y guardamos su descriptor
      redirec_salida(args,indice_sal,&salida);  //si hay una redireccion, llama a la funcion
   }
      pid = fork(); //Creamos la minishell hija

      if (pid == 0){ //Si es la minishell hija

         if(entrada!= 0){

            dup2(entrada, STDIN_FILENO);   //cambia la entrada estandar por el archivo
            close(entrada);
         }

         if(salida!=1)
         {
            dup2(salida,STDOUT_FILENO);   //cambia la salida estandar por el archivo
            close(salida);
         }

         execvp(args[0],args);//Ejecuta la orden
         dup2(salida_estandar,STDOUT_FILENO);  //si falla porque no existe la orden vuelve a poner los descriptores de entrada y salida como estaban para 
         dup2(entrada_estandar,STDIN_FILENO);  //escribir el error por pantalla
         printf("Esa orden no existe.\n");
         if(indice_sal && fd < 0){   //si habia redireccion de salida y el archivo no estaba creado anteriormente, lo borra
            remove(nombre); //borra el archivo creado
         }
         else{close(fd);}
         exit(-1);

      }
      else{ 
         free_argumentos(args); //Libera la memoria dinamica
         if(salida != STDOUT_FILENO){
            close(salida);
         }
         if(entrada != STDIN_FILENO){
            close(entrada);
         }
         return pid; //Devuelve el pid del hijo
      }
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
   pids = (int *)malloc(sizeof(int)* nordenes);


   for(int i=0;i<(nordenes);i++)
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
         
         pids[0]=ejecutar_orden(ordenes[0],STDIN_FILENO,salida,&backgr);

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
      waitpid(pids[nordenes-1],NULL,0);
   }
   free(pids);
   free_ordenes_pipes(ordenes,pipes,nordenes);

}   








