#ifndef EJECUTAR_H
#define EJECUTAR_H
#include <sys/wait.h>
pid_t ejecutar_orden(const char *orden,int entrada,int salida, int *pbackgr);
void ejecutar_linea_ordenes(const char *orden);
int ** crear_pipes (int nordenes);

#endif