/*
 *Created by Deangeli Gomes Neves
 *
 * This software may be freely redistributed under the terms
 * of the MIT license.
 *
 */


#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"
#include "file.h"
#include "png.h"
#include "jpeglib.h"
/* Implementacoes vetoriais de uma imagem cinza 2D: inteira e double */

typedef struct _grayImage { 
  int *val; /* brilho do pixel */
  int  ncols,nrows; /* dimensoes da imagem */
  int *tbrow; /* tabela de linhas */
} GrayImage;


typedef struct _cor {
  int val[3]; /* R,G,B */
} Cor;

typedef struct _colorimage {
  int   nx,ny;    /* dimensoes da imagem */
  Cor **cor;      /* matriz com a cor dos pixels */ 
  float dx,dy;    /* tamanho do pixel em unidades de comprimento */
  int   Imax;     /* depth (2^b-1)*/
  char  unid[10]; /* unidade de comprimento */
} ColorImage;


/*
 * ColorImage e GrayImage sao estruturas obseltas, elas estao sendo mantidos por questoes de retrocompatibilidade.
 * Tente usar a estrurua Image que pode ser usada para imagens coloridas e cinzas. Nessa Estrutura cada canal da imagem
 * e representado por um vetor de floats. Entao em uma imagem RGB, a estrutura tera 3 canais e para acessar a
 * coordenada X=30 e y=16 de uma imagem por exemplo, voce tera que fazer o calculo index = y*(image->nx) + x
 * para poder obter o indice do vetor que corresponde a essa posicao da imagem.
 *
 * O atributo Scaling factor e util para saber quantos bits tem em cada canal. Normalmente as imagens sao 8 bits,
 * portanto as intensidades de luminosidade variam entre 0 a 255. Neste Caso o Scaling factor é 255, pois se eu quiser
 * deixar todos os valores de uma imagem entre 0 e 1 eu preciso dividir por 255.
 * */


enum ColorSpace {
    UNKNOWN = 0,
    GRAYSCALE = 1,
    GRAYSCALE_ALPHA = 2,
    RGB = 3,
    RGBA = 4,
    YCbCr = 5,
    YCbCrK = 6,
    HSV = 7,
    HSVA = 8,
    CMYK = 9
};

enum DataType {
    FLOAT,
    DOUBLE
};


typedef struct _image {
    int   nx,ny,nz;    /* dimensoes da imagem */
    float dx,dy, dz;    /* tamanho do pixel em unidades de comprimento */
    unsigned  char channelDepth;
    int numberPixels;
    float **channel; /*might be RGB, or Ycbcr*/
    int scalingFactor;
    char  unid[10]; /* unidade de comprimento */
    int nchannels;
    ColorSpace colorSpace;
    DataType dataTypeId;
} Image;


//macro para facil acesso aos pixels da imagem. a macro computa o indice para acessar a posicao (x,y) da imagem
#define imageVal(image, x, y) image->channel[0][(y*image->nx) + x]
#define imageValCh(image, x, y, c) image->channel[c][(y*image->nx) + x]
#define imageVolume(image, x, y,z) image->channel[0][(z*image->ny*image->nx) + (y*image->nx) + x]
#define imageVolumeCh(image, x, y, z,c) image->channel[c][(z*image->ny*image->nx) + (y*image->nx) + x]

#define numericDerivate(f,x,index,step) f(x)

void         *readImageByExt(char *filename);
GrayImage    *createGrayImage(int ncols,int nrows);
void          destroyGrayImage(GrayImage **img);
GrayImage     *readGrayImage(char *filename);
void          writeGrayImage(GrayImage *img, char *filename);
int           minimumValue(GrayImage *img);
int           maximumValue(GrayImage *img);

ColorImage   *createColorImage(int nx, int ny);
void          destroyColorImage(ColorImage **I);
ColorImage   *readColorImage(char *filename);
void          writeColorImage(ColorImage *I, char *filename);
int minimumColorValue(ColorImage *img);
int maximumColorValue(ColorImage *img);
int minimumIntensityColor(ColorImage *img, int c);
int maximumIntensityColor(ColorImage *img, int c);

