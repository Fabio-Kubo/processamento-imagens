#include <stdio.h>
#include <opencv2/opencv.hpp>

typedef struct Block {
    std::vector<Mat*> cells;
} Block;

typdef struct HogManager{
    std::vector<Blocks> blocks;
    int numberOfCellsPerBlock;
} HogManager;
