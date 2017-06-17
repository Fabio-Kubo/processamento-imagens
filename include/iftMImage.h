#ifndef IFT_MIMAGE_H_
#define IFT_MIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iftCommon.h"
#include "iftImage.h"
#include "iftFImage.h"
#include "iftColor.h"
#include "iftAdjacency.h"


#define iftMGetXCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) % (s)->xsize)
#define iftMGetYCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) / (s)->xsize)
#define iftMGetZCoord(s,p) ((p) / (((s)->xsize)*((s)->ysize)))
#define iftMGetVoxelIndex(s,v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define iftMDiagonalSize(s) (iftRound(sqrtf(s->xsize*s->xsize + s->ysize*s->ysize + s->zsize*s->zsize)))

/* Multiband image: iftMImage *img; img->band[b].val[p]  */

typedef struct ift_mimage {
  iftBand *band; /* a list of images, one for each band */
  int      xsize,ysize,zsize; /* image dimensions of each band */
  float    dx,dy,dz;  /* voxel size */
  int     *tby, *tbz; /* LUT to speed up index to/from coordinate conversions */
  int      n,m; /* number of voxels and number of bands */
} iftMImage;

  int         iftMXSize(iftMImage *img);
  int         iftMYSize(iftMImage *img);
  int         iftMZSize(iftMImage *img);
  iftVoxel    iftMGetVoxelCoord(iftMImage *img, int p);
  iftMImage  *iftCreateMImage(int xsize,int ysize,int zsize, int nbands);
  void        iftDestroyMImage(iftMImage **img);
  iftMImage  *iftCopyMImage(iftMImage *img);
  char        iftMValidVoxel(iftMImage *img, iftVoxel v);
  void        iftMCopyVoxelSize(iftMImage *img1, iftMImage *img2);
/**
 * @brief Copies the voxel size from an iftImage to an iftMImage
 *
 * @author Thiago Vallin Spina
 *
 * @param img1 An iftImage.
 * @param img2 An iftMImage.
 */
  void        iftMCopyVoxelSizeFromImage(iftImage *img1, iftMImage *img2);
/**
 * @brief Copies the voxel size from an iftMImage to an iftImage
 *
 * @author Thiago Vallin Spina
 *
 * @param img1 An iftMImage.
 * @param img2 An iftImage.
 */
  void        iftMCopyVoxelSizeToImage(iftMImage *img1, iftImage *img2);
  iftMImage  *iftImageToMImage(const iftImage *img, char color_space); /* See options for color_space in iftColor.h */
  iftImage   *iftMImageToImage(iftMImage *img, int Imax, int band);
  char        iftIs3DMImage(iftMImage *img);
  
  iftMImage   *iftReadMImage(const char *filename);
  void  	     iftWriteMImage(iftMImage *mimg, const char *filename);
  void        iftWriteMImageBands(iftMImage *mimg, char *base_filename);
  
  iftMImage  *iftMAddFrame(iftMImage *img, int bx, int by, int bz, float value);
  iftMImage  *iftMRemFrame(iftMImage *fimg, int bx, int by, int bz);
  void        iftSetMImage(iftMImage *img, float value);
  iftImage   *iftEuclMImageBasins(iftMImage *img, iftAdjRel *A);
  iftImage   *iftMImageBasins(iftMImage *img, iftAdjRel *A);
  iftImage   *iftBorderProbImage(iftMImage *img);
  iftImage   *iftRegionProbImage(iftMImage *img);
  iftImage   *iftUniformProbImage(iftMImage *img);
  void        iftMultMImageByScalar(iftMImage *Z, float scalar);
  
  iftMImage  *iftGradientVector(iftImage *img, iftImage *mask, iftAdjRel *A);
  
  /* Voxel sampling methods that return a binary image with 0/1 value,
     by using the border information to avoid outliers. */

  iftImage *iftSelectNonBorderVoxels(iftMImage *img, iftImage *mask, int nsamples);
  iftImage *iftGridSampling(iftMImage *img, iftImage *mask, int nsamples);
  iftImage *iftAltMixedSampling(iftMImage *img, iftImage *mask, int nsamples);
  iftImage *iftSelectNSamplesFromMask(iftMImage *img, iftImage *mask1, int nsamples);

  // If band is negative then the function will search for the maximum value among all bands
  float iftMMaximumValue(iftMImage *img, int band);
  // If band is negative then the function will search for the minimum value among all bands
  float iftMMinimumValue(iftMImage *img, int band);
  
#ifdef __cplusplus
}
#endif

#endif


