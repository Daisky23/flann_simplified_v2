//
// Created by 张浩天 on 2016/8/30.
//

#ifndef FLANN_V2_RANDOM_H
#define FLANN_V2_RANDOM_H

#include <iostream>
#include <time.h>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#include <windows.h>

#pragma comment( lib,"winmm.lib" )

#define PI 3.14159

namespace flann {
    class Randomer {
    public:
        Randomer() {
            srand((unsigned)timeGetTime());
        }

        static double genUniformRandom(double rangeStart, double rangeEnd) {
            double r;
            r = rangeStart + ((rangeEnd - rangeStart) * (double)rand() / (double)RAND_MAX);
            return r;
        }

        static double genGaussianRandom(){
            // Use Box-Muller transform to generate a point from normal
            // distribution.
            double x1, x2;
            do{
                x1 = genUniformRandom(0.0, 1.0);
            } while (x1 == 0); // cannot take log of 0.
            x2 = genUniformRandom(0.0, 1.0);
            double z;
            z = sqrt(-2.0 * log(x1)) * cos(2.0 * PI * x2);
            return z;
        }

        static unsigned int genRandomUns32(unsigned int rangeStart, unsigned int rangeEnd){
            unsigned int r;
            if (RAND_MAX >= rangeEnd - rangeStart) {
                r = rangeStart + (unsigned int)((rangeEnd - rangeStart + 1.0) * rand() / (RAND_MAX + 1.0));
            } else {
                r = rangeStart + (unsigned int)(
                        (rangeEnd - rangeStart + 1.0) * ((unsigned long long)rand() * ((unsigned long long)RAND_MAX + 1)
                                                         + (unsigned long long)rand())
                                            / ((unsigned long long)RAND_MAX * ((unsigned long long)RAND_MAX + 1) + (unsigned long long)RAND_MAX + 1.0));
            }
            return r;
        }
    };
}


#endif //FLANN_V2_RANDOM_H
