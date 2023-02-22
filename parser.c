#include "parser.h"
#include <regex.h>
#include "converter.h"
/**
 * @brief Implementation of the parser
 */

/**
 * @brief This method handles the opening of the file  
 * It also checks the path of the file with regular expressions
 * @param path : the path of the file
 * @return FILE* : Pointer towards the file
 */


FILE*  readFile(char* path) {
    FILE *file = NULL; //File stream
    regex_t regex; //Regular expression struct
    char* pattern = "\\s*\\w*.mp\\s*";
    /*Compiling regular expression*/
    int reti = regcomp(&regex,pattern,0);
    if (reti) {
        perror("Could not compile regex \n");
        exit(EXIT_FAILURE);
    }
    printf(" path : %s \n",path);

    /*Applying regex to path*/
    reti = regexec(&regex,path,0,NULL,0);
    if (reti == REG_NOMATCH) {
        perror("Your file name has invalid characters");
        exit(EXIT_FAILURE);
    }else if (reti == 0) {
        printf("Valid file name : %s \n",path);
        file = fopen(path,"rb");
        if (!file) {
            perror("No such file or directory or unable to access file \n");
            exit(EXIT_FAILURE);
        }
    }

    regfree(&regex);
    return file;

}

/**
 * This function reads the header of the mp file and stores it in a metadata struct
 * The header starts with a byte identifying the block type (H,C,D)
 * Then it has a 4 bytes integer representing the size of the block
 * Then it has an l bytes representing the content of the block
 * @param file : the file stream
 * @param blocktype : the type of the block (H,C,D)
 * @return metadata : the metadata struct
 */
metadata* readHeader(FILE *file) {
    metadata *meta = (metadata*)malloc(sizeof(metadata));
    char magic[8]; // Char to store MINI-PNG
    int blockLength; // Int to store the length of the block
    char blockType; // Char to store the type of the block
    int readBytes = 0; // Int to store the number of bytes read
    int isHeaderFound = 0; // Int to store if the header has been found
    int numberofCblocks= 0; // Int to store the number of comment blocks
    int numberofDblocks = 0; // Int to store the number of data blocks
    int DblocksSize = 0; // Int to store the size of the data blocks
    // Read the Mini-PNG string
    fread(magic, 1, 8, file);
    readBytes += 8;
    if (strncmp(magic, "Mini-PNG", 8) != 0) {
        fprintf(stderr, "Error: Invalid file format.\n");
        exit(EXIT_FAILURE);
    }

    // Read the blocks until the end of the file
     while (!feof(file)) {
        if ((fread(&blockType, sizeof(char), 1, file) != 1) | (blockType == '\000'))   {
            break; // end of file
        }

        char blockLengthBuffer[4];
            if (fread(&blockLengthBuffer, sizeof(int), 1, file) != 1) {
            perror("Failed to read block length");
            exit(1);
        }
        blockLength = (blockLengthBuffer[0] << 24) | (blockLengthBuffer[1] << 16) | (blockLengthBuffer[2] << 8) | blockLengthBuffer[3];

        if (blockType == 'H') {
            readBytes += 4;
            if (blockLength != 9) {
                fprintf(stderr, "Error: Invalid header block length.\n");
                exit(EXIT_FAILURE);
            }
            isHeaderFound = 1;
            unsigned char buffer[9];

            fread(buffer, 1, 9, file);
            readBytes += 9;

            meta->width = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
            meta->height = (buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | buffer[7];
            meta->pixelType = buffer[8];
            switch (meta->pixelType) {
                case 0:
                    printf("This picture is a black and white picture \n");
                    break;
                case 1:
                    printf("This picture is a grayscale picture \n");
                    break;
                case 2:
                    printf("This picture is a palette picture \n");
                    break;
                case 3:
                    printf("This picture is a 24bit color picture \n");
                    break;
                default:
                    perror("Incompatible PixelType");
                    exit(EXIT_FAILURE);

            }
        } else if (blockType == 'C') {
            if (numberofCblocks == 0) {
                //printf("Comment block found.\n");
                char *commentbuffer = (char *) malloc(blockLength + 1);
                fread(commentbuffer, 1, blockLength, file);
                meta->comment = commentbuffer;
                readBytes += blockLength;
                numberofCblocks++;
            } else {
                //printf("Comment block found.\n");
                char *commentbuffer = (char *) malloc(blockLength + 1);
                fread(commentbuffer, 1, blockLength, file);
                meta->comment = realloc(meta->comment, strlen(meta->comment) + blockLength + 1);
                strcat(meta->comment, commentbuffer);
                readBytes += blockLength;
            }

        } else if (blockType == 'D') {
            // Check if the D blocks are separated by another block
            if (numberofDblocks == 0) {
                //printf("Data block found.\n");
                meta->data = (unsigned char *) malloc(blockLength);
                fread(meta->data, 1, blockLength, file);
                readBytes += blockLength;
                numberofDblocks++;
                DblocksSize += blockLength;
            } else {
                meta->data = realloc(meta->data, sizeof(meta->data) + blockLength);
                fread(meta->data, 1, blockLength, file);
                readBytes += blockLength;
                numberofDblocks++;
                DblocksSize += blockLength;
            }
            //Comparing the size of the D block with the width and height of the image


        } else if (blockType == '0') {
            //printf("End of file found.\n");
            break;
        } else {
            fprintf(stderr, "Error: Invalid block type.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (!isHeaderFound) {
        fprintf(stderr, "Error: Header block not found.\n");
        exit(EXIT_FAILURE);
    }

    if (numberofDblocks == 0) {
        fprintf(stderr, "Error: Data block not found.\n");
        exit(EXIT_FAILURE);
    }

    if (numberofCblocks == 0) {
        meta->comment = "";
    }
  

    return meta;
}
/**
* @brief This function displays the image in the terminal
 * pixels are stored from top to bottom then from left to right
 * The second parameter determines the printmode which can take two values
 *  *** 1 if we wish to print X whenever the bit is equal to one and 0 for the same value and it's opposite.
 * @param file : the file stream
 *
*/
void displayImage(FILE *file) {
    int printmode = 0; // Printing mode
    
    metadata *meta = readHeader(file);
    int width = meta->width;
    int height = meta->height;
    int pixelType = meta->pixelType;
    char *comment = meta->comment;
    unsigned char *data = meta->data;
    printf("width : %d \n",width);
    printf("height : %d \n",height);
    printf("pixelType : %d \n",pixelType);
    printf("comment : %s \n",comment);
    printf("Image : \n");
    if (pixelType == 0) {
				printf("Choose print mode : \n 0 : space for 1 and X for 0 \n 1 : X for 1 and space for 0 \n");
				scanf("%d",&printmode);
        if (printmode == 1) {
            for (int i = 0; i < (width < height ? width : height) + abs(width - height); i++) {
                for (int bit = 7; bit >= 0; bit--) {

                    if ((data[i] >> bit) & 1) {
                        printf("X");
                    } else {
                        printf(" ");
                    }

                }

                printf("\n");
            }
        } else if (printmode == 0) {
            for (int i = 0; i < (width < height ? width : height) + abs(width - height); i++) {
                for (int bit = 7; bit >= 0; bit--) {

                    if ((data[i] >> bit) & 1) {
                        printf(" ");
                    } else {
                        printf("X");
                    }

                }

                printf("\n");
            }
        }
    } else {
        FILE *output = fopen("output.pnm", "w");
        convertToPNM(meta,output);
        printf("Image converted to PNM format and saved in output.pnm \n");
        fclose(output);
    }
    fclose(file);
}

