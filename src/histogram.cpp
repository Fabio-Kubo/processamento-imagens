#include "histogram.h"
#include <omp.h>



Histogram *GrayHistogram(GrayImage *grayImage, int nbins)
{
    Histogram *hist = (Histogram *) calloc(1,sizeof(Histogram));
    int p, n = grayImage->ncols*grayImage->nrows;
    int i, Imax = maximumValue(grayImage), Imin=minimumValue(grayImage);
    int K = (Imax-Imin)/MIN(nbins,Imax-Imin); /* size of each interval */

    hist->n   = nbins+1;
    hist->val = (float *)calloc(hist->n,sizeof(float));

    for (p=0; p <  n; p++) {
        hist->val[(grayImage->val[p] - Imin) / K]++;
    }
    for (i=0; i <  hist->n; i++) {
        hist->val[i] /= n;
    }

    return(hist);
}


Histogram *GrayHistogram(GrayImage *grayImage, int maxValueAllowed, int minValueAllowed)
{
    Histogram *hist = (Histogram *) calloc(1,sizeof(Histogram));
    int p, n = grayImage->ncols*grayImage->nrows;
    int i;

    hist->n   = maxValueAllowed-minValueAllowed+1;
    hist->val = (float *)calloc(hist->n,sizeof(float));

    for (p=0; p <  n; p++)
        hist->val[grayImage->val[p]-minValueAllowed] += 1;

    for (i=0; i <  hist->n; i++)
        hist->val[i] /= n;

    return(hist);
}

Histogram *GrayHistogram(GrayImage *grayImage, int nbins, int maxValueAllowed, int minValueAllowed)
{
    Histogram *hist = (Histogram *) calloc(1,sizeof(Histogram));
    int p, n = grayImage->ncols*grayImage->nrows;
    int i;

    hist->n   = nbins;
    float step = (maxValueAllowed-minValueAllowed+1)/(float)nbins;
    hist->val = (float *)calloc(hist->n,sizeof(float));
    int binIndex = -1;

    for (p=0; p <  n; p++)
        binIndex = (grayImage->val[p]-minValueAllowed)/step;
    hist->val[binIndex] += 1.0f;

    for (i=0; i <  hist->n; i++)
        hist->val[i] /= n;

    return(hist);
}

Histogram *GrayHistogramFrom8BitGrayImage(GrayImage *grayImage){
    Histogram *hist = GrayHistogram(grayImage, 255, 0);
    return hist;
}

/* K = 2^b / w is the size of each interval, where b is the depth of
   the image per channel (we are assuming 8 bits per channel) and w is
   the number of intervals in each axis. */

Histogram *ColorHistogram(ColorImage *colorImage, int nbins)
{
    Histogram *hist = (Histogram *) calloc(1,sizeof(Histogram));
    int xp, yp, n = colorImage->nx*colorImage->ny;
    int i,  w = (pow((double)nbins,(1.0/3.0))+0.5), K = (maximumColorValue(colorImage)+1) / w + 1;


    hist->n   = w*w*w + 1;
    hist->val = (float *)calloc(hist->n,sizeof(float));

    for (yp=0; yp < colorImage->ny; yp++){
        for (xp=0; xp < colorImage->nx; xp++){
            int l1, l2, l3;
            l1 = colorImage->cor[yp][xp].val[0]/K;
            l2 = colorImage->cor[yp][xp].val[1]/K;
            l3 = colorImage->cor[yp][xp].val[2]/K;
            i  = l1 + l2*w + l3*w*w;
            hist->val[i]++;
        }
    }

    for (i=0; i <  hist->n; i++) {
        hist->val[i] /= n;
    }


    return(hist);
}


Histogram *ColorHistogramFrom8bitColorImage(ColorImage *colorImage, int nbinsPerChannel)
{
    Histogram *hist = (Histogram *) calloc(1,sizeof(Histogram));
    int xp, yp, n = colorImage->nx*colorImage->ny;
    int i,  w = nbinsPerChannel;
    float binSize = 256/(nbinsPerChannel);


    hist->n   = w*w*w ;
    hist->val = (float *)calloc(hist->n,sizeof(float));
    int l1, l2, l3;
    for (yp=0; yp < colorImage->ny; yp++){
        for (xp=0; xp < colorImage->nx; xp++){
            l1 = colorImage->cor[yp][xp].val[0]/binSize;
            l2 = colorImage->cor[yp][xp].val[1]/binSize;
            l3 = colorImage->cor[yp][xp].val[2]/binSize;
            i  = l1 + l2*w + l3*w*w;
            hist->val[i] += 1.0f;
        }
    }

    for (i=0; i <  hist->n; i++) {
        hist->val[i] /= n;
    }


    return(hist);
}


