#include "../util/matrix.h"
#include "../util/lsh_index.h"
#include <mex.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    if (nrhs < 2) {
        mexPrintf("Param numbers should be 2 at least! [base, query]\n");
        return;
    }

//    const mxArray* datasetMat = prhs[0];
    double *datasetMat = mxGetPr(prhs[0]);
    int veclen = mxGetM(prhs[0]);
    int size = mxGetN(prhs[0]);
//    const flann::Matrix<double> dataset((double *) mxGetData(datasetMat), size, veclen);
    const flann::Matrix<double> dataset(datasetMat, size, veclen);

    flann::Index index(dataset);
    if (nrhs == 2) {
    }
    else if (nrhs == 4) {
        double* table_number = mxGetPr(prhs[2]);
        double* hash_number = mxGetPr(prhs[3]);
        mexPrintf("%f %f\n",*table_number, *hash_number);

        flann::IndexParams indexParams((int)(*table_number), (int)(*hash_number));
        index = flann::Index(dataset, indexParams);
    }
    else {
        mexPrintf("Param numbers should be 2 or 4! [base, query]\n");
        return;
    }

    index.buildIndex();

    mexPrintf("Index Built!\n");


//    const mxArray* queryMat = prhs[1];
    double *queryMat = mxGetPr(prhs[1]);
    veclen = mxGetM(prhs[1]);
    size = mxGetN(prhs[1]);

    int k = 20000;
    plhs[0] = mxCreateDoubleMatrix(k,size,mxREAL);
    double* outdata = mxGetPr(plhs[0]);

    plhs[1] = mxCreateDoubleMatrix(k,size,mxREAL);
    double* outdata1 = mxGetPr(plhs[1]);

    for (int q = 0; q < size; q++) {
        //    const flann::Matrix<double> query((double *) mxGetData(queryMat), size, veclen);
        const flann::Matrix<double> query(queryMat + veclen * q, 1, veclen);

        flann::Matrix<size_t> indices(new size_t[k], 1, k);
        flann::Matrix<double> dists(new double[k], 1, k);

        mexPrintf("Query Begin...\n");

        index.knnSearch(query, indices, dists, k);

        for (int i = 0; i < k; ++i) {
            mexPrintf("query=%d, index=%d, distance=%f\n", q, indices[0][i], dists[0][i]);
        }

        for (size_t i = 0; i < k; ++i) {
            outdata[k * q + i] = (double) indices[0][i];
            outdata1[k * q + i] = (double) dists[0][i];
        }
    }
}