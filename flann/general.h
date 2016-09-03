//
// Created by 张浩天 on 2016/8/21.
//

#ifndef FLANN_GENERAL_H
#define FLANN_GENERAL_H

#include "defines.h"
#include <stdexcept>
#include <cassert>
#include <limits.h>

namespace flann
{

    class FLANNException : public std::runtime_error
    {
    public:
        FLANNException(const char* message) : std::runtime_error(message) { }

        FLANNException(const std::string& message) : std::runtime_error(message) { }
    };


    template <typename T>
    struct flann_datatype_value
    {
        static const flann_datatype_t value = FLANN_NONE;
    };

    template<>
    struct flann_datatype_value<char>
    {
        static const flann_datatype_t value = FLANN_INT8;
    };

    template<>
    struct flann_datatype_value<short>
    {
        static const flann_datatype_t value = FLANN_INT16;
    };

    template<>
    struct flann_datatype_value<int>
    {
        static const flann_datatype_t value = FLANN_INT32;
    };

#ifdef LLONG_MAX
    template<>
struct flann_datatype_value<long long>
{
	static const flann_datatype_t value = FLANN_INT64;
};
#endif

    template<>
    struct flann_datatype_value<unsigned char>
    {
        static const flann_datatype_t value = FLANN_UINT8;
    };

    template<>
    struct flann_datatype_value<unsigned short>
    {
        static const flann_datatype_t value = FLANN_UINT16;
    };

    template<>
    struct flann_datatype_value<unsigned int>
    {
        static const flann_datatype_t value = FLANN_UINT32;
    };

#ifdef ULLONG_MAX
    template<>
struct flann_datatype_value<unsigned long long>
{
	static const flann_datatype_t value = FLANN_UINT64;
};
#endif


    template<>
    struct flann_datatype_value<float>
    {
        static const flann_datatype_t value = FLANN_FLOAT32;
    };

    template<>
    struct flann_datatype_value<double>
    {
        static const flann_datatype_t value = FLANN_FLOAT64;
    };



    template <flann_datatype_t datatype>
    struct flann_datatype_type
    {
        typedef void type;
    };

    template<>
    struct flann_datatype_type<FLANN_INT8>
    {
        typedef char type;
    };

    template<>
    struct flann_datatype_type<FLANN_INT16>
    {
        typedef short type;
    };

    template<>
    struct flann_datatype_type<FLANN_INT32>
    {
        typedef int type;
    };

#ifdef LLONG_MAX
    template<>
struct flann_datatype_type<FLANN_INT64>
{
	typedef long long type;
};
#endif

    template<>
    struct flann_datatype_type<FLANN_UINT8>
    {
        typedef unsigned char type;
    };


    template<>
    struct flann_datatype_type<FLANN_UINT16>
    {
        typedef unsigned short type;
    };

    template<>
    struct flann_datatype_type<FLANN_UINT32>
    {
        typedef unsigned int type;
    };

#ifdef ULLONG_MAX
    template<>
struct flann_datatype_type<FLANN_UINT64>
{
	typedef unsigned long long type;
};
#endif

    template<>
    struct flann_datatype_type<FLANN_FLOAT32>
    {
        typedef float type;
    };

    template<>
    struct flann_datatype_type<FLANN_FLOAT64>
    {
        typedef double type;
    };


    inline size_t flann_datatype_size(flann_datatype_t type)
    {
        switch (type) {
            case FLANN_INT8:
                return sizeof(flann_datatype_type<FLANN_INT8>::type);
            case FLANN_INT16:
                return sizeof(flann_datatype_type<FLANN_INT16>::type);
            case FLANN_INT32:
                return sizeof(flann_datatype_type<FLANN_INT32>::type);
            case FLANN_INT64:
                return sizeof(flann_datatype_type<FLANN_INT64>::type);
            case FLANN_UINT8:
                return sizeof(flann_datatype_type<FLANN_UINT8>::type);
            case FLANN_UINT16:
                return sizeof(flann_datatype_type<FLANN_UINT16>::type);
            case FLANN_UINT32:
                return sizeof(flann_datatype_type<FLANN_UINT32>::type);
            case FLANN_UINT64:
                return sizeof(flann_datatype_type<FLANN_UINT64>::type);
            case FLANN_FLOAT32:
                return sizeof(flann_datatype_type<FLANN_FLOAT32>::type);
            case FLANN_FLOAT64:
                return sizeof(flann_datatype_type<FLANN_FLOAT64>::type);
            default:
                return 0;
        }
    }

}

#endif //FLANN_GENERAL_H