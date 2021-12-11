/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 * ADT encargado de computar las estadísticas de títulos de imdb
 * Lleva un registro anual de las siguientes estadísticas:
 * Query 1: Cantidad de películas, cortos y series
 * Query 2: Cantidad de series y películas por género
 * Query 3: Las 5 películas con mayor cantidad de votos
 * Ofrece un iterador de años y dentro de cada uno de ellos, un iterador del top de las películas
 */

#ifndef TPE_G1_IMDBADT_H
#define TPE_G1_IMDBADT_H
#include <stdlib.h>
#include "title.h"

typedef struct imdbCDT * imdbADT;

/*!
 * Inicializa el ADT de imdb
 * @param genreDim Cantidad de géneros
 * @return ADT de imdb
 */
imdbADT newImdbADT(unsigned char genreDim);

/*!
 * Arma una lista de años y registra las estadísticas de los títulos que se ingresan
 * @param imdb ADT de imdb
 * @param title Estructura que contiene información sobre un título
 */
void loadData(imdbADT imdb, tTitle * title);


/*!
 * Libera los recursos asignados al ADT
 * @param imdb ADT de imdb
 */
void freeImdb(imdbADT imdb);


/*!
 * Comienza a iterar desde el mayor año cargado
 * @param imdb ADT de imdb
 */
void toBeginYear(imdbADT imdb);


/*!
 * Verifica si quedan años por iterar
 * @param imdb ADT del imdb
 * @return 0 si ya no se puede iterar, 1 si todavia quedan años
 */
int hasNextYear(imdbADT imdb);

/*!
 * Hace que el ADT apunte al siguiente año
 * Advertencia: la funcion es void. Para un correcto encapsulamiento, se deben utilizar getters y setters
 * al obtener los atributos del año
 * @param imdb ADT de imbd
 */
void nextYear(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return La cantidad de películas del año actual en el iterador
 */
unsigned long getQtyFilms(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return La cantidad de cortos del año actual en el iterador
 */
unsigned long getQtyShorts(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return La cantidad de series del año actual en el iterador
 */
unsigned long getQtySeries(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return Matriz de 2xN siendo N la cantidad de géneros que existen. Almacena la cantidad de títulos según tipo y género del año actual en el iterador.
 *      Donde cada columna representa un género y la primera fila la cantidad de películas y la segunda de series.
 */
unsigned long ** getQtyByGenresByYear(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return El numero de año actual
 */
unsigned int getCurrentYear(imdbADT imdb);

/*!
 * Comienza a iterar desde la primera pelicula del año que le corresponde
 * @param imdb ADT de imdb
 */
void toBeginMovie(imdbADT imdb);

/*!
 * Verifica si quedan pel´íulas por iterar en el año correspondiente
 * @param imdb ADT del imdb
 * @return 0 si ya no se puede iterar, 1 si todavía quedan años
 */
int hasNextMovie(imdbADT imdb);

/*!
 * Hace que el ADT apunte a la siguiente película de un año
 * Advertencia: la función es void. Para un correcto encapsulamiento, se deben utilizar getters y setters
 * al obtener los atributos de la película
 * @param imdb ADT de imbd
 */
void nextMovie(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return String con el titulo de la película actual
 */
char * getCurrentMovieTitle(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return Cantidad de votos de la película actual
 */
unsigned long getCurrentMovieVotes(imdbADT imdb);

/*!
 * @param imdb ADT de imdb
 * @return Numero de raiting de la película actual
 */
float getCurrentMovieRating(imdbADT imdb);

/*!
 * Devuelve los nombres de los géneros de una película separados por comas
 * @param imdb ADT de imdb
 * @param vecGenres Vector de géneros
 * @return Nombres de los géneros de la película actual separados por comas
 */
char * getCurrentMovieGenres(imdbADT imdb, char * vecGenres[]);

#endif //TPE_G1_IMDBADT_H
