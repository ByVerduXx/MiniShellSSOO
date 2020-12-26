#ifndef EJECUTAR_H
#define EJECUTAR_H
#include <sys/wait.h>
pid_t ejecutar_orden(const char *orden, int *pbackgr);
void ejecutar_linea_ordenes(const char *orden);
#endif