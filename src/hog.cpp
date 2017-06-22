#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <ostream>

using namespace cv;

Mat * calculateHog(std::vector<Mat> images_blocks) {

    Mat * histogram = new Mat(images_blocks.size(), 9, CV_32F);

    for (int i = 0; i < images_blocks.size(); i++) {
        images_blocks[i].convertTo(images_blocks[i], CV_32F, 1/255.0);

        // Calculate gradients gx, gy
        Mat gx, gy;

        //calculates image 1-derivative x
        Sobel(images_blocks[i], gx, CV_32F, 1, 0, 1);

        //calculates image 1-derivative y
        Sobel(images_blocks[i], gy, CV_32F, 0, 1, 1);

        //Calculate gradient magnitude and direction (in degrees)
        Mat magnitude, angle;
        cartToPolar(gx, gy, magnitude, angle, 1);

        for (int m = 0; m < images_blocks[i].rows; m++) {
          for (int n = 0; n < images_blocks[i].cols; n++) {

              int floor_angule_index = (int) angle.at<float>(m,n) / 20.0;
              float floor_weigth = (angle.at<float>(m,n) - floor_angule_index * 20) / 20;
              float ceiling_weigth = 1 - floor_weigth;

              histogram->at<float>(i,floor_angule_index) += floor_weigth * magnitude.at<float>(m,n);

              if(floor_angule_index != 8){
                  histogram->at<float>(i,floor_angule_index + 1) += ceiling_weigth * magnitude.at<float>(m,n);
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

    Mat * histogram = calculateHog(images);

    //print Mat content
    std::cout << "M = "<< std::endl << " "  << *histogram << std::endl << std::endl;

    return 0;
}
