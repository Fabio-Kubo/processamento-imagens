#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include "common.h"
#include "FL.h"

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
