//
// Created by 张浩天 on 2016/8/22.
//

#ifndef FLANN_LSH_INDEX_H
#define FLANN_LSH_INDEX_H

#include <algorithm>
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "../general.h"
#include "dynamic_bitset.h"
#include "matrix.h"
#include "lsh_table.h"
#include "dist.h"
#include "result_set.h"

#include <mex.h>

namespace flann {

    struct IndexParams {
        IndexParams(unsigned int table_number_ = 64, unsigned int hash_number_ = 1,
                    double w_ = 4.0, unsigned int table_size_ = 100) :
                table_number(table_number_), hash_number(hash_number_), w(w_), table_size(table_size_) {}
        unsigned int table_number;
        unsigned int hash_number;
        double w;
        unsigned int table_size;
    };

    class Index {
    public:
        typedef double ElementType;
        typedef double DistanceType;

        Index(const IndexParams& params = IndexParams(), Distance distance = Distance()) :
                distance_(distance), size_(0), size_at_build_(0), veclen_(0),
                index_params_(params) {
            table_number_ = params.table_number;
            hash_number_ = params.hash_number;
            w_ = params.w;
            table_size_ = params.table_size;
        }

        Index(const Matrix<ElementType>& input_data, const IndexParams& params = IndexParams(), Distance distance = Distance()) :
                distance_(distance), size_(0), size_at_build_(0), veclen_(0),
                index_params_(params) {
            table_number_ = params.table_number;
            hash_number_ = params.hash_number;
            w_ = params.w;
            setDataset(input_data);
            table_size_ = size_;
//            table_size_ = params.table_size;
        }

        Index(const Index& other) :
                distance_(other.distance_),
                size_(other.size_),
                size_at_build_(other.size_at_build_),
                veclen_(other.veclen_),
                index_params_(other.index_params_),
                points_(other.points_) {
            table_number_ = other.table_number_;
            hash_number_ = other.hash_number_;
            w_ = other.w_;
            table_size_ = other.table_size_;
        }

        /**
         * Builds the index
         */
        void buildIndex() {

            freeIndex();

            // building index
            buildIndexImpl();

            size_at_build_ = size_;

        }

        void buildIndex(const Matrix<ElementType>& dataset) {
            setDataset(dataset);
            table_size_ = size_;
            this->buildIndex();
        }

        int knnSearch(const Matrix<ElementType>& queries,
                      Matrix<size_t>& indices,
                      Matrix<DistanceType>& dists,
                      size_t knn) const {
            int count = 0;
            ResultSet resultSet(knn);
            mexPrintf("ResultSet Ready.\n");/*TODO*/

            getNeighbors(queries[0], resultSet);

            size_t n /*= std::min(resultSet.size(), knn)*/;
            if (resultSet.size() > knn) n = resultSet.size();
            else n = knn;

            resultSet.copy(indices[0], dists[0], n);
            count += n;

            return count;
        }

        void addPoints(const Matrix<ElementType>& points, float rebuild_threshold = 2) {
            size_t old_size = size_;

            extendDataset(points);

            if (rebuild_threshold>1 && size_at_build_*rebuild_threshold<size_) {
                buildIndex();
            }
            else {
                for (unsigned int i = 0; i < table_number_; ++i) {
                    LshTable& table = tables_[i];
                    for (size_t i=old_size;i<size_;++i) {
                        table.add(i, points_[i]);
                    }
                }
            }
        }

        ElementType* getPoint(size_t index)
        {
            if (index!=size_t(-1)) {
                return points_[index];
            }
            else {
                return NULL;
            }
        }

        /**
         * @return number of features in this index.
         */
        inline size_t size() const
        {
            return size_;
        }

        /**
         * @return The dimensionality of the features in this index.
         */
        inline size_t veclen() const
        {
            return veclen_;
        }

        /**
         * Returns the parameters used by the index.
         *
         * @return The index parameters
         */
        IndexParams getParameters() const
        {
            return index_params_;
        }

        void saveIndexToFile(std::string filename) {
            using namespace std;

            /*******************************
             *      Objects To Save
             *******************************
             *
             *  unsigned int table_number_;
             *  unsigned int hash_number_;
             *  unsigned int table_size_;
             *  double w_;
             *
             *  size_t veclen_;
             *  size_t size_;
             *  size_t size_at_build_;
             *
             *  std::vector<LshTable> tables_;
             *
             */

            ofstream fout;
            fout.open(filename, ios_base::out|ios_base::trunc|ios_base::binary);
            fout.write((char *) &table_number_, sizeof(unsigned int));
            fout.write((char *) &hash_number_, sizeof(unsigned int));
            fout.write((char *) &table_size_, sizeof(unsigned int));
            fout.write((char *) &w_, sizeof(double));
            fout.write((char *) &veclen_, sizeof(size_t));
            fout.write((char *) &size_, sizeof(size_t));
            fout.write((char *) &size_at_build_, sizeof(size_t));

            mexPrintf("---------------------------------------------\n");
            for (size_t i = 0; i < table_number_; ++i) {
                mexPrintf("Saving table No.%d\n", i + 1);
                tables_[i].saveToFile(fout);
                mexPrintf("---------------------------------------------\n");
            }

        }

