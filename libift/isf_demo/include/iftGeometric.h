#ifndef IFT_GEOMETRIC_H_
#define IFT_GEOMETRIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iftCommon.h"
#include "iftImage.h"
#include "iftMatrix.h"
#include "iftInterpolation.h"
#include "iftDataSet.h"

iftImage  *iftRotateImage2D(iftImage *img, float theta);
iftImage  *iftScaleImage2D(iftImage *img, float sx, float sy);
/**
 * @brief Scales an image using nearest-neighbor interpolation.
 *
 * @author Thiago Vallin Spina
 * @date Feb 15, 2016
 *
 * @warning This function only works for 2D images
 *
 * @param img The input image to be scaled.
 * @param sx The scale in the X dimension.
 * @param sy The scale in the Y dimension.
 *
 * @return The interpolated image.
 *
 */
iftImage  *iftScaleImage2DByNearestNeighbor(iftImage *img, float sx, float sy);

iftImage  *iftRotateImage(iftImage *img, float theta_x, float theta_y);
iftImage  *iftScaleImage(iftImage *img, float sx, float sy, float sz);
iftImage  *iftFlipImage(iftImage *img, char axis); /* IFT_AXIS_X, IFT_AXIS_Y, or AXIS_Z */
iftImage  *iftTransformImageClipping(iftImage *img, iftMatrix *InvE, int xsize, int ysize, int zsize);
iftFImage *iftTransformFImageClipping(iftFImage *img, iftMatrix *InvE, int xsize, int ysize, int zsize);
iftImage  *iftTransformImageClip(iftImage *img, iftMatrix *M);

#ifdef __cplusplus
}
#endif

#endif
