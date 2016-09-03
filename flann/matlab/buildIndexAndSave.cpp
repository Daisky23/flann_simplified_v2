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
    unsigned long t1, t2;

    if (nrhs >= 2) {
        filename = mxArrayToString(prhs[1]);
        if (nrhs >= 3) {
            double* L = mxGetPr(prhs[2]);
            indexParams = flann::IndexParams((int)(*L));
        }
    }
    else if (nrhs == 1) {
        filename = "index.dat";
    }
    else {
        std::cout << "Usage: buildIndexAndSave(dataset_Matrix, [name_Of_File_To_Save_Index], [Params: L])";
        return;
    }

    double *datasetMat = mxGetPr(prhs[0]);
    int veclen = mxGetM(prhs[0]);
    int size = mxGetN(prhs[0]);
    const flann::Matrix<double> dataset(datasetMat, size, veclen);

    mexPrintf("Reading Dataset...\n");
    t1 = timeGetTime();
    flann::Index index(dataset, indexParams);
    t2 = timeGetTime();
    mexPrintf("Reading Complete, Time Consuming: %d(ms)\n\n", (t2 - t1));

    mexPrintf("Building Index...\n");
    t1 = timeGetTime();
    index.buildIndex();
    t2 = timeGetTime();
    mexPrintf("Building Complete, Time Consuming: %d(ms)\n\n", (t2 - t1));

    plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
    double* out1 = mxGetPr(plhs[0]);
    *out1 = (t2 - t1);

    mexPrintf("Saving Index...\n");
    t1 = timeGetTime();
    index.saveIndexToFile(filename);
    t2 = timeGetTime();
    mexPrintf("Index Saved in File \"%s\", Time Consuming: %d(ms)\n\n", filename, (t2 - t1));

    plhs[1] = mxCreateDoubleMatrix(1,1,mxREAL);
    double* out2 = mxGetPr(plhs[1]);
    *out2 = (t2 - t1);

}