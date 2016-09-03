//
// Created by 张浩天 on 2016/8/21.
//

#ifndef FLANN_MATRIX_H
#define FLANN_MATRIX_H

#include "../general.h"
#include <stdio.h>

namespace flann {

    typedef unsigned char uchar;

    class Matrix_
    {
    public:

        Matrix_() : rows(0), cols(0), stride(0), type(FLANN_NONE), data(NULL)
        {
        };

        Matrix_(void* data_, size_t rows_, size_t cols_, flann_datatype_t type_, size_t stride_ = 0) :
                rows(rows_), cols(cols_), stride(stride_), type(type_)
        {
            data = static_cast<uchar*>(data_);

            if (stride==0) stride = flann_datatype_size(type)*cols;
        }

        /**
         * Operator that returns a (pointer to a) row of the data.
         */
        inline void* operator[](size_t index) const
        {
            return data+index*stride;
        }

        void* ptr() const
        {
            return data;
        }

        size_t rows;
        size_t cols;
        size_t stride;
        flann_datatype_t type;

    protected:
        uchar* data;

    };

    template <typename T>
    class Matrix : public Matrix_ {
    public:
        typedef T type;

        Matrix() : Matrix_() {}

        Matrix(T* data_, size_t rows_, size_t cols_, size_t stride_ = 0) :
                Matrix_(data_, rows_, cols_, flann_datatype_value<T>::value, stride_) {
            if (stride==0) stride = sizeof(T)*cols;
        }

        /**
         * Operator that returns a (pointer to a) row of the data.
         */
        inline T* operator[](size_t index) const {
            return reinterpret_cast<T*>(data+index*stride);
        }


        T* ptr() const {
            return reinterpret_cast<T*>(data);
        }
    };
}

#endif //FLANN_MATRIX_H
