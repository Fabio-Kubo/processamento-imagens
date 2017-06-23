#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

typedef struct BlockHistogram {
    Mat * histogram;
    int id;
    int predicted_id;
} BlockHistogram;

typedef struct HogImage {
    std::vector<Mat> blocks;
    int id;
} HogImage;

typedef struct HogManager{
    std::vector<HogImage> images;
    int numberBlockPerRow;
    int numberCellPerRow;
} HogManager;
