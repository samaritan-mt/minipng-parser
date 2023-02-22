//
// Created by mtsaad on 21/02/23.
//
/**
 * This file is part of the TP-Parser project.
 * It handles the conversion of the MiniPNG files to PNM files
 * @file converter.h
 * @brief This file contains the prototypes of the functions used to convert the MiniPNG files to PNM files
 * @author Saad MOUTAOUAKIL
 */

#ifndef TP_PARSER_CONVERTER_H
#define TP_PARSER_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/**
 * This function converts the MiniPNG file to a PNM file
 * @param meta : the metadata struct containing the MiniPNG file
 * @param file : the file stream of the output file
 */
void convertToPNM(metadata *meta,FILE *file);

#endif //TP_PARSER_CONVERTER_H
