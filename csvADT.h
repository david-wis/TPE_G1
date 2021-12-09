//
// Created by mbp13 on 03/12/2021.
//

#ifndef TPE_G1_CSVADT_H
#define TPE_G1_CSVADT_H
#include "title.h"

typedef struct csvCDT * csvADT;

csvADT newCsv(char * path, char * mode);

char * readNextString(csvADT csv);

tTitle * readNextTitle(csvADT csv, char ** genres, size_t genresDim, char * titleTypes[], size_t typesDim);

int eof(csvADT csv);

void closeFile(csvADT csv);

void writeString(csvADT csv, const char * txt);

void writeQuery1(csvADT csv, unsigned short year, unsigned long films, unsigned long series, unsigned long shorts);

void writeQuery2(csvADT csv, unsigned short year, char qtyGenres, char ** genres, unsigned long ** qtyByGenres);

void writeQuery3(csvADT csv, unsigned short year, char * title, unsigned long votes, float rating, char * genres);

#endif //TPE_G1_CSVADT_H
