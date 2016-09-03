//
// Created by 张浩天 on 2016/9/1.
//

#include "../util/matrix.h"
#include "../util/lsh_index.h"
#include <string>
#include <iostream>
#include <mex.h>
#include <afxres.h>
#include <windows.h>
using namespace std;

#pragma comment( lib,"winmm.lib" )

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    std::string filename;
    flann::IndexParams indexParams = flann::IndexParams();
    int k = 20000;

    unsigned long t1, t2;

    if (nrhs != 3) {
        if (nrhs != 4) {
            mexPrintf("Usage: loadIndexAndQuery(index_file_name, base_dataset, query_set, [Params: knn])\n");
            return;
        }
        k = (int)(*mxGetPr(prhs[3]));
    }

    filename = mxArrayToString(prhs[0]);

    double *datasetMat = mxGetPr(prhs[1]);
    int veclen = mxGetM(prhs[1]);
    int size = mxGetN(prhs[1]);
    const flann::Matrix<double> dataset(datasetMat, size, veclen);

    mexPrintf("Reading Dataset...\n");
    t1 = timeGetTime();
    flann::Index index(dataset, indexParams);
    t2 = timeGetTime();
    mexPrintf("Reading Complete, Time Consuming: %d(ms)\n\n", (t2 - t1));

    mexPrintf("Loading Index From File \"%s\"...\n", filename);
    t1 = timeGetTime();
    index.loadIndexFromFile(filename);
    t2 = timeGetTime();
    mexPrintf("Loading Index Complete, Time Comsuming: %d(ms)\n\n", (t2 - t1));

    plhs[2] = mxCreateDoubleMatrix(1,1,mxREAL);
    double* outdata2 = mxGetPr(plhs[2]);
    *outdata2 = t2 - t1;

    double *queryMat = mxGetPr(prhs[2]);
    veclen = mxGetM(prhs[2]);
    size = mxGetN(prhs[2]);

    plhs[0] = mxCreateDoubleMatrix(k,size,mxREAL);
    double* outdata = mxGetPr(plhs[0]);

    plhs[1] = mxCreateDoubleMatrix(k,size,mxREAL);
    double* outdata1 = mxGetPr(plhs[1]);


    double sumQueryTime = 0;
    for (int q = 0; q < size; q++) {

        const flann::Matrix<double> query(queryMat + veclen * q, 1, veclen);

        flann::Matrix<size_t> indices(new size_t[k], 1, k);
        flann::Matrix<double> dists(new double[k], 1, k);

        mexPrintf("Query No.%d...\n", q + 1);

        t1 = timeGetTime();
        index.knnSearch(query, indices, dists, k);
        t2 = timeGetTime();

        mexPrintf("Query Complete, Time Comsuming: %d(ms)\n\n", (t2 - t1));
        sumQueryTime += t2 - t1;

//        for (int i = 0; i < k; ++i) {
//            mexPrintf("query=%d, index=%d, distance=%f\n", q, indices[0][i], dists[0][i]);
//        }

        for (size_t i = 0; i < k; ++i) {
            outdata[k * q + i] = (double) indices[0][i];
            outdata1[k * q + i] = (double) dists[0][i];
        }
    }

    plhs[3] = mxCreateDoubleMatrix(1,1,mxREAL);
    double* outdata3 = mxGetPr(plhs[3]);
    *outdata3 = sumQueryTime / size;

}