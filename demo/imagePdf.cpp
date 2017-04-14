#include "FL.h"

int main(int argc, char **argv) {
    char             ext[10],*pos;
    if (argc != 4) {
        printf("pdf <image.ppm> <stdev> <pdf.pgm>\n");
        exit(-1);
    }

    pos = strrchr(argv[1],'.') + 1;
    sscanf(pos,"%s",ext);

    if (strcmp(ext,"ppm")==0){
        ColorImage *colorImage =  readColorImage(argv[1]);
        printf("computando a pdf...\n");
        GrayImage  *pdf        = ProbabilityDensityFunction(colorImage,atof(argv[2]));
        writeGrayImage(pdf,argv[3]);
        destroyGrayImage(&pdf);
        destroyColorImage(&colorImage);
    }else if(strcmp(ext,"pgm")==0){ /* ppm */
        GrayImage *grayImage =  readGrayImage(argv[1]);
        printf("computando a pdf...\n");
        GrayImage  *pdf        = ProbabilityDensityFunction(grayImage,atof(argv[2]));
        writeGrayImage(pdf,argv[3]);
        destroyGrayImage(&pdf);
        destroyGrayImage(&grayImage);
    }
    Image* image = readImage(argv[1]);
    printf("computando a pdf...\n");
    Image *Ipdf = ProbabilityDensityFunction(image, atof(argv[2]) );
    writeImage(Ipdf,"Imagepdf.pgm");
    destroyImage(&image);
    destroyImage(&Ipdf);
    return 0;
}

