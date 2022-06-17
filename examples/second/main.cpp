#include <stdio.h>
#include <vector>
#include "linear_algebra_util.h"
using namespace std;
int main(int, char **)
{
    vector<vector<float>> mat_A{{1.00, 2, 4, 6, 5, 7}, {1.00, 2, 4, 6, 5, 7}, {1.00, 2, 4, 6, 5, 7}, {1.00, 2, 4, 6, 5, 7}};
    vector<float> point_B{1, 2.0, 6.0, 6, 5, 7};
    for (unsigned int i = 0; i < mat_A.size(); i++)
    {
        {
            for (unsigned int j = 0; j < mat_A[0].size(); j++)
                printf(" %0.1f", mat_A[i][j]);
        }
        printf("\n");
    }
    puts("--------------------------------");
    std::vector<std::vector<float>> result(mat_A[0].size(), std::vector<float>(mat_A.size()));
    result = LinearAlgebraUtil::transpose_vector(mat_A);

    for (unsigned int i = 0; i < result.size(); i++)
    {
        {
            for (unsigned int j = 0; j < result[0].size(); j++)
                printf(" %0.1f", result[i][j]);
        }
        printf("\n");
    }
}