void writeHistogram(Histogram *hist, char *filename) {
    FILE *fp = fopen(filename, "w");
    int i;

    for (i = 0; i < hist->n; i++) {
        fprintf(fp, "%d %f\n", i, hist->val[i]);
    }

    fclose(fp);
}

GrayImage *ProbabilityDensityFunction(ColorImage *img, double stdev)
{
    GrayImage *pdf = createGrayImage(img->nx, img->ny);
    int n = pdf->nrows*pdf->ncols;
    float  *val = (float *)calloc(img->nx*img->ny,sizeof(float));
    double  K = 2.0*stdev*stdev, maxdist = 3*stdev*3*stdev;
    float   Imax = maximumColorValue(img), maxval, minval;

    maxval = INT_MIN; minval = INT_MAX;
#pragma omp parallel for
    for (int p=0; p < n; p++) {
        double Rp, Gp, Bp;
        Rp = img->cor[p/img->nx][p%img->nx].val[0]/Imax;
        Gp = img->cor[p/img->nx][p%img->nx].val[1]/Imax;
        Bp = img->cor[p/img->nx][p%img->nx].val[2]/Imax;

        for (int q=0; q < n; q++) {
            if (p != q) {
                double dist, Rq, Gq, Bq;
                Rq = img->cor[q/img->nx][q%img->nx].val[0]/Imax;
                Gq = img->cor[q/img->nx][q%img->nx].val[1]/Imax;
                Bq = img->cor[q/img->nx][q%img->nx].val[2]/Imax;
                dist = (Rp-Rq)*(Rp-Rq) + (Gp-Gq)*(Gp-Gq) + (Bp-Bq)*(Bp-Bq);
                if (dist <= maxdist){
                    val[p] += exp(-dist/K);
                }
            }
        }
        if (val[p] > maxval)
            maxval = val[p];
        if (val[p] < minval)
            minval = val[p];
    }

    if (minval != maxval)
        for (int p=0; p < n; p++) {
            pdf->val[p] = (int)(65535*(val[p]-minval)/(maxval-minval));
        }
    free(val);

    return(pdf);
}

GrayImage *ProbabilityDensityFunction(GrayImage *img, double stdev)
{
    GrayImage *pdf = createGrayImage(img->ncols, img->nrows);
    int n = pdf->nrows*pdf->ncols;
    float  *val = (float *)calloc(img->ncols*img->nrows,sizeof(float));
    double  K = 2.0*stdev*stdev, maxdist = 3*stdev*3*stdev;
    float   Imax = maximumValue(img), maxval, minval;

    printf("maxdist %f\n",maxdist);

    maxval = INT_MIN; minval = INT_MAX;
#pragma omp parallel for
    for (int p=0; p < n; p++) {
        double grayp;
        grayp = img->val[p]/Imax;


        for (int q=0; q < n; q++) {
            if (p != q) {
                double dist, grayq;
                grayq = img->val[q]/Imax;
                dist = (grayp-grayq);

                if (dist <= maxdist){
                    val[p] += exp(-dist/K);
                }
            }
        }
        if (val[p] > maxval)
            maxval = val[p];
        if (val[p] < minval)
            minval = val[p];

    }

    if (minval != maxval)
        for (int p=0; p < n; p++) {
            pdf->val[p] = (int)(65535*(val[p]-minval)/(maxval-minval));
        }
    free(val);

    return(pdf);
}

void destroyHistogram(Histogram **hist)
{
    if (*hist != NULL) {
        free((*hist)->val);
        free(*hist);
    }
    *hist = NULL;
}

Histogram* computeHistogram(GrayImage* grayImage, int numberBins){
    return GrayHistogram(grayImage,numberBins);
}

Histogram* computeHistogram(ColorImage* colorImage, int numberBins){
    return ColorHistogram(colorImage,numberBins);
}

FeatureVector* createFeatureVector(Histogram *histogram){
    FeatureVector* featureVector = (FeatureVector*)calloc(1,sizeof(FeatureVector));
    featureVector->size = histogram->n;
    featureVector->features = (float *)calloc(histogram->n,sizeof(float));
    for (int i = 0; i < histogram->n; ++i) {
        featureVector->features[i] = histogram->val[i];
    }
    return featureVector;
}

