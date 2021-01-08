#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>


#include "redirecciones.h"


/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada) //**args: argumentos introducidos por teclado en la minishell; indice_entrada: "<"; *entrada: descriptor de archivo
{
   

    *entrada = open(args[indice_entrada+1],O_RDONLY,0777);

    if(*entrada == -1) //Comprueba si hay un error en la apertura del archivo
    {
        perror("Error al abrir el archivo");
    }
    else{

        args[indice_entrada] = '\0';	//fija el indice de entrada "<" a NULL para no tomarlo como argumento
        args[indice_entrada+1]= '\0';	//fija el nombre del archivo a NULL para no tomarlo como argumento
    }



}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida)	//**args: argumentos introducidos por teclado en la minishell; indice_salida: ">"; *salida: descriptor de archivo
{
   
    *salida = open(args[indice_salida+1],O_TRUNC | O_WRONLY,0777); //Si ya existe el archivo de salida borra su información para reescribirla y le asigna todos los permisos
    if(*salida == -1)							//Si no existe el archivo de salida 
    {
        *salida = open(args[indice_salida+1],O_CREAT | O_WRONLY,0777);	//Lo crea y le asigna todos los permisos
    }

    args[indice_salida] = '\0'; //fija el indice de salida ">" a NULL para no tomarlo como argumento
    args[indice_salida+1]= '\0'; //fija el nombre del archivo a NULL para no tomarlo como argumento



}

