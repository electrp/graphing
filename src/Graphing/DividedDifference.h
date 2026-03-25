//
// Created by willb on 2/24/2026.
//

#ifndef GRAPHING_DIVIDEDDIFFERENCE_H
#define GRAPHING_DIVIDEDDIFFERENCE_H
#include <array>
#include <vector>

#include "glm/vec4.hpp"

class DividedDifference {
private:
    std::vector<glm::vec4> coefs;
    // Maps coef indices to the calculated ones
    std::vector<int> index_list;
    // Divided difference storage mechanism
    // Stored in reverse to what is expected for easy acceess
    std::vector<glm::dvec4> diff_cache;
    std::vector<int> rev_idx; // back indices of index_list
    int dirty_idx = 0;

public:
    // Returns values from difference cache
    glm::dvec4& get_diff(int col, int row);
    // Swaps order of coefficient in calculations, shouldn't change the resulting polynomial
    void push_internal_index_to_back(int idx);
    void set(glm::vec4 coef, int idx);
    [[nodiscard]] glm::vec4 get_value(int idx) const;
    void resize(int count);
    [[nodiscard]] int size() const;
    void build();
    void print();
    void mark_dirty(int idx);
    // Samples from [0,1]
    glm::vec4 sample_polynomial(float t);

    static void test();
};

#endif //GRAPHING_DIVIDEDDIFFERENCE_H
