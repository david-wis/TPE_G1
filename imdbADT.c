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

#include "imdbADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "title.h"
#define ERR_MSG_OOB "Out of bounds\n"   // Mensaje de error en caso de que se quiera acceder a un nodo inválido
#define MAX_MOVIE_COUNT 5               // Cantidad de películas del top
#define EMPTY "\\N"                     // Cadena que representa valor nulo en los archivos
#define EMPTY_LEN 3                     // Longitud de la cadena que representa valor nulo

typedef struct tMovieNode * tMovieList;

typedef struct tMovieNode { // Nodo que contiene la información de cada película que llegue al top 5
    char * film;            // Nombre de la película
    unsigned long votes;    // Cantidad de votos
    float rating;           // Rating de la película
    unsigned int genres;    // Conjunto de 32 bits que representan la presencia de cada género
    tMovieList nextM;       // Puntero al siguiente nodo
} tMovieNode;

typedef struct tYearNode * tYearList;

typedef struct tYearNode {          // Nodo que contiene la información de un año
    unsigned short year;            // Año
    unsigned long qtyFilms;         // Cantidad de Películas
    unsigned long qtySeries;        // Cantidad de series en el año
    unsigned long qtyShorts;        // Cantidad de cortos en el año
    unsigned long *qtyByGenre[2];   // Cantidad de títulos por tipo y género, columna es género, la primera fila son las películas, la segunda series
    char topSize;                   // Cantidad de elementos de la lista de películas
    tMovieList firstM;              // Primer nodo del top 5 de películas
    tMovieList iterMovie;           // Iterador de películas
    tYearList nextY;                // Puntero al siguiente año
} tYearNode;

typedef struct imdbCDT {
    unsigned char genreDim;         // Cantidad de géneros
    tYearList firstY;               // Puntero al primer año
    tYearList iterYear;             // Iterador por años
} imdbCDT;


/*!
 * Chequea que el iterador de años (y el de top movies) se encuentren dentro del rango
 * @param imdb ADT de imdb
 * @param checkIterMovie 0 si solo se quiere chequear años, 1 si tambien películas
 */
static void checkIterInBounds(imdbADT imdb, int checkIterMovie);

/*!
 * Libera recursivamente los recursos asignados a la lista de películas
 * @param movie Lista de películas
 */
static void freeMoviesRec(tMovieList movie);

/*!
 * Libera recursivamente los recursos asignados a la lista de años
 * @param year Lista de años
 */
static void freeYearsRec(tYearList year);

/*!
 * Guarda la información relacionada con la query 1
 * @param year Año de incidencia
 * @param title información del titulo a registrar
 */
static void loadTitleByYear(tYearList year, tTitle * title);//Query 1

/*!
 * Guarda la información relacionada con la query 2
 * @param year Año de incidencia
 * @param title Información del título a registrar
 * @param genreDim Cantidad de géneros
 */
static void loadTitleByTypeGenre(tYearList year, tTitle * title, unsigned char genreDim);//Query 2

/*!
 * Guarda la información relacionada con la query 3
 * @param year Año de incidencia
 * @param title información del titulo a registrar
 */
static void loadTopMovie(tYearList year, tTitle * title);//Query 3

/*!
 * Se fija si puede agregar recursivamente un año en la lista. Si ya existe, no lo agrega. Devuelve por parámetro de salida un puntero a ese año
 * @param node Lista de años
 * @param year Numero del año
 * @param result Puntero al año encontrado/creado
 * @param genreDim Dimension del vector de géneros
 * @return Primer elemento de la lista de años
 */
static tYearList insertYearRec(tYearList node, unsigned short year, tYearList * result, unsigned char genreDim);

/*!
 * Compara peliculas
 * @param title Titulo a comparar
 * @param movie Nodo del top 5 peliculas de cada año
 * @return > 0 si title es mayor, 0 si son iguales, < 0 si es title menor
 */
static int compareMovies(tTitle * title, tMovieNode * movie);

/*!
 * Agrega una pelicula a la lista de peliculas en orden ascendente
 * @param list Lista de años
 * @param title Titulo a agregar
 * @return Primer elemento de la lista
 */
static tMovieList addTopMovieRec(tMovieList list, tTitle * title);

imdbADT newImdbADT(unsigned char genreDim) {
    imdbADT imdb = safeMalloc(sizeof(imdbCDT));
    imdb->genreDim = genreDim;
    imdb->firstY = NULL;
    imdb->iterYear = NULL;
    return imdb;
}

static void checkIterInBounds(imdbADT imdb, int checkIterMovie) {
    if(imdb->iterYear == NULL || (checkIterMovie && imdb->iterYear->iterMovie == NULL)){
        fprintf(stderr, ERR_MSG_OOB);
        exit(1);
    }
}

void toBeginMovie(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    imdb->iterYear->iterMovie = imdb->iterYear->firstM;
}

int hasNextMovie(imdbADT imdb) {
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->iterMovie != NULL;
}

void nextMovie(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    imdb->iterYear->iterMovie = imdb->iterYear->iterMovie->nextM;
}

char * getCurrentMovieTitle(imdbADT imdb) {
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->film;
}

unsigned long getCurrentMovieVotes(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->votes;
}

float getCurrentMovieRating(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->rating;
}

