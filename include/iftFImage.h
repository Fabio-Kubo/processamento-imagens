#ifndef IFT_FIMAGE_H_
#define IFT_FIMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "iftCommon.h"
#include "iftImage.h"
#include "iftAdjacency.h"

#define iftFGetXCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) % (s)->xsize)
#define iftFGetYCoord(s,p) (((p) % (((s)->xsize)*((s)->ysize))) / (s)->xsize)
#define iftFGetZCoord(s,p) ((p) / (((s)->xsize)*((s)->ysize)))
#define iftFGetVoxelIndex(s,v) ((v.x)+(s)->tby[(v.y)]+(s)->tbz[(v.z)])
#define iftFDiagonalSize(s) (iftRound(sqrtf(s->xsize*s->xsize + s->ysize*s->ysize + s->zsize*s->zsize)))

#define iftFImageCenter(img) ((iftVoxel){(img)->xsize/2, (img)->ysize/2, (img)->zsize/2})

typedef struct ift_fimage {
  float *val;
  int    xsize,ysize,zsize;
  float  dx,dy,dz;
  int   *tby, *tbz;
  int    n;
} iftFImage;

/**
 *
 * @brief Verifies if the float images <b>img1</b> and <b>img2</b> are in the same domain
 * @author Deangeli
 * @date May 19, 2016
 *
 *This function may throw an exception if the images have different domains.
 *
 *@param img1 first float image
 *@param img2 second float image
 *@param function function name
 *
 * */
void        iftVerifyFImageDomains(iftFImage *img1, iftFImage *img2, char *function);

/**
 * @brief Verifies if a float image is 3D
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *
 *@return 1 if the image is 3D, 0 otherwise
 * */
char        iftIs3DFImage(const iftFImage *img);

/**
 * @brief Gets the number os pixels on x-axis
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *
 *@return number of pixels on x-axis
 * */
int         iftFXSize(iftFImage *img);

/**
 * @brief Gets the number os pixels on y-axis
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *
 *@return number of pixels on y-axis
 * */
int         iftFYSize(iftFImage *img);

/**
 * @brief Gets the number os pixels on z-axis
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *
 *@return number of pixels on z-axis
 * */
int         iftFZSize(iftFImage *img);

/**
 * @brief Gets the voxel coordinates
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *@param p  voxel index
 *
 *@return a voxel
 * */
iftVoxel    iftFGetVoxelCoord(const iftFImage *img, int p);

/**
 * @brief Generates a copy of <b>img</b>
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param img float image
 *
 *@return a copy of the input image
 * */
iftFImage *iftFCopyImage(const iftFImage *img);

/**
 * @brief Creates a float image
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param xsize number of pixels on x-axis
 *@param ysize number of pixels on y-axis
 *@param zsize number of pixels on z-axis
 *
 *@return a float image
 * */
iftFImage  *iftCreateFImage(int xsize,int ysize,int zsize);

/**
 * @brief Creates a float image
 * @author Deangeli
 * @date May 19, 2016
 *
 *@param xsize number of pixels on x-axis
 *@param ysize number of pixels on y-axis
 *@param zsize number of pixels on z-axis
 *
 *@return a float image
 * */
void iftDestroyFImage(iftFImage **img);

/*
 * @brief Checks if the voxel <b>v</b> is containned in the image <b>img</b>
 * @author Deangeli
 * @date jun 17, 2016

 *@param img image
 *@param v voxel

 *@return 1 if the voxel is in image, 0 otherwise
 * */
char        iftFValidVoxel(iftFImage *img, iftVoxel v);

/*
 * @brief Checks if the point <b>P</b> is containned in the image <b>img</b>
 * @author Deangeli
 * @date jun 17, 2016

 *@param img image
 *@param P point

 *@return 1 if the voxel is in image, 0 otherwise
 * */
char        iftFValidPoint(iftFImage *img, iftPoint P);

/*
 * @brief Returns the maximum value from an image
 * @author Deangeli
 * @date jun 17, 2016

 *@param img image

 *@return The maximum value in the image
 * */
