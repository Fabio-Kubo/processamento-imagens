  //
// Created by deangeli on 5/20/17.
//

#ifndef _SAMPLING_H
#define _SAMPLING_H

#include "vector.h"
#include "image.h"

GVector* gridSampling(Image* image, size_t patchSizeX,size_t patchSizeY);

GVector* randomSampling_noImage(Image* image, size_t numberOfPatchs, size_t patchSizeX,size_t patchSizeY,int seed);

#endif //LIBFL_SAMPLING_H
