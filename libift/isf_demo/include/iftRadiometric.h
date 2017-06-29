#ifndef IFT_RADIOMETRIC_H_
#define IFT_RADIOMETRIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftColor.h"
#include "iftGQueue.h"
#include "iftHistogram.h"

/* Radiometric Transformations */

iftImage *iftLinearStretch(iftImage *img, double f1, double f2, double g1, double g2);
iftFImage *iftFLinearStretch(iftFImage *img, double f1, double f2, double g1, double g2);
iftImage *iftNormalize(iftImage *img, double minval, double maxval);
iftImage *iftNormalizeWithNoOutliers(iftImage *img, int minval, int maxval);
iftImage *iftWindowAndLevel(iftImage *img, int width, int level, int maxval);
iftImage *iftGaussianStretch(iftImage *img, double mean, double stdev, int maxval);
iftImage *iftExponenStretch(iftImage *img, double f1, double f2, double g1, double g2);
iftImage *iftEqualize(iftImage *img, int max_val);
iftImage *iftMatchHistogram(iftImage *img, iftImage *ref);
iftImage * iftTradMatchHistogram(iftImage *img, iftImage *ref);

/* Radiometric Resolution */
int 	  iftRadiometricResolution(iftImage *img);


#ifdef __cplusplus
}
#endif

#endif
