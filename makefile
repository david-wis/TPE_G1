COMPILER=gcc
DEBUG_COMPILER=-g -DDEBUG=1
OUTPUT_FILE=imdb
FILES=main.c imdbADT.c utils.c csvADT.c title.c

all:
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) -Wall -pedantic -std=c99 -lm -fsanitize=address

debug: COMPILER+=$(DEBUG_COMPILER)
debug: all
	./$(OUTPUT_FILE) imdb.csv genres.csv

imdbADT.o:
	$(COMPILER) -c imdbADT.c utils.c title.c

csvADT.o: csvADT.c
	$(COMPILER) -c csvADT.c utils.c title.c

clean:
	rm $(OUTPUT_FILE)