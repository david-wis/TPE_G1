//
// Created by mbp13 on 03/12/2021.
//

#ifndef TPE_G1_CSVADT_H
#define TPE_G1_CSVADT_H
typedef struct csvCDT * csvADT;

csvADT newCsv(char * path, char * mode);

static char * readLine(FILE * f);

char * readNextString(csvADT csv);

tTitle * readNextTitle(csvADT csv, char ** genres, char ** titleTypes);

int eof(csvADT csv);

void closeFile(csvADT csv);

#endif //TPE_G1_CSVADT_H
