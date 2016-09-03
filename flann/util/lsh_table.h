//
// Created by 张浩天 on 2016/8/22.
//

#ifndef FLANN_LSH_TABLE_H
#define FLANN_LSH_TABLE_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits.h>
#include <map>
#include <math.h>
#include <stddef.h>
#include <time.h>
#include "random.h"
#include <fstream>
#include <string>
#include <mex.h>

#include "matrix.h"

namespace flann {

    typedef size_t FeatureIndex;

    typedef std::pair<unsigned int, unsigned int> BucketKey;

    typedef std::vector<FeatureIndex> Bucket;

#define prime_ 4294967291U
#define MAX_HASH_RND 536870912U

    class LshTable {
    public:

        typedef double ElementType;
        typedef std::map<unsigned int, Bucket> BucketsSeq;
        typedef std::map<unsigned int, BucketsSeq> BucketsSpace;

        LshTable() {}

        LshTable(unsigned int feature_size, unsigned int hash_num, double w, unsigned int table_size, bool init) {
            randomer = Randomer();

            feature_size_ = feature_size;
            hash_num_ = hash_num;
            w_ = w;
            table_size_ = table_size;

            a_.clear(), a_.resize(hash_num);
            b_.clear(), b_.resize(hash_num);
            r1_.clear(), r1_.resize(hash_num);
            r2_.clear(), r2_.resize(hash_num);

            buckets_space_.clear();
        }

        LshTable(unsigned int feature_size, unsigned int hash_num, double w, unsigned int table_size) {
            randomer = Randomer();

            feature_size_ = feature_size;
            hash_num_ = hash_num;
            w_ = w;
            table_size_ = table_size;

            buckets_space_.clear();

            a_.clear(), a_.resize(hash_num);
            b_.clear(), b_.resize(hash_num);
            r1_.clear(), r1_.resize(hash_num);
            r2_.clear(), r2_.resize(hash_num);
            for (size_t i = 0; i < hash_num; ++i) {
                r1_[i] = randomer.genRandomUns32(1, MAX_HASH_RND);
                r2_[i] = randomer.genRandomUns32(1, MAX_HASH_RND);
                b_[i] = randomer.genUniformRandom(0, w_);
                a_[i].resize(feature_size);
                /*mexPrintf("b[%d] is %f, r1[%d] is %d, r2[%d] is %d\n", i, b_[i],i,r1_[i],i,r2_[i])*/;
                for (size_t j = 0; j < feature_size; ++j) {
                    a_[i][j] = randomer.genGaussianRandom();
//                    mexPrintf("%f\n",a_[i][j]);
                }
                /*mexPrintf("a[%d][0] is %f\n", i,a_[i][0]);*/
            }
        }

        bool flag = false;

        void add(size_t index, const ElementType* feature) {
            // Add the value to the corresponding bucket
//                mexPrintf("add index %u\n", index);
                flag = true;
                BucketKey bucketKey = getKey(feature);
                flag = false;
//            mexPrintf("h1=%u, h2=%u\n",bucketKey.first, bucketKey.second);
//            mexPrintf("h1 size is %d, h2 size is %d\n",buckets_space_[bucketKey.first].size(),buckets_space_[bucketKey.first][bucketKey.second].size());
                buckets_space_[bucketKey.first][bucketKey.second].push_back(index);
        }

        void add(const std::vector< std::pair<size_t, ElementType*> >& features)
        {
            // Add the features to the table
            for (size_t i = 0; i < features.size(); ++i) {
                add(features[i].first, features[i].second);
            }
        }

        inline const Bucket* getBucketFromKey(BucketKey key) const {

            // That means we have to check for the hash table for the presence of a key
            BucketsSpace::const_iterator bucket_seq_it, bucket_seq_end = buckets_space_.end();
            bucket_seq_it = buckets_space_.find(key.first);
            // Stop here if that bucket does not exist
            if (bucket_seq_it == bucket_seq_end) return 0;

            BucketsSeq buckets_seq_ = bucket_seq_it->second;
//            mexPrintf("h1 size is %d\n",buckets_seq_.size());

            BucketsSeq::const_iterator bucket_it, bucket_end = buckets_seq_.end();
            bucket_it = buckets_seq_.find(key.second);
            if (bucket_it == bucket_end) return 0;
            else {
                Bucket bucket = bucket_it->second;
//                mexPrintf("h2 size is %d\n", bucket.size());
                return &bucket;
            }

        }

        BucketKey getKey(const ElementType* feature) const {
            unsigned int h1, h2;
            std::vector<long long> h;

            h.clear(), h.resize(hash_num_);
            for (size_t i = 0; i < hash_num_; ++i) {
                double h_tmp = 0;
                for (size_t j = 0; j < feature_size_; ++j) {
                    h_tmp += a_[i][j] * feature[j];
                }

                h_tmp += b_[i];
                h_tmp = floor(h_tmp / w_);

                h[i] = (long long )h_tmp;
            }

            long long h1_tmp = 0, h2_tmp = 0;

            for (size_t i = 0; i < hash_num_; ++i) {
                h1_tmp += r1_[i] * h[i];
                h1_tmp = h1_tmp % prime_;
                if (h1_tmp < 0) h1_tmp += prime_;
            }
            h1 = (unsigned int)(h1_tmp % table_size_);

            for (size_t i = 0; i < hash_num_; ++i) {
                h2_tmp += r2_[i] * h[i];
                h2_tmp = h2_tmp % prime_;
                if (h2_tmp < 0) h2_tmp += prime_;
            }
            h2 = (unsigned int)(h2_tmp);

            return std::make_pair(h1, h2);
        }

