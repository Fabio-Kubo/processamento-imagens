#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <ostream>

#include "hog.h"

using namespace cv;

HogManager * initializeHogManager(std::vector<Mat> images){
    const int NUMBER_OF_BLOCKS_PER_ROW_IMAGE = 8;
    const int NUMBER_OF_CELLS_PER_ROW_BLOCK = 2;

    HogManager * hogManager = (HogManager *) malloc(sizeof(HogManager));
    hogManager->numberBlockPerRow = NUMBER_OF_BLOCKS_PER_ROW_IMAGE;
    hogManager->numberCellPerRow = NUMBER_OF_CELLS_PER_ROW_BLOCK;

    //TODO percorrer todos os pontos de interesse de cada imagem e extrair

    return hogManager;
}


int getHistogramBaseIndex(HogManager * hogManager, int pixelXAbsolute, int pixelYAbsolute, int cellDimension){
    int numberBins = 9;
    int cellRow = pixelYAbsolute / cellDimension;
    int cellColumn = pixelXAbsolute / cellDimension;

    int cellPosition = cellRow * hogManager->numberCellPerRow + cellColumn;

    return cellPosition * numberBins;
}

std::vector<BlockHistogram *> calculateHog(HogManager * hogManager) {

    int numberBlocksPerImage = hogManager->numberBlockPerRow * hogManager->numberBlockPerRow;
    int numberCellsPerBlock = hogManager->numberCellPerRow * hogManager->numberCellPerRow;
    int cellDimension = hogManager->images[0].blocks[0].rows / hogManager->numberCellPerRow;

    std::vector<BlockHistogram *> blocksHistograms;

    BlockHistogram * newBlockHistogram;

    for (int imageIndex = 0; imageIndex < hogManager->images.size(); imageIndex++) {
        for (int blockIndex = 0; blockIndex < numberBlocksPerImage; blockIndex++) {

            //creates new block Histogram
            newBlockHistogram = (BlockHistogram *) malloc(sizeof(BlockHistogram));
            newBlockHistogram->id = hogManager->images[imageIndex].id;

            // Calculate gradients gx, gy by using image 1-derivative x and y direction
            Mat gx, gy;
            Sobel(hogManager->images[imageIndex].blocks[blockIndex], gx, CV_32F, 1, 0, 1);
            Sobel(hogManager->images[imageIndex].blocks[blockIndex], gy, CV_32F, 0, 1, 1);

            //Calculate gradient magnitude and direction (in degrees)
            Mat magnitude, angle;
            cartToPolar(gx, gy, magnitude, angle, 1);

            //for each pixel, adds its magnitude to its histogram[floor(angle)] and histogram[floor(angle) + 1]
            for (int pixelX = 0; pixelX < hogManager->images[imageIndex].blocks[blockIndex].rows; pixelX++) {
                for (int pixelY = 0; pixelY < hogManager->images[imageIndex].blocks[blockIndex].cols; pixelY++) {
                    int histogramBaseIndex = getHistogramBaseIndex(pixelX, pixelY, cellDimension);

                    int floorAngleIndex = (int) angle.at<float>(pixelX, pixelY) / 20.0;
                    float floorWeigth = (angle.at<float>(pixelX, pixelY) - floorAngleIndex * 20) / 20;
                    float ceilingWeigth = 1 - floorWeigth;

                    newBlockHistogram->histogram->at<float>(blockIndex, histogramBaseIndex + floorAngleIndex) += floorWeigth * magnitude.at<float>(pixelX, pixelY);

                    if(ceilingWeigth != 0){
                        newBlockHistogram->histogram->at<float>(blockIndex, histogramBaseIndex + floorAngleIndex + 1) += ceilingWeigth * magnitude.at<float>(pixelX, pixelY);
                    }
                }
            }
            blocksHistograms.push_back(newBlockHistogram);
        }
    }

    return blocksHistograms;
}
