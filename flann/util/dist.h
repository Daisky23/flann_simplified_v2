//
// Created by 张浩天 on 2016/8/22.
//

#ifndef FLANN_DIST_H
#define FLANN_DIST_H

#include <cmath>
#include <cstdlib>
#include <string.h>
#include <stdint.h>
#include <mex.h>

#include "../defines.h"

namespace flann {

    struct Distance
    {
        typedef double ElementType;
        typedef double ResultType;

        ResultType operator()(const double * a, const double * b, size_t size) const
        {
            ResultType result = 0;
            ResultType diff;
            for(size_t i = 0; i < size; ++i ) {
                diff = *a++ - *b++;
                result += diff*diff;
            }
            return sqrt(result);
        }

    };

}

#endif //FLANN_DIST_H
