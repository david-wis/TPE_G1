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

#endif //TPE_G1_CSVADT_H
