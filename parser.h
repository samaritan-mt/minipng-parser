#ifndef PARSER_H
#define PARSER_H
/**
 * This file is part of the TP-Parser project.
 * It handles the parsing of the MiniPNG files
 * @file parser.h
 * @brief This file contains the prototypes of the functions used to parse the MiniPNG files
 * @author Saad MOUTAOUAKIL
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    unsigned char pixelType;
    char * comment;
    unsigned char * data;
}metadata;

FILE* readFile(char * path);

metadata* readHeader(FILE * file);
void displayImage(FILE * file);



#endif


