//
// Created by willb on 3/26/2026.
//

#include "InterpolatingCubicSplines.h"

// InterpolatingCubicSplines::~InterpolatingCubicSplines() {
// }

// std::vector<glm::vec4> GenerateCoef(int k, glm::vec4 points) {
//     // a3 == b0, arry is b0 -> b_(k-1)
//     std::vector<glm::vec4> barr (k, glm::vec4{});
//     std::vector<int> modfn; // Current function in scope
//     // format:
//     //   b_(k-i-1) = (sum of l_j * b_j) - Point
//     //   internal len = k-i-1
//     //   Point is P_(max(k-i+1, k))
//     std::vector<std::vector<int>> list;
//
//     // Start by using the final derivative term (f''(k) == 0)
//     for (int i = 0; i < k; ++i) {
//         int curr = k - i;
//         modfn[i] = curr * curr * curr;
//     }
//     list.push_back(modfn);
//
//     // Since derivative is 0,
//     // b_(k-1) = -(kb0, (k-1)b1, ... 2b_(k-2))
//     // P_k = K*a_1 + (k^3-k)b0 + ((k-1)^3-(k-1)) + ... + 6*b_k-2
//     modfn.resize(k - 1);
//     for (int i = 0; i < k-1; ++i) {
//         int curr = k - i;
//         modfn[i] = modfn[i] - curr;
//     }
//     list.push_back(modfn);
//
//     // Now we continue the pattern
//     // We know that the last term from modfn will always be `6*b_-
//     for (int it = 2; it < k; ++it) {
//
//     }
// }