float       iftFMaximumValue(const iftFImage *img);

/*
 * @brief Returns the minimum value from an image
 * @author Deangeli
 * @date jun 17, 2016

 *@param img image

 *@return The minimum value in the image
 * */
float       iftFMinimumValue(const iftFImage *img);

/*
 * @brief Copies the voxel grid size from a image to another
 * @author Deangeli
 * @date jun 17, 2016

 *@param img1 source image
 *@param img1 target image

 * */
void        iftFCopyVoxelSize(const iftFImage *img1, iftFImage *img2);
/**
 * @brief Copies the voxel size from an iftImage to an iftFImage
 *
 * @author Thiago Vallin Spina
 *
 * @param img1 An iftImage
 * @param img2 An iftFImage
 */
void        iftFCopyVoxelSizeFromImage(iftImage *img1, iftFImage *img2);

/**
 * @brief Copies the voxel size from an iftFImage to an iftImage
 *
 * @author Thiago Vallin Spina
 *
 * @param img1 An iftFImage
 * @param img2 An iftImage
 */
void        iftFCopyVoxelSizeToImage(iftFImage *img1, iftImage *img2);

/**
 * @brief Sets all voxel values, from an image, given a value
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img image
 * @param value value to be set
 */

void        iftFSetImage(iftFImage *img, float value);

/**
 * @brief Generates an iftFImage given an iftImage
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img iftImage
 *
 * @return an iftFImage pointer
 */

iftFImage  *iftImageToFImage(const iftImage *img);

/**
 * @brief Generates an iftImage given an iftFImage
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img iftFImage
 *
 * @return an iftImage pointer
 */

iftImage   *iftFImageToImage(const iftFImage *img, int Imax);

/**
 * @brief Generates an iftFImage given an iftFmage and a value
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img iftFImage
 *
 * @return an iftImage pointer
 */
iftFImage *iftImageToFImageMaxVal(const iftImage *img, float fmax);


/**
 * @brief Normalizes the voxels values based on the voxel neighbourhood
 * @author Deangeli
 * @date jun 17, 2016
 *
 *
 *
 * @param img iftFImage
 * @param A neighborhood around a voxel
 *
 * @return a normalized image
 */
iftFImage  *iftFNormalizeImageLocally(iftFImage *img, iftAdjRel *A);

/**
 * @brief Reads a image as a float image, given a file path
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param filename file path
 *
 * @return a float image
 */
iftFImage  *iftFReadImage(const char *filename);

/**
 * @brief Writes a float image
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param filename file path
 *
 */
void iftFWriteImage(iftFImage *img, const char *filename);


/**
 * @brief Gets the slice XY from  a 3D float image, given a Z coordinate.
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img Target image
 * @param zcoord Z axis coordinate.
 * @return The 2D float image slice.
 */
iftFImage  *iftFGetXYSlice(iftFImage *img, int zcoord);

/**
 * @brief Gets the slice ZX from  a 3D float image, given a Y coordinate.
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img Target image
 * @param ycoord Y axis coordinate.
 * @return The 2D float image slice.
 */
iftFImage  *iftFGetZXSlice(iftFImage *img, int ycoord);

/**
 * @brief Gets the slice YZ from  a 3D float image, given a X coordinate.
 * @author Deangeli
 * @date jun 17, 2016
 *
 * @param img Target image
 * @param xcoord X axis coordinate.
 * @return The 2D float image slice.
 */
iftFImage  *iftFGetYZSlice(iftFImage *img, int xcoord);

/**
 * @brief Inserts a 2D float image as a XY slice in a 3D image in a specified Z coordinate.
 *
 * @param img Target image
 * @param slice Inserted 2D image slice
 * @param zcoord Z axis coord.
 */
void        iftFPutXYSlice(iftFImage *img, iftFImage *slice, int zcoord);

