#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

typedef struct Block {
    std::vector<Mat> cells;
} Block;

typedef struct HogManager{
    std::vector<Block> blocks;
    int numberOfCellsPerBlock;
} HogManager;
