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


int main(int argc, char *argv[])
{

   char buf[BUFSIZ];

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
            ejecutar_linea_ordenes(buf);
         }

      } 
   }

   return 0;
}



