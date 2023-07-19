#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/image.h"
#include "headers/filter.h"

int main(int argc, char** argv)
{
    char nfFilename[25];
    char rawFilename[25];

    strcat(strcpy(nfFilename, argv[1]), ".nf");
    strcat(strcpy(rawFilename, argv[1]), ".raw");

    printf("%s %s\n", nfFilename, rawFilename);
    
    Image image;
    float sigma = 0;
    int window_size = 1;

    setImageDimensionsFromFile(&image, nfFilename);
    getImagePixelsFromFile(&image, rawFilename);

    // for(int i = 1; i <= 9; i += 2) {
    //     window_size = i;

    //     printf("\n\n");
    //     char newImageFilename[100] = "";
    //     sprintf(newImageFilename, "median%d.raw", i);
    //     printf("%s\n", newImageFilename);
    //     getImagePixelsFromFile(&image, rawFilename);
    //     medianFilter(image.pixels, image.width, image.height, window_size);
    //     image.width = image.height;
    //     printImageInfo(&image);
    //     generateImageFromPixels(&image, newImageFilename);
    //     freeImagePixels(&image);
    // }

    for(int i = 1; i <= 9; i += 2) {
        window_size = i;

        for(int j = 1; j < 10; j++) {
            sigma = j * 0.1;
            
            char newImageFilename[100] = "";
            sprintf(newImageFilename, "data/raw_output/nlm%d_%d.raw", j, window_size);
            printf("%s\n", newImageFilename);
            getImagePixelsFromFile(&image, rawFilename);
            nlmFilter(image.pixels, image.width, image.height, sigma, window_size);
            generateImageFromPixels(&image, newImageFilename);
            freeImagePixels(&image);
        }
    }

    return 0;
}