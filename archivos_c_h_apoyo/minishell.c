#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"

static void manejar_sigchild(int signo){
   int estado;
   waitpid(-1,&estado,WNOHANG);//Suspende la ejecución del proceso en curso hasta que cualquier proceso hijo ha terminado(-1) y vuelve inmediatamente en caso de que ningún hijo ha terminado(WNOHANG)
}
int main(int argc, char *argv[])
{

   char buf[BUFSIZ];
   struct sigaction sa;//Estructura que se usa para describir una acción a realizar

   memset(&sa,0,sizeof(sa));//Copia el carácter 0 al resultado de la operación sizeof(sa) al carácter sa.
   sa.sa_handler = manejar_sigchild;
   sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;//El flag SA_NOCLDSTOP no genera SIGCHLD cuando el hijo para.
                                           //El flag SA_RESTART afecta al comportamiento de funciones ininterrumpibles de manera que si las interrumpe activa la variable errno.

   sigaction(SIGCHLD,&sa,NULL);//Se utiliza para cambiar la accion realizada por un proceso al recibir una señal especifíca.
                              //SIGCHLD es la señal enviada a un proceso cuando uno de sus procesos hijos termina.
                              //sa estructura sigaction creada anteriormente.
                              
   while (1)
   {
   
	   imprimir_prompt();
      leer_linea_ordenes((char *) buf);
      if (strcmp(buf,"exit") == 0){
         exit(0);
      }
      else{

         if (es_ord_interna(buf))
         {
            ejecutar_ord_interna(buf);
         }
         else{
            if(strstr(buf, ";")!=NULL){ //Comprueba si existe un carácter ";" en el input
               char *token = strtok(buf, ";");//Divide la cadena por el ";" y la almacena en una lista
               while (token!= NULL){//Condición cuando no quedan más órdenes
                  ejecutar_linea_ordenes(token);//Ejecuta la función para cada orden
                  token=strtok(NULL,";");//Pasa a la siguiente orden 
                  printf("\n ");
               }
               
            }
            else{
            ejecutar_linea_ordenes(buf);
            }
         }

      } 
   }

   exit(EXIT_SUCCESS);
}
