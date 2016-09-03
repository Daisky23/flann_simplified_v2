//
// Created by 张浩天 on 2016/8/23.
//

#ifndef FLANN_RESULT_SET_H
#define FLANN_RESULT_SET_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <mex.h>

#include "dist.h"

namespace flann {

// TODO ResultSet
    class ResultSet
    {
    public:
        struct DistIndex
        {
            DistIndex(double dist, unsigned int index) :
                    dist_(dist), index_(index)
            {
            }
            bool operator<(const DistIndex dist_index) const
            {
                return (dist_ < dist_index.dist_) || ((dist_ == dist_index.dist_) && index_ < dist_index.index_);
            }
            double dist_;
            unsigned int index_;
        };

        /** Default cosntructor */
        ResultSet(unsigned int capacity) :
                worst_sim_(/*std::numeric_limits<double>::max()*/10000000000), capacity_(capacity)
        {
            this->clear();
        }

        inline void addPoint(double dist, size_t index) {
            // Don't do anything if we are worse than the worst
            if (dist >= worst_sim_) return;
//            mexPrintf("adding point to result set: index %d, dist %f, worst dist %f\n", index, dist, worst_sim_);

            dist_indices_.insert(DistIndex(dist, index));

            if (is_full_) {
                if (dist_indices_.size() > capacity_) {
                    dist_indices_.erase(*dist_indices_.rbegin());
                    worst_sim_ = dist_indices_.rbegin()->dist_;
                }
            }
            else if (dist_indices_.size() == capacity_) {
                is_full_ = true;
                worst_sim_ = dist_indices_.rbegin()->dist_;
            }
        }

        /** Remove all elements in the set
         */
        void clear()
        {
            dist_indices_.clear();
            is_full_ = false;
        }

        /** Check the status of the set
         * @return true if we have k NN
         */
        inline bool full() const
        {
            return is_full_;
        }

        /** Copy the set to two C arrays
         * @param indices pointer to a C array of indices
         * @param dist pointer to a C array of distances
         * @param n_neighbors the number of neighbors to copy
         */
        void copy(size_t* indices, double* dist, int n_neighbors, bool sorted = true)
        {
            if (n_neighbors<0) n_neighbors = dist_indices_.size();
            int i = 0;
            typedef std::set<DistIndex>::const_iterator Iterator;
            for (Iterator dist_index = dist_indices_.begin(), dist_index_end =
                    dist_indices_.end(); (dist_index != dist_index_end) && (i < n_neighbors); ++dist_index, ++indices, ++dist, ++i) {
                *indices = dist_index->index_;
                *dist = dist_index->dist_;
            }
        }

        /** The number of neighbors in the set
         * @return
         */
        size_t size() const
        {
            return dist_indices_.size();
        }

        /** The distance of the furthest neighbor
         * If we don't have enough neighbors, it returns the max possible value
         * @return
         */
        inline double worstSim() const
        {
            return worst_sim_;
        }
    protected:
        /** Flag to say if the set is full */
        bool is_full_;

        /** The worst distance found so far */
        double worst_sim_;

        /** The best candidates so far */
        std::set<DistIndex> dist_indices_;

        /** The number of neighbors to keep */
        unsigned int capacity_;
    };

}

#endif //FLANN_RESULT_SET_H
