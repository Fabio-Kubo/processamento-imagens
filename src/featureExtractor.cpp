//
// Created by deangeli on 5/20/17.
//

#include "featureExtractor.h"

Matrix* computeColorHistogram(GVector* vector_images,size_t nbinsPerChannel,size_t totalNumberBins){
    Matrix* matrix = createMatrix(vector_images->size,totalNumberBins,sizeof(float));
    int k = 0;
    for (size_t i = 0; i < vector_images->size; ++i) {
        Image* patch = VECTOR_GET_ELEMENT_AS(Image*,vector_images,i);
        GVector* featureVector = computeHistogramForFeatureVectorGivenNBins(patch,nbinsPerChannel,true);
        for (size_t j = 0; j < matrix->numberColumns; ++j) {
            MATRIX_GET_ELEMENT_BI_AS(float,matrix,k) = VECTOR_GET_ELEMENT_AS(float,featureVector,j);
            k++;
        }
        destroyVector(&featureVector);
    }
    return matrix;
}

HogManager* createHogManager() {
    //printf("[createHogManager] creating hog manager.\n");
    HogManager* hogManager = (HogManager*)calloc(1,sizeof(HogManager));
    hogManager->binSize = 20;
    hogManager->cellSizeX = 8;
    hogManager->cellSizeY = 8;
    hogManager->cellsPerBlockX = 2;
    hogManager->cellsPerBlockY = 2;
    hogManager->gradientImageMagnitude = NULL;
    hogManager->gradientImagePhase = NULL;
    hogManager->image = NULL;
    hogManager->strideX = hogManager->cellSizeX;
    hogManager->strideY = hogManager->cellSizeY;
    hogManager->useUnsignedGradients = false;
    hogManager->fitBlock = false;
    hogManager->computeGradient = true;
    hogManager->hogFeatureVectorSize = 0;
    hogManager->hogFeatureVector = NULL;
    return hogManager;
}



void compudeGradientImage(HogManager* hogManager){

    Kernel *kernelX = createSobelHorizontalGradientKernel(3,3);
    Kernel *kernelY = createSobelVerticalGradientKernel(3,3);

    Image *sobelX = convolveImageWithKernel(hogManager->image, kernelX, false, false);
    Image *sobelY = convolveImageWithKernel(hogManager->image, kernelY, false, false);


    Image* gradientMagnitude = createImage(sobelX->nx,sobelX->ny,1,sobelX->nchannels);
    Image* gradientphase = createImage(sobelX->nx,sobelX->ny,1,sobelX->nchannels);

    if(hogManager->useUnsignedGradients){

        for (int k = 0; k < sobelX->numberPixels; ++k) {
            double gradientMagnitudeValue = (sobelX->channel[0][k]*sobelX->channel[0][k]) +
                                            (sobelY->channel[0][k]*sobelY->channel[0][k]);

            gradientMagnitudeValue = (float)sqrt(gradientMagnitudeValue);
            double angle = atan2(sobelY->channel[0][k],sobelX->channel[0][k]);
            if(angle < 0){
                angle = M_PI + angle;
            }

            angle *= 180/M_PI; //rad2degree

            gradientMagnitude->channel[0][k] = gradientMagnitudeValue;
            gradientphase->channel[0][k] = angle;
        }

    }else{

        for (int k = 0; k < sobelX->numberPixels; ++k) {
            double gradientMagnitudeValue = (sobelX->channel[0][k]*sobelX->channel[0][k]) +
                                            (sobelY->channel[0][k]*sobelY->channel[0][k]);

            gradientMagnitudeValue = (float)sqrt(gradientMagnitudeValue);
            double angle = atan2(sobelY->channel[0][k],sobelX->channel[0][k]);
            if(angle < 0){
                angle = 2.0*M_PI + angle;
            }
            angle *= 180/M_PI; //rad2degree

            gradientMagnitude->channel[0][k] = gradientMagnitudeValue;
            gradientphase->channel[0][k] = angle;
        }

    }

    destroyKernel(&kernelX);
    destroyKernel(&kernelY);
    destroyImage(&sobelX);
    destroyImage(&sobelY);

    hogManager->gradientImageMagnitude = gradientMagnitude;
    hogManager->gradientImagePhase = gradientphase;
}

void removeNegativeROIImages(GVector * images) {
    Image * image;

    for (int i = 0; i < images->size; ++i) {
        image = VECTOR_GET_ELEMENT_AS(Image *, images, i);
        if (image->imageROI.coordinateX < 0 || image->imageROI.coordinateY < 0) {
            removeElementInVectorAt(images, i);
            i--; // verifica de novo a posicao que teve imagem removida
        }
    }
}

