#include "FL.h"

int main(int argc, char **argv) {

    GrayImage *grayImage = NULL;
    char *pos = NULL;
    char fileExtension[10];

    if (argc!=2){
        printf("Usage: <image1.[pgm]>\n");
        exit(-1);
    }

    pos = strrchr(argv[1],'.') + 1;
    sscanf(pos,"%s",fileExtension);


    if (strcmp(fileExtension,"pgm")==0){
        grayImage = readGrayImage(argv[1]);
        applyGranulometryOnImage(grayImage,20);
        destroyGrayImage(&grayImage);
    } else { /* ppm */
        printf("Invalide image format\n");
    }

    //free(pos);
    return 0;
}