bool isValidPixelCoordinate(GrayImage *image,int pixelCoordinateX,int pixelCoordinateY);
bool isImagesSameDomain(GrayImage *image1,GrayImage *image2);
void copyGrayImage(GrayImage *image1,GrayImage **image2);
/* Subtrai a imagem2 da imagem1 e guarda o resultado na imagem1.
 * O parametro saturation define se valores abaixo de zero serao armazenados na imagem1 ou nao*/
GrayImage *imageSubtraction(GrayImage *image1, GrayImage *image2, bool saturation);
/* soma a intensidade de todos os pixels na imagem*/
int sumUpAllPixelsValues(GrayImage *image);

/*salva uma imagem em ASCII. O parametro maxValueOnChanel define o valor maximo no canal...comumente
 * esse valor e 255*/
void writeImageAsASCII(ColorImage* image,const char *filename, unsigned int maxValueOnChanel);
void writeImageAsASCII(GrayImage* image,const char *filename,unsigned int maxValueOnChanel);

/*salva uma imagem em binario. O parametro maxValueOnChanel define o valor maximo no canal...comumente
 * esse valor e 255*/
void writeImageAsBinary(ColorImage* image,const char *filename, unsigned int maxValueOnChanel);
void writeImageAsBinary(GrayImage* image,const char *filename,unsigned int maxValueOnChanel);

void writeImage(ColorImage* image,const char *filename, const char *Magicnumber,unsigned int maxValueOnChanel);
void writeImage(GrayImage* image,const char *filename, const char *Magicnumber,unsigned int maxValueOnChanel);
void writeImage(GrayImage* image,const char *filename);
void writeImage(ColorImage* image,const char *filename);


ColorImage *RGBtoYCbCr(ColorImage *rgb);
GrayImage* extractColorChannelAsGrayImage(ColorImage* colorImage,int channel);

/*funcoes que utilizam a nova estrutura de imagem*/
Image* createImage(int nx, int ny,int nchannels);
Image* createImage(int nx, int ny,int nz, int nchannels);
Image* createImage(int nx, int ny);
void destroyImage(Image**image );
Image *readImagePGM(char *filename);
Image *readImagePPM(char *filename);
Image *readImagePNG(char *filename);
Image *readImageJPEG(char *filename);
Image* readImage(char *filename);
void writeImageP2(Image *image,char *filename);
void writeImageP3(Image *image, char *filename);
void writeImageP5(Image *image, char *filename);
void writeImageP6(Image *image, char *filename);
void writeImagePGM(Image* image, char *filename,  char* magicNumber);
void writeImagePPM(Image* image, char *filename,  char* magicNumber);
void writeImagePNG(Image *image,char *filename);
void writeImageJPEG(Image *image,char *filename);
void writeImage(Image* image, char *filename);
bool isValidPixelCoordinate(Image *image,int pixelCoordinateX,int pixelCoordinateY);
Image *imageSubtraction(Image *image1, Image *image2, bool saturation);
bool isImagesSameDomain(Image *image1,Image *image2);
Image *convertRGBtoYCbCr(Image *rgbImage);
GrayImage* convertImage2GrayImage(Image* image);
Image* convertGrayImage2Image(GrayImage* grayImage);
ColorImage* convertImage2ColorImage(Image* image);
Image* convertColorImage2Image(ColorImage* colorImage);
float sumUpAllPixelsValues(Image *image, bool normalize);
Image* extractImageChannel(Image* image, int channel);
void printImage(Image* image);
Image* createAlphaChannel(Image* image,float alpha);
Image* convertGrayImage2RGBImage(Image* image_ppm);
Image* mergeImages(Image* image1, Image* image2);
Image* getSlice(Image* image, int sliceIndex);
Image* packImagesFromDirectory(DirectoryManager* directoryManager);
Image* extractSubImage(Image*image, int xCoord,int yCoord, int xsize, int ysize, bool zeroPadding);


uint8_t* convertImage2IntergerArray8bits(Image* imageRGBA);
Image* copyImage(Image* image, bool copyChannels);
void addUniformNoise(Image* image, float uniformValue, double probability);
void addSaltAndPepperNoise(Image* image, double probability);
void addAdditiveGaussianNoise(Image* image, double mean, double variance);

#endif