Matrix* computeHogDescriptorForRegionsOfInterest(GVector* vector_ROIs, HogManager* hogManager){
    //Matrix* matrix = createMatrix(vector_ROIs->size,totalNumberBins,sizeof(float));
    GVector* featureVector_hog = NULL;
    int shift = 0;
    size_t nrows = vector_ROIs->size;
    size_t ncols = 0;
    for (int i = 0; i < vector_ROIs->size; ++i) {
        Image* workImage = hogManager->image;
        Image *subImage = VECTOR_GET_ELEMENT_AS(Image*,vector_ROIs,i);
        RegionOfInterest regionOfInterest = subImage->imageROI;
        //RegionOfInterest *regionOfInterest =  VECTOR_GET_ELEMENT_AS(RegionOfInterest*, vector_ROIs, i);
        workImage->imageROI = regionOfInterest;
        //printf("%d\n", workImage->imageROI.coordinateX);
        computeHogDescriptor(hogManager);
        if(featureVector_hog){
            float* data = (float*)featureVector_hog->data;
            appendVectorsSameType_macro(float,data,hogManager->hogFeatureVector,shift, hogManager->hogFeatureVectorSize);
            shift += hogManager->hogFeatureVectorSize;
            free(hogManager->hogFeatureVector);
            hogManager->hogFeatureVector = NULL;
            //VECTOR_PRINT_AS(double, "%f ",featureVector_hog);
        }else{
            ncols = hogManager->hogFeatureVectorSize;
            featureVector_hog = createNullVector(ncols*nrows,sizeof(float));
            float* data = (float*)featureVector_hog->data;
            appendVectorsSameType_macro(float,data,hogManager->hogFeatureVector,shift, ncols);
            shift += ncols;
            free(hogManager->hogFeatureVector);
            hogManager->hogFeatureVector = NULL;
            //VECTOR_PRINT_AS(double, "%f ",featureVector_hog);
        }
    }
    Matrix* matrix = createMatrix(nrows,ncols,sizeof(float));
    destroyVector(&(matrix->matrixData));
    matrix->matrixData = featureVector_hog;
    // printf("%d %d\n", matrix->numberRows, matrix->numberColumns);
    //MATRIX_PRINT_AS(float,"%f ",matrix);
    return matrix;
}

