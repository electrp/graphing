//
// Created by willb on 2/24/2026.
//

#include "DividedDifference.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <sstream>

#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "Curves/BBDeCasteljauBezier.h"
#include "glm/gtx/string_cast.hpp"

// Helper for divided difference
//  - Sums together the previous value and its own length to create a backwards
//    table of sizes that can be used for triangular lists
static int triangular_cache(int col) {
    static std::vector<int> sums = {0, 1};
    while (col >= sums.size() - 1) {
        sums.push_back(sums.size() + sums.back());
    }

    return sums[col];
}

glm::dvec4 & DividedDifference::get_diff(int col, int row) {
    if (dirty_idx != -1)
        build();

    int idx = triangular_cache(coefs.size() - col);
    return diff_cache[idx - row - 1];
}

void DividedDifference::push_internal_index_to_back(int idx) {
    int internal_index = index_list[idx];
    int last_coef_idx = rev_idx[coefs.size() - 1];

    // In case it's already in the right spot
    if (internal_index == coefs.size() - 1)
        return;

    mark_dirty(idx);

    index_list[idx] = coefs.size() - 1;
    index_list[last_coef_idx] = internal_index;
    rev_idx[internal_index] = last_coef_idx;
    rev_idx[coefs.size() - 1] = idx;
}

void DividedDifference::set(glm::vec4 coef, int idx) {
    coefs[idx] = coef;
    mark_dirty(index_list[idx]);
}

glm::vec4 DividedDifference::get_value(int idx) const {
    return coefs[idx];
}

void DividedDifference::build() {
    if (coefs.size() == 0) {
        diff_cache.clear();
        return;
    }

    // Coefs with correct indices
    diff_cache.resize(triangular_cache(coefs.size()));
    // diff cache end index
    int dcei = diff_cache.size() - 1;
    for (int to_idx = dirty_idx; to_idx < coefs.size(); ++to_idx) {
        int idx = rev_idx[to_idx];
        diff_cache[dcei - to_idx] = coefs[idx];
    }

    int last_acc = 0;
    int idx_acc = coefs.size();
    for (int col = 1; col < coefs.size(); ++col) {
        int row_count = coefs.size() - col;
        // Use the index of the modified value to iterate from (not recalculating whole triangle)
        for (int row = std::max(0, dirty_idx - col); row < row_count; ++row) {
            glm::dvec4 g1 = diff_cache[dcei - (last_acc + row)];
            glm::dvec4 g2 = diff_cache[dcei - (last_acc + row + 1)];
            double t1 = rev_idx[row];
            double t2 = rev_idx[row + col];
            diff_cache[dcei - idx_acc - row] = (g2 - g1) / (t2 - t1);
        }
        last_acc = idx_acc;
        idx_acc += row_count;
    }

    dirty_idx = -1;
}

void DividedDifference::print() {
    for (int row = 0; row < coefs.size(); ++row) {
        std::cout << row << "  ";
        for (int col = 0; col < coefs.size() - row; ++col){
            std::cout << glm::to_string(get_diff(col, row)) << '\t';
        }
        std::cout << std::endl;
    }

    std::cout << "Polynomials:\n";
    auto lam = [&](unsigned char c, int idx) {
        std::cout << c << "  ";
        std::stringstream poly_stub;
        for (int col = 0; col < coefs.size() - 1; ++col) {
            std::cout << get_diff(col, 0)[idx] << poly_stub.str() << " + ";
            int toff = rev_idx[col];
            if (toff < 0) {
                poly_stub << "(t+" << -toff << ")";
            } else if (toff == 0) {
                poly_stub << 't';
            } else {
                poly_stub << "(t-" << toff << ")";
            }
        }
        std::cout << get_diff(coefs.size() - 1, 0)[idx] << poly_stub.str() << std::endl;
    };

    lam('x', 0);
    lam('y', 1);
    lam('z', 2);
    lam('w', 3);
}

void DividedDifference::mark_dirty(int idx) {
    if (dirty_idx == -1)
        dirty_idx = index_list[idx];
    else
        dirty_idx = std::min(dirty_idx, static_cast<int>(index_list[idx]));
}

glm::vec4 DividedDifference::sample_polynomial(float t) {
    if (dirty_idx != -1)
        build();

    if (coefs.size() == 0)
        return {0, 0, 0, 0};

    // Scaled t value
    double ts = t * (coefs.size() - 1);
    double t_it = 1;
    int i = 0;
    glm::dvec4 result = {0, 0, 0, 0};
    while (true) {
        result += get_diff(i, 0) * t_it;
        if (i == coefs.size() - 1) {
            break;
        }
        t_it *= (ts - static_cast<double>(rev_idx[i]));
        ++i;
    }
    return result;
}

void DividedDifference::test() {
    DividedDifference diff;
    diff.resize(4);
    diff.set({0, 0, 0, 0}, 0);
    diff.set({1, 1, 0, 0}, 1);
    diff.set({2, 4, 0, 0}, 2);
    diff.set({3, 9, 0, 0}, 3);
    diff.print();
    diff.push_internal_index_to_back(0);
    diff.print();
    diff.set({0, 0, 0, 0}, 0);
    diff.set({1, 1, 0, 0}, 1);
    diff.set({4, 2, 0, 0}, 2);
    diff.set({9, 3, 0, 0}, 3);
    diff.print();

}

void DividedDifference::resize(int count) {
    int old_size = coefs.size();

    if (count == old_size) return;

    coefs.resize(count);
    index_list.resize(count);
    rev_idx.resize(count);
    // Reset indices when grow or shrink, fixes case where internally last point index is near the end
    for (int i = 0; i < count; ++i) {
        index_list[i] = i;
        rev_idx[i] = i;
    }
    mark_dirty(0);
}

int DividedDifference::size() const {
    return coefs.size();
}