Histogram* computeHistogram(Image *image,float binSize, bool normalization){

    int numberBinsPerChannel = ceil((image->scalingFactor+1)/binSize) ;
    int totalNumberBins = pow(numberBinsPerChannel,image->nchannels);
    Histogram *histogram = createHistogram(totalNumberBins);
    histogram->binSize = binSize;
#pragma omp parallel for
    for (int yp=0; yp < image->ny; yp++){
        for (int xp=0; xp < image->nx; xp++) {
            int index = (yp*image->nx) + xp;
            int binIndex = 0;
            int w = 1;
            for (int cp=0; cp < image->nchannels; cp++) {
                int binIndexOnChannel = image->channel[cp][index]/binSize;
                binIndex += (binIndexOnChannel)*w;
                w *= numberBinsPerChannel;
            }
            histogram->val[binIndex] += 1.0f;
        }
    }
    if(normalization){
        int numberPixels = image->nx*image->ny;
        for (int i = 0; i < histogram->n; ++i) {
            histogram->val[i] /= numberPixels;
        }
    }
    return histogram;
}

FeatureVector* computeHistogramForFeatureVector(Image *image,float binSize, bool normalization){

    int numberBinsPerChannel = ceil((image->scalingFactor+1)/binSize) ;
    int totalNumberBins = pow(numberBinsPerChannel,image->nchannels);
    FeatureVector *histogram = createFeatureVector(totalNumberBins);
#pragma omp parallel for
    for (int yp=0; yp < image->ny; yp++){
        for (int xp=0; xp < image->nx; xp++) {
            int index = (yp*image->nx) + xp;
            int binIndex = 0;
            int w = 1;
            for (int cp=0; cp < image->nchannels; cp++) {
                int binIndexOnChannel = image->channel[cp][index]/binSize;
                binIndex += (binIndexOnChannel)*w;
                w *= numberBinsPerChannel;
            }
            histogram->features[binIndex] += 1.0f;
        }
    }
    if(normalization){
        int numberPixels = image->nx*image->ny;
        for (int i = 0; i < histogram->size; ++i) {
            histogram->features[i] /= numberPixels;
        }
    }
    return histogram;
}

Image *ProbabilityDensityFunction(Image *image, double standardDeviation)
{
    Image *pdf = createImage(image->nx, image->ny,1);
    pdf->scalingFactor = 255; //8-bits
    float  *val = (float *)calloc(image->nx*image->ny,sizeof(float));
    double  K = 2.0*standardDeviation*standardDeviation, maxdist = 3*standardDeviation*3*standardDeviation;
    float   Imax = image->scalingFactor, maxval, minval;
    maxval = INT_MIN; minval = INT_MAX;
#pragma omp parallel for
    for (int p=0; p < image->numberPixels; p++) {
        float *vec_p = (float*)calloc(image->nchannels,sizeof(float));
        float *vec_q = (float*)calloc(image->nchannels,sizeof(float));
        for (int i = 0; i < image->nchannels; ++i) {
            vec_p[i] = image->channel[i][p]/Imax;
        }

        for (int q=0; q < image->numberPixels; q++) {
            if (p != q) {
                double dist = 0;
                for (int i = 0; i < image->nchannels; ++i) {
                    vec_q[i] = image->channel[i][q]/Imax;
                    dist += (vec_p[i] - vec_q[i])*(vec_p[i] - vec_q[i]);
                }
                dist = sqrt(dist);
                if (dist <= maxdist){
                    val[p] += exp(-dist/K);
                }
            }
        }
        free(vec_p);
        free(vec_q);

        if (val[p] > maxval)
            maxval = val[p];
        if (val[p] < minval)
            minval = val[p];
    }

    if (minval != maxval)
        for (int p=0; p < image->numberPixels; p++) {
            pdf->channel[0][p] = (int)(255*(val[p]-minval)/(maxval-minval));
        }
    free(val);

    return(pdf);

}

Histogram* createHistogram(int n){
    Histogram *histogram = (Histogram *) calloc(1,sizeof(Histogram));
    histogram->n = n;
    histogram->val = (float *)calloc(histogram->n,sizeof(float));
    histogram->binSize = 1;
    return histogram;
}