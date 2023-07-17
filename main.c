#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_image.h"

void main(int argc, char** argv)
{
    char nfFilename[25];
    char rawFilename[25];

    strcat(strcpy(nfFilename, argv[1]), ".nf");
    strcat(strcpy(rawFilename, argv[1]), ".raw");

    printf("%s %s\n", nfFilename, rawFilename);
    
    Image image;

    setImageDimensionsFromFile(&image, nfFilename);
    generateImageFromArbitraryPixelsFromFile(&image, rawFilename);
    printImageInfo(&image);
    generateImageFromArbitraryPixels(&image);
    // printMaterialMap();
}