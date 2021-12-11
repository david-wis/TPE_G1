/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 *  Funciones auxiliares de asignación y reasignación de memoria con control de errores de falta de memoria
 */

#ifndef TPE_G1_UTILS_H
#define TPE_G1_UTILS_H
#include <stdio.h>

/*!
 * Version de malloc que aborta si no hay memoria suficiente
 * @param bytes Cantidad de bytes a los que se pretende asignar espacio en el heap
 * @return Puntero a dirección de memoria asignada
 */
void * safeMalloc(size_t bytes);

/*! Version de calloc que aborta si no hay memoria suficiente
 * @param size Cantidad de elementos
 * @param bytes Cantidad de bytes de cada elemento a los que se pretende asignar espacio en el heap
 * @return Puntero a direccion de memoria asignada
 */
void * safeCalloc(size_t size, size_t bytes);

/*!
 * Version de realloc que aborta si no hay memoria suficiente
 * @param p Direccion de memoria que se desea reasignar
 * @param bytes Cantidad de bytes a los que se pretende asignar espacio en el heap
 * @return Puntero a direccion de memoria asignada
 */
void * safeRealloc(void * p, size_t bytes);

#endif //TPE_G1_UTILS_H