void computeHogDescriptor(HogManager* hogManager){
    float range =  (hogManager->useUnsignedGradients)? 180 : 360;
    int nBins = range/hogManager->binSize;
    float *binsCenter = (float*)calloc(nBins,sizeof(float));

    float value = 0;
    for (int i = 0; i < nBins; ++i) {
        binsCenter[i] = value;
        value += hogManager->binSize;
    }

    if(hogManager->image == NULL){
        printf("[computeHogDescriptor] could not perform hog. Image is empty\n");
        return;
    }


    if(hogManager->gradientImageMagnitude == NULL){
        compudeGradientImage(hogManager);
    } else if(areImagesSameDimension(hogManager->image,hogManager->gradientImageMagnitude)){
        destroyImage(&hogManager->gradientImageMagnitude);
        destroyImage(&hogManager->gradientImagePhase);
        compudeGradientImage(hogManager);
    } else if(hogManager->computeGradient){
        destroyImage(&hogManager->gradientImageMagnitude);
        destroyImage(&hogManager->gradientImagePhase);
        compudeGradientImage(hogManager);
    }


//    printImageInfo(hogManager->image);
//    printImageInfo(hogManager->gradientImageMagnitude);
//    printImageInfo(hogManager->gradientImagePhase);

    Image* workImage = hogManager->image;
    int startY = workImage->imageROI.coordinateY;
    int startX = workImage->imageROI.coordinateX;
    int height = workImage->imageROI.coordinateY+workImage->imageROI.size_y;
    if(height > hogManager->image->ny){
        height = hogManager->image->ny;
    }
    int width = workImage->imageROI.coordinateX+workImage->imageROI.size_x;
    if(width > hogManager->image->nx){
        width = hogManager->image->nx;
    }
    int blockSizeX = hogManager->cellsPerBlockX*hogManager->cellSizeX;
    int blockSizeY = hogManager->cellsPerBlockY*hogManager->cellSizeY;
    int strideX = hogManager->strideX;
    int strideY = hogManager->strideY;

    int y_cell_start = 0;
    int x_cell_start = 0;
    int y_cell_end = 0;
    int x_cell_end = 0;

    int y_block_start = 0;
    int x_block_start = 0;
    int y_block_end = 0;
    int x_block_end = 0;

    float phaseValueDegree;
    float binIndex_float;
    int firstCloseIndex;
    int secondCloseIndex;
    float factor1;
    float factor2;
    float distance1;
    float distance2;
    float *histogramCell = NULL;



    y_block_start = startY;
    x_block_start = startX;
    float *hogFeatureVector = NULL;
    int hogFeatureVectorSize = 0;

    //for each block
    while(y_block_start < height && x_block_start < width){
        // printf("[computeHogDescriptor] block [%d %d]\n",x_block_start,y_block_start);

        if(x_block_start + blockSizeX > width || y_block_start + blockSizeY > height){
            // printf("[computeHogDescriptor] skip block [%d %d]\n",x_block_start,y_block_start);
            x_block_start += strideX;
            if(x_block_start >= width){
                x_block_start = startX;
                y_block_start += strideY;
            }
            continue;
        }

        y_block_end = y_block_start + blockSizeY;
        x_block_end = x_block_start + blockSizeY;

        x_cell_start = x_block_start;
        y_cell_start = y_block_start;
        float *histogramBlock = NULL;
        int sizeHistogramBlock = 0;
        while(y_cell_start < y_block_end && x_cell_start < x_block_end){
            x_cell_end = x_cell_start + hogManager->cellSizeX;
            y_cell_end = y_cell_start +  hogManager->cellSizeY;
            // printf("[computeHogDescriptor] cell [%d %d]\n",x_cell_start,y_cell_start);
            histogramCell = (float*)calloc(nBins,sizeof(float));

            //compute histogram for cell
            for (int coordY_cell =  y_cell_start;  coordY_cell < y_cell_end; coordY_cell++) {
                for (int coordX_cell =  x_cell_start;  coordX_cell < x_cell_end; coordX_cell++) {

                    phaseValueDegree = imageVal(hogManager->gradientImagePhase,coordX_cell,coordY_cell);
                    binIndex_float = phaseValueDegree/hogManager->binSize;
                    firstCloseIndex = (int)(binIndex_float - 0.000001);
                    if(binIndex_float - firstCloseIndex <= 0.5){
                        secondCloseIndex = firstCloseIndex-1;
                        if(secondCloseIndex < 0){
                            secondCloseIndex = nBins-1;
                        }
                    }else{
                        secondCloseIndex = firstCloseIndex+1;
                        if(secondCloseIndex >= nBins){
                            secondCloseIndex = 0;
                        }
                    }
                    distance1 = phaseValueDegree-binsCenter[firstCloseIndex];
                    if(distance1 < 0){
                        distance1 = -distance1;
                    }

                    distance2 = phaseValueDegree-binsCenter[secondCloseIndex];
                    if(distance2 < 0){
                        distance2 = -distance2;
                    }
                    factor1 = distance2/(hogManager->binSize);
                    factor2 = distance1/(hogManager->binSize);


                    histogramCell[firstCloseIndex] += factor1*imageVal(hogManager->gradientImageMagnitude,coordX_cell,coordY_cell);
                    histogramCell[secondCloseIndex] += factor2*imageVal(hogManager->gradientImageMagnitude,coordX_cell,coordY_cell);
                }
            }

            //normalization on cell histogram
            double histogramMagnitudeCell = 0;
            for (int i = 0; i < nBins; ++i) {
                histogramMagnitudeCell += histogramCell[i]*histogramCell[i];
            }

            histogramMagnitudeCell = sqrt(histogramMagnitudeCell);
            for (int i = 0; i < nBins; ++i) {
                if (histogramMagnitudeCell <= 0) {
                    histogramCell[i] = 0;
                    continue;
                }
                histogramCell[i] /= histogramMagnitudeCell;
            }

            //merging cells hitograms
            float *histogramAux = NULL;
            mergeVectorsSameType_macro(float, histogramBlock,sizeHistogramBlock,histogramCell,nBins,histogramAux);
            free(histogramCell);
            free(histogramBlock);
            histogramBlock = histogramAux;
            sizeHistogramBlock += nBins;

            x_cell_start += hogManager->cellSizeX;
            if(x_cell_start >= x_block_end){
                x_cell_start = x_block_start;
                y_cell_start += strideY;
            }
        }
        //normalization on block histogram
        double histogramMagnitudeBlock = 0;
        for (int i = 0; i < sizeHistogramBlock; ++i) {
            histogramMagnitudeBlock += histogramBlock[i]*histogramBlock[i];
        }

        histogramMagnitudeBlock = sqrt(histogramMagnitudeBlock);
        for (int i = 0; i < sizeHistogramBlock; ++i) {
            if (histogramMagnitudeBlock <= 0) {
                histogramBlock[i] = 0;
                continue;
            }
            histogramBlock[i] /= histogramMagnitudeBlock;
        }


        //merging block hitograms
        float *featureVectorAux = NULL;
        mergeVectorsSameType_macro(float, hogFeatureVector, hogFeatureVectorSize,histogramBlock,sizeHistogramBlock,featureVectorAux);
        free(hogFeatureVector);
        free(histogramBlock);
        hogFeatureVector = featureVectorAux;
        hogFeatureVectorSize += sizeHistogramBlock;

        x_block_start += strideX;
        if(x_block_start >= width){
            x_block_start = startX;
            y_block_start += strideY;
        }
    }

    hogManager->hogFeatureVector = hogFeatureVector;
    hogManager->hogFeatureVectorSize = hogFeatureVectorSize;
    free(binsCenter);
}

void destroyHogManager(HogManager** pHogManager){
    if(pHogManager == NULL){
        return;
    }
    HogManager* aux = *pHogManager;
    if(aux == NULL){
        return;
    }
    /*if(aux->image){
        destroyImage(&(aux->image));
    }*/
    if(aux->gradientImagePhase){
        destroyImage(&(aux->gradientImagePhase));
    }
    if(aux->gradientImageMagnitude){
        destroyImage(&(aux->gradientImageMagnitude));
    }
    if(aux->hogFeatureVector){
        free(aux->hogFeatureVector);
    }
    free(*pHogManager);
    (*pHogManager) = NULL;
}



