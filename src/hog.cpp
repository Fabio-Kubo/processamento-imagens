#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <ostream>

#include "hog.h"
#include "image.h"

using namespace cv;

Mat * convertImageToHogImage(Image image){
    //TODO converter image to Mat * e aplicar ao final o metodo abaixo
    //images_blocks[i].convertTo(images_blocks[i], CV_32F, 1/255.0);

    return null;
}

/*
Each image should be a Mat instance with float between 0-1 values (CV-32F)
*/
Mat * calculateHog(HogManager * hogManager) {

    Mat * histogram = new Mat( hogManager->blocks.size(), hogManager->numberOfCellsPerBlock * 9, CV_32F);

    for (int blockIndex = 0; blockIndex < hogManager->blocks.size(); blockIndex++) {
        for (int cellIndex = 0; cellIndex < hogManager->numberOfCellsPerBlock; cellIndex++) {

            //get current cell
            Mat * current_cell = &(hogManager->blocks[blockIndex].cells[cellIndex]);
            int cell_histogram_base_index = cellIndex * 9;

            // Calculate gradients gx, gy by using image 1-derivative x and y direction
            Mat gx, gy;
            Sobel(images_blocks[i], gx, CV_32F, 1, 0, 1);
            Sobel(images_blocks[i], gy, CV_32F, 0, 1, 1);

            //Calculate gradient magnitude and direction (in degrees)
            Mat magnitude, angle;
            cartToPolar(gx, gy, magnitude, angle, 1);

            // for each pixel, updates values in histogram
            for (int cell_x = 0; cell_x < current_cell->rows; cell_x++) {
                for (int cell_y = 0; cell_y < current_cell->cols; cell_y++) {

                    int floor_angle_index = (int) angle.at<float>(m,n) / 20.0;
                    float floor_weigth = (angle.at<float>(m,n) - floor_angle_index * 20) / 20;
                    float ceiling_weigth = 1 - floor_weigth;

                    histogram->at<float>(blockIndex, cell_histogram_base_index + floor_angle_index) += floor_weigth * magnitude.at<float>(cell_x, cell_y);

                    if(floor_angule_index != 8){
                        histogram->at<float>(blockIndex, cell_histogram_base_index + floor_angle_index + 1) += ceiling_weigth * magnitude.at<float>(cell_x, cell_y);
                    }
                }
            }
        }
    }

    return histogram;
}

int main(int argc, char** argv )
{

    std::vector<Mat> images;

    images.push_back(imread("data/obj1.png"));
    images.push_back(imread("data/obj2.png"));
    images.push_back(imread("data/obj3.png"));

    //Mat * histogram = calculateHog(images);

    //print Mat content
    //std::cout << "M = "<< std::endl << " "  << *histogram << std::endl << std::endl;

    return 0;
}