char * getCurrentMovieGenres(imdbADT imdb, char * vecGenres[]){
    checkIterInBounds(imdb, 1);
    char * s = NULL;
    size_t len = 0;
    size_t wordlen;
    unsigned int genres = imdb->iterYear->iterMovie->genres;
    if (!genres) {
        s = safeMalloc(EMPTY_LEN); // Pone el \N
        return strcpy(s, EMPTY);
    }
    for (int i = 0; i < imdb->genreDim && genres; i++, genres >>= 1) {
        if (genres & 1) { // Se fija si el título pertenece al i-ésimo género
            wordlen = strlen(vecGenres[i]);
            s = safeRealloc(s, len + wordlen + 1);
            strcpy(s + len, vecGenres[i]); // Concatena el genero
            s[len + wordlen] = ',';
            len += (wordlen + 1); // Longitud de la palabra mas la , o el \0
        }
    }
    s[len-1] = 0;
    return s;
}

static void freeMoviesRec(tMovieList movie) {
    if (movie == NULL)
        return;
    freeMoviesRec(movie->nextM);
    free(movie->film);
    free(movie);
}

static void freeYearsRec(tYearList year){
    if(year == NULL)
        return;
    freeMoviesRec(year->firstM);
    free(year->qtyByGenre[0]);
    free(year->qtyByGenre[1]);
    freeYearsRec(year->nextY);
    free(year);
}

void freeImdb(imdbADT imdb) {
    freeYearsRec(imdb->firstY);
    free(imdb);
}

void toBeginYear(imdbADT imdb){
    imdb->iterYear = imdb->firstY;
}

int hasNextYear(imdbADT imdb){
    return imdb->iterYear != NULL;
}

void nextYear(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    imdb->iterYear = imdb->iterYear->nextY;
}

unsigned int getCurrentYear(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->year;
}

unsigned long getQtyFilms(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyFilms;
}

unsigned long getQtyShorts(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyShorts;
}

unsigned long getQtySeries(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtySeries;
}

unsigned long ** getQtyByGenresByYear(imdbADT imdb) {
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyByGenre;
}


static void loadTitleByYear(tYearList year, tTitle * title) { // QUERY 1
    switch (title->titleType) {
        case MOVIE:
            year->qtyFilms++;   // Incrementa la cantidad de películas del año
            break;
        case SHORT:
            year->qtyShorts++;  // Incrementa la cantidad de cortos del año
            break;
        case TVMINISERIES:
        case TVSERIES:
            year->qtySeries++;  // Incrementa la cantidad de series del año
            break;
        default:
            break;
    }
}

static void loadTitleByTypeGenre(tYearList year, tTitle * title, unsigned char genreDim) { // QUERY 2
    if(title->titleType == MOVIE || title->titleType == TVMINISERIES || title->titleType == TVSERIES) {
        int idx = title->titleType != MOVIE; // La matriz tiene en la fila 0 las películas y la 1 las series
        unsigned int genres = title->genres;
        for (int i = 0; i < genreDim && genres; i++, genres >>= 1) {
            if (genres & 1) { // Se fija si el título pertenece al i-ésimo género
                year->qtyByGenre[idx][i]++;
            }
        }
    }
}

static int compareMovies(tTitle * title, tMovieNode * movie) {
    int aux = (title->numVotes > movie->votes) - (title->numVotes < movie->votes);
    if (!aux) {
        aux = strcmp(movie->film, title->primaryTitle);
    }
    return aux;
}

static tMovieList addTopMovieRec(tMovieList list, tTitle * title) {
    int cmp;
    if (list == NULL || (cmp = compareMovies(title, list)) <= 0) {
        tMovieList m = safeMalloc(sizeof(tMovieNode));
        m->film = safeMalloc(strlen(title->primaryTitle) + 1);
        strcpy(m->film, title->primaryTitle);
        m->votes = title->numVotes;
        m->genres = title->genres;
        m->rating = title->avgRating;
        m->nextM = list;
        return m;
    }
    if (cmp > 0){
        list->nextM = addTopMovieRec(list->nextM, title);
    }
    return list;
}

static void loadTopMovie(tYearList year, tTitle * title) { //QUERY 3
    if (year->topSize < MAX_MOVIE_COUNT) { // Si hay menos de 5 películas, se guarda siempre como maximo
        year->firstM = addTopMovieRec(year->firstM, title);
        year->topSize++;
    } else if (compareMovies(title, year->firstM) > 0) { // Si hay 5 y es mayor al primero, se borra el menor y se guarda
        tMovieList aux = year->firstM->nextM;
        free(year->firstM->film);
        free(year->firstM);
        year->firstM = aux;
        year->firstM = addTopMovieRec(year->firstM, title);
    }
}

static tYearList insertYearRec(tYearList node, unsigned short year, tYearList * result, unsigned char genreDim) {
    if (node == NULL || node->year < year) {
        tYearList new = safeMalloc(sizeof(tYearNode));
        new->year = year;
        new->firstM = NULL;
        new->qtyByGenre[0] = safeCalloc(genreDim, sizeof(unsigned long)); // Fila para peliculas
        new->qtyByGenre[1] = safeCalloc(genreDim, sizeof(unsigned long)); // Fila para series
        new->qtyFilms = new->qtySeries = new->qtyShorts = 0;              // Todos los contadores arrancan en cero
        new->nextY = node;
        new->topSize = 0;                                                 // Comienza sin películas
        new->iterMovie = NULL;
        *result = new;
        return new;
    }

    if (node->year > year) {
        node->nextY = insertYearRec(node->nextY, year, result, genreDim);
    } else {
        *result = node;
    }
    return node;
}

void loadData(imdbADT imdb, tTitle * title) {
    tYearList pCurrYear;
    imdb->firstY = insertYearRec(imdb->firstY, title->startYear, &pCurrYear, imdb->genreDim); // Arma la lista de años

    loadTitleByYear(pCurrYear, title);
    loadTitleByTypeGenre(pCurrYear, title, imdb->genreDim);
    if (title->titleType == MOVIE) {
        loadTopMovie(pCurrYear, title);
    }
}
