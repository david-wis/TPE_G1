#include <stdio.h>
#include <stdlib.h>
#include "csvADT.h"
#include "imdbADT.h"

#define QTY_GENRES 32
#define WRONG_PARAMS "Two arguments were expected"
#define READ "r"
#define WRITE "w"
#define QUERY1_HEADERS "year;films;series;shorts"
#define QUERY3_HEADERS "year;film;votes;rating;genres"


void loadGenres(char * fileName, char * vecGenres[], char * genreDim);
void writeResults(imdbADT imdb);
void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv);

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
    loadGenres("/home/david/Desktop/TPE-G1/genres.csv", vecGenres, &genreDim); //TODO: Ponerlo con argv

    imdbADT imdb = newImdbADT(vecGenres, genreDim);
    csvADT csvTitles = newCsv("/home/david/Desktop/TPE-G1/imdb.csv", READ); //TODO: Ponerlo con argv
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

void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv) {
    if (!hasNextMovie(imdb))
        return;
    char * title = getCurrentMovieTitle(imdb);
    unsigned long votes = getCurrentMovieVotes(imdb);
    float rating = getCurrentMovieRaiting(imdb);
    char * genres = getCurrentMovieGenres(imdb);
    nextMovie(imdb);
    writeMoviesRec(imdb, year, csv);
    writeQuery3(csv, year, title, votes, rating, genres);
}

void writeResults(imdbADT imdb){
    toBeginYear(imdb);
    csvADT query1File = newCsv("/home/david/Desktop/TPE-G1/query1.csv", WRITE);
    csvADT query2File = newCsv("/home/david/Desktop/TPE-G1/query2.csv", WRITE);
    csvADT query3File = newCsv("/home/david/Desktop/TPE-G1/query3.csv", WRITE);
    writeString(query1File, QUERY1_HEADERS);
    writeString(query3File, QUERY3_HEADERS);
    while (hasNextYear(imdb)){
        unsigned short year = getCurrentYear(imdb);

        //Query 1
        writeQuery1(query1File, year, getQtyFilms(imdb), getQtySeries(imdb), getQtyShorts(imdb));

        //Query 2

        // Query 3
        toBeginMovie(imdb);
        writeMoviesRec(imdb, year, query3File);
        nextYear(imdb);
    }
    closeFile(query1File);
    closeFile(query2File);
    closeFile(query3File);
}

