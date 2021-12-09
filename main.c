#include <stdio.h>
#include <stdlib.h>
#include "csvADT.h"
#include "imdbADT.h"

#define QTY_GENRES 32
#define WRONG_PARAMS "Two arguments were expected"
#define READ "r"
#define WRITE "w"
#define QUERY1_HEADERS "year;films;series;shorts"

void loadGenres(char * fileName, char * vecGenres[], char * genreDim);
void writeResults(imdbADT imdb);

int main(int argc, char * argv[]) {
    #ifdef RELEASE
    if (argc != 2) {
        fprintf(stderr, WRONG_PARAMS);
        exit(1);
    }
    #endif

    char * titleTypes[] = {"movie", "short", "tvMiniSeries", "tvSeries"};
    char genreDim;
    char * vecGenres[QTY_GENRES];
    loadGenres("/Users/mbp13/Desktop/PI/TPE_G1/genres.csv", vecGenres, &genreDim); //TODO: Ponerlo con argv

    imdbADT imdb = newImdbADT(vecGenres, genreDim);
    csvADT csvTitles = newCsv("/Users/mbp13/Desktop/PI/TPE_G1/imdb.csv", READ); //TODO: Ponerlo con argv
    tTitle * title;
    while (!eof(csvTitles)) {
        title = readNextTitle(csvTitles, vecGenres, genreDim, titleTypes, sizeof(titleTypes)/sizeof(char*));
        if (title != NULL) {
            loadData(imdb, title);
            freeTitle(title);
        }
    }
    closeFile(csvTitles);
    writeResults(imdb);
    // TODO: escritura de csv
    freeImdb(imdb);
    return 0;
}

void loadGenres(char * fileName, char * vecGenres[], char * genreDim) {
    csvADT genresCsv = newCsv(fileName, READ);
    int i = 0;
    while (i < QTY_GENRES && !eof(genresCsv))
        vecGenres[i++] = readNextString(genresCsv);
    *genreDim = i > 0 ? i - 1 : i;
    closeFile(genresCsv);
}

void writeResults(imdbADT imdb){
    toBeginYear(imdb);
    csvADT query1File = newCsv("/Users/mbp13/Desktop/PI/TPE_G1/query1.csv", WRITE);
    writeString(query1File, QUERY1_HEADERS);
    while (hasNextYear(imdb)){
        writeQuery1(query1File, getCurrentYear(imdb), getQtyFilms(imdb), getQtyShorts(imdb), getQtySeries(imdb));
        nextYear(imdb);
    }
}