/**
 * @brief Inserts a 2D float image as a ZX slice in a 3D image in a specified X coordinate.
 *
 * @param img Target image
 * @param slice Inserted 2D image slice
 * @param zcoord Z axis coord.
 */
void        iftFPutZXSlice(iftFImage *img, iftFImage *slice, int ycoord);

/**
 * @brief Inserts a 2D float image as a ZX slice in a 3D image in a specified X coordinate.
 *
 * @param img Target image
 * @param slice Inserted 2D image slice
 * @param zcoord Z axis coord.
 */
void        iftFPutYZSlice(iftFImage *img, iftFImage *slice, int xcoord);


/**
 * @brief Reads all slices between <b>first</b> and <b>last</b> given a 3D float image basename.
 * @author Deangeli
 * @date jun 17, 2016
 *
 * Gets the slices XY of 3D float image based on the Z coordinate, from <b>first</b> and <b>last</b>.
 *
 * @param basename basename
 * @param first number of the first desired slice in Z-axis
 * @param last number of the last desired slice in Z-axis.
 * @param xsize number of voxels in X-axis
 * @param ysize number of voxels in Y-axis
 * @return a 3D float  image that contains the desired slices.
 */
iftFImage  *iftFReadRawSlices(char *basename, int first, int last, int xsize, int ysize);

void        iftFWriteRawSlices(iftFImage *img, char *basename);

/**
 * @brief Gets the interpolated voxel value given a position.
 *
 * @param img Target float image.
 * @param p position to be interpolated.
 * @return Interpolated brightness value.
 */
float       iftFImageValueAtPoint(iftFImage *img, iftPoint P);


float       iftFImageValueAtPoint2D(iftFImage *img, iftPoint P);


iftImage   *iftAttCoefToHU(iftFImage *attcoef, double mean_of_water);

/**
 * @brief Extract a Region Of Interest (ROI) of an FImage from a Bounding Box.
 * @date Apr 17, 2016
 * @ingroup Image
 * 
 * @param f img FImage whose ROI will be extracted.
 * @param  bb  Bounding Box of the ROI.
 * @return     An ROI FImage.
 *
 * @warning If the ROI Image doesn't fit entirely inside the Target Image, the function extracts what it is possible.
 * @warning The Size of the ROI Image is the the same of the Input Bounding
 */
iftFImage  *iftFExtractROI(const iftFImage *fimg, iftBoundingBox bb);
void        iftFInsertROI(const iftFImage *roi, iftFImage *target, iftVoxel begin);

/**
 * @brief Adds a frame to a float image
 *
 *
 * The <b>sz</b> indicates the thickness of frame in all axes (x,y,z). For example, if we have
 * a 2D image of 5x10 pixels, and  <b>sz</b> is equal to 2, it will results
 * in an output image of 9x14 pixels. The sketch below illustrates the output
 * image generated. The "x" are the pixels values of the image without frame and "V"
 * are the pixels values on frame.
 *
 *
 *
 *  Input                           Output
 *                                 VVVVVVVVVVVVVV
 *                                 VVVVVVVVVVVVVV
 *  xxxxxxxxxx                     VVxxxxxxxxxxVV
 *  xxxxxxxxxx                     VVxxxxxxxxxxVV
 *  xxxxxxxxxx     ----->          VVxxxxxxxxxxVV
 *  xxxxxxxxxx                     VVxxxxxxxxxxVV
 *  xxxxxxxxxx                     VVxxxxxxxxxxVV
 *                                 VVVVVVVVVVVVVV
 *                                 VVVVVVVVVVVVVV
 *
 * @author Deangeli
 * @date May 12, 2016
 * @param img float image where the frame will be added
 * @param sz thickness of frame
 * @param value color component value for all pixel on frame
 *
 * @return Original image on the frame of sz x sz x sz   .
 *
 */
iftFImage  *iftFAddFrame(const iftFImage *img, int sz, float value);