        void saveToFile(std::ofstream& fout) {
            using namespace std;
            /*******************************
             *      Objects To Save
             *******************************
             *
             *  vector<vector<double>> a_;    // hash_num_ by veclen_
             *  vector<double> b_;                 // hash_num_ by 1
             *  vector<unsigned int> r1_, r2_;     // hash_num_ by 1
             *
             *  BucketsSpace buckets_space_;
             **/


            for (size_t i = 0; i < hash_num_; ++i) {
                for (size_t j = 0; j < feature_size_; ++j) {
                    fout.write((char *) &a_[i][j], sizeof(double));
                }
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fout.write((char *) &b_[i], sizeof(double));
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fout.write((char *) &r1_[i], sizeof(unsigned int));
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fout.write((char *) &r2_[i], sizeof(unsigned int));
            }

            size_t bucketsSpaceSize = buckets_space_.size();
            fout.write((char *) &bucketsSpaceSize, sizeof(size_t));
//            mexPrintf("bucket space size: %u\n\n", bucketsSpaceSize);
            for (std::map<unsigned int, BucketsSeq>::const_iterator bucketsSeqIt = buckets_space_.begin(),
                         bucketsSeqEnd = buckets_space_.end(); bucketsSeqIt != bucketsSeqEnd; ++bucketsSeqIt) {
                unsigned int bucketsSeqKey = bucketsSeqIt->first;
                fout.write((char *) &bucketsSeqKey, sizeof(unsigned int));
                size_t bucketsSeqSize = bucketsSeqIt->second.size();
                fout.write((char *) &bucketsSeqSize, sizeof(size_t));
//                mexPrintf("bucket sequence key: %u, size: %u\n", bucketsSeqKey, bucketsSeqSize);
                for (std::map<unsigned int, Bucket>::const_iterator bucketIt = bucketsSeqIt->second.begin(),
                        bucketEnd = bucketsSeqIt->second.end(); bucketIt != bucketEnd; ++bucketIt) {
                    unsigned int bucketKey = bucketIt->first;
                    fout.write((char *) &bucketKey, sizeof(unsigned int));
                    size_t bucketSize = bucketIt->second.size();
                    fout.write((char *) &bucketSize, sizeof(size_t));
//                    mexPrintf("   bucket key: %u, size: %u\n\n", bucketIt->first, bucketIt->second.size());
                    for (std::vector<FeatureIndex>::const_iterator it = bucketIt->second.begin(),
                            end = bucketIt->second.end(); it != end; ++it) {
                        size_t index = *it;
                        fout.write((char *) &index, sizeof(size_t));
                    }
                }
            }

        }

        void loadFromFile(std::ifstream& fin) {
            using namespace std;
            /*******************************
             *      Objects To Load
             *******************************
             *
             *  vector<vector<double>> a_;               // hash_num_ by veclen_
             *  vector<double> b_;                       // hash_num_ by 1
             *  vector<unsigned int> r1_, r2_;           // hash_num_ by 1
             *
             *  BucketsSpace buckets_space_;
             **/


            for (size_t i = 0; i < hash_num_; ++i) {
                a_[i].resize(feature_size_);
                for (size_t j = 0; j < feature_size_; ++j) {
                    fin.read((char *) &a_[i][j], sizeof(double));
                }
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fin.read((char *) &b_[i], sizeof(double));
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fin.read((char *) &r1_[i], sizeof(unsigned int));
            }
            for (size_t i = 0; i < hash_num_; ++i) {
                fin.read((char *) &r2_[i], sizeof(unsigned int));
            }

            size_t bucketsSpaceSize;
            fin.read((char *) &bucketsSpaceSize, sizeof(size_t));
//            mexPrintf("bucket space size: %u\n\n", bucketsSpaceSize);
            for (size_t bucketsSeqIt = 0; bucketsSeqIt < bucketsSpaceSize; ++bucketsSeqIt) {
                unsigned int bucketsSeqKey;
                fin.read((char *) &bucketsSeqKey, sizeof(unsigned int));
                size_t bucketsSeqSize;
                fin.read((char *) &bucketsSeqSize, sizeof(size_t));
//                mexPrintf("bucket sequence key: %u, size: %u\n", bucketsSeqKey, bucketsSeqSize);
                for (size_t bucketIt = 0; bucketIt < bucketsSeqSize; ++bucketIt) {
                    unsigned int bucketKey;
                    fin.read((char *) &bucketKey, sizeof(unsigned int));
                    size_t bucketSize;
                    fin.read((char *) &bucketSize, sizeof(size_t));
//                    mexPrintf("   bucket sequence key: %u, size: %u\n\n", bucketKey, bucketSize);
                    for (size_t it = 0; it < bucketSize; ++it) {
                        size_t index;
                        fin.read((char *) &index, sizeof(size_t));
                        buckets_space_[bucketsSeqKey][bucketKey].push_back(index);
                    }
                }
            }
        }

    private:

        /** The hash table of all the buckets in case we cannot use the speed version
         */
        BucketsSpace buckets_space_;

        /** The size of features*/
        // do not save(already saved in the index, no need to store again in hash tables)
        unsigned int feature_size_;  // veclen_ in index
        unsigned int hash_num_;
        double w_;
        unsigned int table_size_;

        /** Random hash vectors*/
        std::vector<std::vector<double>> a_; // hash_num_ by veclen_
        std::vector<double> b_; // hash_num_ by 1
        std::vector<unsigned int> r1_, r2_; // hash_num_ by 1

        // do not save(util class)
        Randomer randomer;

    };

}

#endif //FLANN_LSH_TABLE_H
