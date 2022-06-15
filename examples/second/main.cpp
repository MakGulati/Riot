#include <stdio.h>
#include <vector>
#include "linear_algebra_util.h"
using namespace std;
int main(int, char **)
{
    vector<float> point_A{1.0, 2, 4, 6, 5, 7};
    vector<float> point_B{1, 2.0, 6.0, 6, 5, 7};
    std::vector<float> err(point_A.size(), 1);
    err = LinearAlgebraUtil::subtract_vector(point_A, point_B);
    for (unsigned int i = 0; i < err.size(); i++)
    {
        printf("error: %f\n", err[i]);
    }
}
