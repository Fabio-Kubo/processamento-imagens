/*
 *Created by Deangeli Gomes Neves
 *
 * This software may be freely redistributed under the terms
 * of the MIT license.
 *
 */

#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "image.h"
#include "common.h"
#include "adjacencyRelation.h"
#include "featureVector.h"

typedef struct _histogram {
    float *val;
    int    n;
    int binSize;
} Histogram;


Histogram *GrayHistogram(GrayImage *grayImage);
Histogram *GrayHistogram(GrayImage *grayImage, int nbins);
Histogram *GrayHistogram(GrayImage *grayImage, int maxValueAllowed, int minValueAllowed);
Histogram *GrayHistogram(GrayImage *grayImage, int nbins, int maxValueAllowed, int minValueAllowed);
Histogram *GrayHistogramFrom8BitGrayImage(GrayImage *grayImage);
Histogram *ColorHistogram(ColorImage *colorImage, int nbins);
Histogram *ColorHistogramFrom8bitColorIMage(ColorImage *colorImage, int nbinsPerChannel);
Histogram* computeHistogram(GrayImage* grayImage, int numberBins);
Histogram* computeHistogram(ColorImage* colorImage, int numberBins);
void DestroyHistogram(Histogram **hist);
FeatureVector* createFeatureVector(Histogram *histogram);
FeatureVector* computeHistogramForFeatureVector(Image *image,float binSize, bool normalization);
/*
 * computa a densidade de probabilidade para cada pixel da imagem baseado em todos os outros pixels
 * */
GrayImage *ProbabilityDensityFunction(ColorImage *img, double stdev);
GrayImage *ProbabilityDensityFunction(GrayImage *img, double stdev);

/*
 * computa a densidade de probabilidade para cada pixel da imagem baseado em sua adjacência
 * */
GrayImage *ProbabilityDensityFunction(ColorImage *img, double stdev,AdjacencyRelation *adjRel);
GrayImage *ProbabilityDensityFunction(GrayImage *img, double stdev,AdjacencyRelation *adjRel);

void writeHistogram(Histogram *hist, char *filename);
void destroyHistogram(Histogram **hist);
Histogram* createHistogram(int n);
Histogram* computeHistogram(Image *image,float binSize,bool normalization);
Image *ProbabilityDensityFunction(Image *image, double standardDeviation);



#endif //_HISTOGRAM_H
