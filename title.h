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

#ifndef TPE_G1_TITLE_H
#define TPE_G1_TITLE_H

// Enum con los 4 tipos de títulos disponibles
typedef enum { MOVIE = 0, SHORT, TVMINISERIES, TVSERIES } tTitleType;

typedef struct tTitle{
    char * id;                      // Identificador único de un título
    char * primaryTitle;            // Título
    tTitleType titleType;           // Tipo de título
    unsigned int genres;            // Se usa cada uno de los 32 bits para un género
    unsigned int runtimeMinutes;    // Cantidad de minutos
    unsigned short startYear;       // Año de comienzo
    unsigned short endYear;         // Año de finalización
    unsigned long numVotes;         // Cantidad de votos
    float avgRating;                // Promedio de rating
} tTitle;

/*!
 * Libera el titulo
 * @param t Puntero a título
 */
void freeTitle(tTitle * t);

#endif //TPE_G1_TITLE_H
