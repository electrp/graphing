//
// Created by willb on 2/5/2026.
//

#include "PascalCache.h"

#include <vector>

long long GetCombination(int d, int i) {
    static std::vector<std::vector<long long>> cache = {{1}, {1, 1}};

    while (d >= cache.size() - 1) {
        int row = cache.size();
        std::vector<long long> insert;
        std::vector<long long>& prev = cache[row - 1];
        insert.resize(row + 1);
        insert[0] = 1;
        for (int l1 = 1; l1 < row; ++l1) {
            insert[l1] = prev[l1 - 1] + prev[l1];
        }
        insert[row] = 1;
        cache.emplace_back(insert);
    }

    return cache[d][i];
}
