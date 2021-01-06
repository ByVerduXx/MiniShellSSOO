#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>


#include "redirecciones.h"


/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{
   

    *entrada = open(args[indice_entrada+1],O_RDONLY,0777);

    if(*entrada == -1)
    {
        perror("Error al abrir el archivo");
    }
    else{

        args[indice_entrada] = '\0';
        args[indice_entrada+1]= '\0';
    }



}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida)
{
   
    *salida = open(args[indice_salida+1],O_WRONLY | O_TRUNC,0777);
    if(*salida == -1)
    {
        *salida = open(args[indice_salida+1],O_CREAT | O_WRONLY,0777);
    }

    args[indice_salida] = '\0';
    args[indice_salida+1]= '\0';



}

