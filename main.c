/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 *  Programa principal. Utiliza las librerías "imdb" y "csv" creadas por los alumnos
 *  para procesar dos archivos de texto que contienen géneros de títulos de imdb e información
 *  sobre dichos títulos. Luego imprime ciertas estadísticas establecidas por la cátedra en
 *  tres archivos csv llamados query1.csv, query2.csv y query3.csv
 *
 */

#include <stdio.h>
#include "csvADT.h"
#include "imdbADT.h"

#define QTY_GENRES 32                                   // Cantidad maxima de géneros que se van a leer
#define WRONG_PARAMS "Two arguments were expected\n"    // Mensaje de error en caso de no haber enviado los dos archivos como parámetro
#define QUERY1_HEADERS "year;films;series;shorts"       // Headers de los archivos de queries
#define QUERY2_HEADERS "year;genre;films;series"
#define QUERY3_HEADERS "year;film;votes;rating;genres"
#define QUERY1_FILE "query1.csv"                        // Nombres de los archivos de queries
#define QUERY2_FILE "query2.csv"
#define QUERY3_FILE "query3.csv"
#define QTY_PARAMS 3                                    // Cantidad de parámetros esperados, incluyendo el nombre del ejecutable


/*!
 * Carga los géneros en el vector de géneros a partir de un archivo
 * @param fileName Nombre del archivo de géneros
 * @param vecGenres Vector de géneros
 * @param genreDim Puntero a la cantidad de géneros
 */
void loadGenres(char * fileName, char * vecGenres[], unsigned char * genreDim);

/*!
 * Libera los punteros a cada genero
 * @param genres Vector de géneros
 * @param genreDim Cantidad de géneros
 */
void freeGenres(char ** genres, unsigned char genreDim);

/*!
 * Escribe de manera recursiva el top de películas de un año
 * @param imdb ADT del imdb
 * @param year Año de registro a escribir
 * @param csv Archivo donde se escribe la query3
 * @param vecGenres Vector de géneros
 */
void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv, char ** vecGenres);

/*!
 * Escribe las estadísticas en los archivos query1.csv, query2.csv y query3.csv
 * @param imdb ADT del imdb
 * @param genreDim Cantidad de géneros
 * @param genres Vector de géneros
 */
void writeResults(imdbADT imdb, unsigned char genreDim, char ** genres);


int main(int argc, char * argv[]) {
    if (argc < QTY_PARAMS) { // Produce un mensaje de error si la cantidad de parámetros que recibe es incorrecta
        fprintf(stderr, WRONG_PARAMS);
        exit(1);
    }

    char * titleTypes[] = {"movie", "short", "tvMiniSeries", "tvSeries"}; // Vector de tipos de título
    unsigned char genreDim;
    char * vecGenres[QTY_GENRES];
    loadGenres(argv[2], vecGenres, &genreDim);

    imdbADT imdb = newImdbADT(genreDim);
    csvADT csvTitles = newCsv(argv[1], READ);

    tTitle * title;
    size_t typesDim = sizeof(titleTypes)/sizeof(char*);
    while (!eof(csvTitles)) { // Mientras queden títulos por leer
        title = readNextTitle(csvTitles, vecGenres, genreDim, titleTypes, typesDim);
        if (title != NULL) {
            loadData(imdb, title); // Se cargan las estadísticas del título
            freeTitle(title);
        }
    }
    closeFile(csvTitles);
    writeResults(imdb, genreDim, vecGenres);
    freeImdb(imdb);
    freeGenres(vecGenres, genreDim);

    return 0;
}

void loadGenres(char * fileName, char * vecGenres[], unsigned char * genreDim) {
    csvADT genresCsv = newCsv(fileName, READ);
    int i = 0;
    char * s;
    while (i < QTY_GENRES && !eof(genresCsv)) { // Lee a lo sumo 32 géneros
        if ((s = readNextString(genresCsv)) != NULL) {
            vecGenres[i++] = s;
        }
    }
    *genreDim = i;
    closeFile(genresCsv);
}

void freeGenres(char ** genres, unsigned char genreDim){
    for(int i = 0; i < genreDim; i++) {
        free(genres[i]);
    }
}

void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv, char ** vecGenres) {
    if (!hasNextMovie(imdb)){
        return;
    }
    char * title = getCurrentMovieTitle(imdb);
    unsigned long votes = getCurrentMovieVotes(imdb);
    float rating = getCurrentMovieRating(imdb);
    char * genres = getCurrentMovieGenres(imdb, vecGenres);
    nextMovie(imdb);
    writeMoviesRec(imdb, year, csv, vecGenres); // Llamada recursiva de pila
    writeQuery3(csv, year, title, votes, rating, genres); // Se van a escribir en orden inverso
    free(genres);
}

void writeResults(imdbADT imdb, unsigned char genreDim, char ** genres){
    toBeginYear(imdb);
    csvADT query1File = newCsv(QUERY1_FILE, WRITE); // Crea un lector de csv por cada query
    csvADT query2File = newCsv(QUERY2_FILE, WRITE);
    csvADT query3File = newCsv(QUERY3_FILE, WRITE);
    writeString(query1File, QUERY1_HEADERS); // Escribe el encabezado en cada archivo de salida
    writeString(query2File, QUERY2_HEADERS);
    writeString(query3File, QUERY3_HEADERS);
    while (hasNextYear(imdb)){
        unsigned short year = getCurrentYear(imdb);
        //Query 1
        writeQuery1(query1File, year, getQtyFilms(imdb), getQtySeries(imdb), getQtyShorts(imdb));

        //Query 2
        writeQuery2(query2File, year, genreDim, genres, getQtyByGenresByYear(imdb));

        // Query 3
        toBeginMovie(imdb);
        writeMoviesRec(imdb, year, query3File, genres);
        nextYear(imdb);
    }
    closeFile(query1File); // Cierra los lectores de cada query
    closeFile(query2File);
    closeFile(query3File);
}

