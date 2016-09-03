//
// Created by 张浩天 on 2016/8/21.
//

#ifndef FLANN_DEFINES_H
#define FLANN_DEFINES_H

/* Nearest neighbour index algorithms */
enum flann_algorithm_t
{
    FLANN_INDEX_LSH 			= 6,

    FLANN_INDEX_SAVED 			= 254,
    FLANN_INDEX_AUTOTUNED 		= 255,
};

enum flann_distance_t
{
    FLANN_DIST_EUCLIDEAN 			= 1,
    FLANN_DIST_L2 					= 1,
    FLANN_DIST_MANHATTAN 			= 2,
    FLANN_DIST_L1 					= 2,
    FLANN_DIST_MINKOWSKI 			= 3,
    FLANN_DIST_MAX   				= 4,
    FLANN_DIST_HIST_INTERSECT  		= 5,
    FLANN_DIST_HELLINGER 			= 6,
    FLANN_DIST_CHI_SQUARE		 	= 7,
    FLANN_DIST_KULLBACK_LEIBLER  	= 8,
    FLANN_DIST_HAMMING         		= 9,
    FLANN_DIST_HAMMING_LUT			= 10,
    FLANN_DIST_HAMMING_POPCNT   	= 11,
    FLANN_DIST_L2_SIMPLE	   		= 12,
};

enum flann_datatype_t
{
    FLANN_NONE 		= -1,
    FLANN_INT8 		= 0,
    FLANN_INT16 	= 1,
    FLANN_INT32 	= 2,
    FLANN_INT64 	= 3,
    FLANN_UINT8 	= 4,
    FLANN_UINT16 	= 5,
    FLANN_UINT32 	= 6,
    FLANN_UINT64 	= 7,
    FLANN_FLOAT32 	= 8,
    FLANN_FLOAT64 	= 9
};

enum flann_checks_t {
    FLANN_CHECKS_UNLIMITED = -1,
    FLANN_CHECKS_AUTOTUNED = -2,
};

#endif //FLANN_DEFINES_H