/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 * Estructura que almacena la información de un título
 * Ofrece una función para liberar la memoria asignada para un título
 * Cuenta con un enum de los tipos de títulos
 */

#include "title.h"
#include <stdlib.h>

void freeTitle(tTitle * t) {
    if (t != NULL) {
        free(t->id);
        free(t->primaryTitle);
    }
    free(t);
}