        void loadIndexFromFile(std::string filename) {
            using namespace std;

            /*******************************
             *      Objects To Save
             *******************************
             *
             *  unsigned int table_number_;
             *  unsigned int hash_number_;
             *  unsigned int table_size_;
             *  double w_;
             *
             *  size_t veclen_;
             *  size_t size_;
             *  size_t size_at_build_;
             *
             *  std::vector<LshTable> tables_;
             *
             */

            ifstream fin;
            fin.open(filename, ios_base::in|ios_base::binary);
            fin.read((char *) &table_number_, sizeof(unsigned int));
            fin.read((char *) &hash_number_, sizeof(unsigned int));
            fin.read((char *) &table_size_, sizeof(unsigned int));
            fin.read((char *) &w_, sizeof(double));
            fin.read((char *) &veclen_, sizeof(size_t));
            fin.read((char *) &size_, sizeof(size_t));
            fin.read((char *) &size_at_build_, sizeof(size_t));

            index_params_ = IndexParams(table_number_, hash_number_, w_, table_size_);

            tables_.resize(table_number_);
            for (size_t i = 0; i < table_number_; ++i) {
                LshTable& table = tables_[i];
                table = LshTable(veclen_, hash_number_, w_, table_size_, false);
                mexPrintf("---------------------------------------------\n");
                mexPrintf("Loading table No.%d\n", i + 1);
                table.loadFromFile(fin);
            }

            mexPrintf("---------------------------------------------\n");
            mexPrintf("Loaded Index Info\n");
            mexPrintf("---------------------------------------------\n");
            mexPrintf("vector length: %u\n", veclen_);
            mexPrintf("data set size: %u\n", size_);
            mexPrintf("data set size at build: %u\n", size_at_build_);
            mexPrintf("table number: %u\n", table_number_);
            mexPrintf("hash number: %u\n", hash_number_);
            mexPrintf("table size: %u\n", table_size_);
            mexPrintf("w: %f\n", w_);
            mexPrintf("---------------------------------------------\n");

        }

    protected:
        void setDataset(const Matrix<ElementType>& dataset)
        {
            size_ = dataset.rows;
            veclen_ = dataset.cols;

            points_.resize(size_);
            for (size_t i=0;i<size_;++i) {
                points_[i] = dataset[i];
            }

        }

    private:
        void freeIndex() {

        }

        void buildIndexImpl() {
            tables_.resize(table_number_);
            std::vector<std::pair<size_t,ElementType*> > features;
            features.reserve(points_.size());

            for (size_t i=0; i < points_.size(); ++i) {
                features.push_back(std::make_pair(i, points_[i]));
            }

            for (unsigned int i = 0; i < table_number_; ++i) {
                LshTable& table = tables_[i];

                table = LshTable(veclen_, hash_number_, w_, table_size_);

                // Add the features to the table
                table.add(features);
            }
        }

        void getNeighbors(const ElementType* vec, ResultSet& result) const {

            std::vector<LshTable>::const_iterator table = tables_.begin();
            std::vector<LshTable>::const_iterator table_end = tables_.end();
            for (; table != table_end; ++table) {
                // TODO key -> H1 H2
                BucketKey key = table->getKey(vec);

//                mexPrintf("table: get bucket from key(%u, %u), searching bucket...\n", key.first, key.second);/*TODO*/
                const Bucket* bucket = table->getBucketFromKey(key);

                if (bucket == 0) continue;
                std::vector< FeatureIndex >::const_iterator training_index = bucket->begin();
                std::vector< FeatureIndex >::const_iterator last_training_index = bucket->end();
//                mexPrintf("bucket size is %u\n", bucket->size());/*TODO*/

                double distance;
                int cnt = 0;
                int skip = 0;
                for (; training_index != last_training_index; ++training_index) {
                    if (*training_index >= size_) {
                        ++skip;
                        continue;
                    }
//                    mexPrintf("Point %u, ", *training_index);
                    distance = distance_(vec, points_[*training_index], veclen_);
//                    mexPrintf("distance = %f\n", distance);
                    result.addPoint(distance, *training_index);
                    ++cnt;
                }
//                mexPrintf("%d points skipped, %d points added\n", skip, cnt);
            }
        }

        void extendDataset(const Matrix<ElementType>& new_points)
        {
            size_t new_size = size_ + new_points.rows;
            points_.resize(new_size);
            for (size_t i = size_; i < new_size; ++i) {
                points_[i] = new_points[i-size_];
            }
            size_ = new_size;
        }


        /** The similarity functor*/
        // do not save(keep default)
        Distance distance_;

        /** Point data */
        // do not save(store independently)
        std::vector<ElementType*> points_;

        /** The different hash tables */
        std::vector<LshTable> tables_;

        /** Parameters of the index. */
        // do not save(save concrete info instead)
        IndexParams index_params_;

        /** store the infos in Index Params */
        unsigned int table_number_;
        unsigned int hash_number_;
        double w_;
        unsigned int table_size_;

        /** Size of one point in the index (and database) */
        size_t veclen_;

        /** Number of points in the index (and database) */
        size_t size_;

        /** Number of features in the dataset when the index was last built. */
        size_t size_at_build_;

    };

}

#endif //FLANN_LSH_INDEX_H
