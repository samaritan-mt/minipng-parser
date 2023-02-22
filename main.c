#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc,char ** argv) {
    if (argc < 2) {
        perror("Usage : ./main <path to the .mp file>\n");
        exit(-1);
    } else {
        char * path = argv[1];
        FILE * file = readFile(path);
        if (!file) {
            printf("Parsing failed \n");
        }else {

            displayImage(file);
        }
    }
    return 0;
}
