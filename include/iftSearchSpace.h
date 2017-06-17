//
// Created by Alan Peixinho on 9/24/15.
//

#include <stddef.h>

#ifndef IFT_IFTSEARCHSPACE_H
#define IFT_IFTSEARCHSPACE_H


iftDblArray * iftUniformSearchSpace(double begin, double end, double inc);

/**
 * @brief Creates a space search with specific values.
 * @warning Since this function uses undefined number of arguments, you HAVE to pass double values as parameter, not integers.
 * @author Peixinho
 * @param n Number of values to search.
 * @param ... Sequence of values to search.
 */
iftDblArray * iftSearchSpaceValues(int n, ...);
iftDblArray * iftGeometricSearchSpace(double begin, double end, double mul);

#endif //IFT_IFTSEARCHSPACE_H