/**
 * @brief Removes a frame from  a float image
 *
 * The <b>sz</b> indicates the thickness of frame (x,y,z). For example, if we have
 * an image of 9x14 pixels, and  <b>sz</b> is equal to 2, it will results
 * in an output image of 5x10 pixels. The sketch below illustrates the output
 * image generated. The "V" are the pixels values on the frame and, "x" are the non-removed  pixels
 * values.
 *
 *  Input                           Output
 * VVVVVVVVVVVVVV
 * VVVVVVVVVVVVVV
 * VVxxxxxxxxxxVV                   xxxxxxxxxx
 * VVxxxxxxxxxxVV                   xxxxxxxxxx
 * VVxxxxxxxxxxVV   ----->          xxxxxxxxxx
 * VVxxxxxxxxxxVV                   xxxxxxxxxx
 * VVxxxxxxxxxxVV                   xxxxxxxxxx
 * VVVVVVVVVVVVVV
 * VVVVVVVVVVVVVV
 *
 * @author Deangeli
 * @date May 12, 2016
 * @param fimg float Image/fimage/etc where the frame will be added
 * @param sz thickness of frame
 * @param value color component value for all pixel on frame
 *
 * @return Image generated by removing a rectangle frame of  sz x sz x sz
 *
 */
iftFImage  *iftFRemFrame(const iftFImage *fimg, int sz);

/**
 * @brief Adds a 3D frame to an float image
 *
 * @author Deangeli
 * @date May 12, 2016
 * @param img float image where the frame will be added
 * @param sx thickness of frame on x-axis
 * @param sy thickness of frame on y-axis
 * @param sz thickness of frame on z-axis
 * @param value color component value for all pixel on frame
 *
 * @return Original image on the frame of sx x sy x sz
 */
iftFImage  *iftFAddRectangularBoxFrame(const iftFImage *img, int sx, int sy, int sz, float value);

/**
 * @brief Removes a 3D frame to a float image
 *
 * @author Deangeli
 * @date May 12, 2016
 * @param fimg float float image where the frame will be added
 * @param sx thickness of frame on x-axis
 * @param sy thickness of frame on y-axis
 * @param sz thickness of frame on z-axis
 *
 * @return Image generated by removing a rectangle frame of  sx x sy x sz
 */
iftFImage  *iftFRemRectangularBoxFrame(const iftFImage *fimg, int sx, int sy, int sz);


/**
 * @brief Returns the Minimum (Image) Bounding Box necessary to cover all non-zero pixels/voxels,
 * and returns its Geometric Center (if <b>gc_out != NULL</b>).
 * @author Samuel Martins
 * @date Apr 17, 2016
 * @ingroup Image
 *
 * E.x: The internal contour is the resulting Minimum Bounding Box, and the character <b>X</b> is its
 * Geometric Center: \n
 * <pre>
 *  ________________ 
 * |                | 
 * |  ___________   | 
 * | |        111|  | 
 * | |        222|  | 
 * | |           |  | 
 * | |      X 333|  | 
 * | |222_____333|  | 
 * |                | 
 * |____________ ___|
 * </pre>
 *
 * @param img Target FImage used to extract the Bounding Box.
 * @param gc_out Return the Geometric Center from the Bounding Box to this Reference. If it is NULL, nothing is returned to it.
 * @return The Minimum (FImage) Bounding Box.
 *
 * @warning If <b>gc_out</b> is <b>NULL</b>, the Geometric Center is not Returned.
 */
iftBoundingBox iftFMinBoundingBox(const iftFImage *fimg, iftPoint *gc_out);


/**
 * @brief Shifts a Float Image according to given displacements.
 * @author Samuel Martins
 * @date Jul 5, 2016
 * 
 * @param  fimg Float Image to be Shifted.
 * @param  dx   Displacement in x-axis.
 * @param  dy   Displacement in y-axis.
 * @param  dz   Displacement in z-axis.
 * @return      The resulting Shifted Image.
 */
iftFImage *iftFShiftImage(const iftFImage *fimg, int dx, int dy, int dz);


#ifdef __cplusplus
}
#endif

#endif


