//
// Created by mtsaad on 21/02/23.
//

#include "converter.h"

/**
 * This function converts the MiniPNG file to a PNM file
 * @param meta : the metadata struct containing the MiniPNG file
 * @param char *  : the name of the output file of the output file
 *
 */
void convertToPNM(metadata *meta,FILE *file) {
    int width = meta->width;
    int height = meta->height;
    int pixelType = meta->pixelType;
    char *comment = meta->comment;
    unsigned char *data = meta->data;
    if (pixelType == 1) {
    // Initialize the header of the PNM file
    fprintf(file, "P1\n");
    // Write the width and the height of the image
    fprintf(file, "%d\n %d\n", width, height);
    // Write the comment of the image
    if (!strcmp("",comment)){
        fprintf(file, "# %s\n", comment);
    }
    // Write the image data
    for (int i = 0; i < width * height; i++) {
        if (data[i] == 0xff) {
            fprintf(file, "1");
        } else if (data[i] == 0x00) {
            fprintf(file, "0");
        }
        fprintf(file, "\n");

    }

    } else if (pixelType == 3) {
        // Initialize the header of the PNM file
        fprintf(file, "P3\n");
        // Write the width and the height of the image
        fprintf(file, "%d\n %d\n", width, height);
        //write the maximum value of the colors which is 255
        fprintf(file, "255\n");
        // Write the comment of the image
        if (!strcmp("",comment)){
        fprintf(file, "# %s\n", comment);
        }
        // Write the image data
        //the 24 bits are stored in 3 bytes
        // the first byte is the red component, the second byte is the green component and the third byte is the blue component
        // Retrieve the 3 bytes and print them in each line separated by a space as integers
        for (int i = 0; i < width * height * 3; i += 3) {
            fprintf(file, "%d %d %d\n", data[i] , data[i + 1] , data[i + 2] );
        }

    }

}
