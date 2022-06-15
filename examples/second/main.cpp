#include <stdio.h>
#include <vector>
#include "linear_algebra_util.h"
using namespace std;
int main(int, char **)
{
    vector<double> point_A{1, 2, 4, 6, 5, 7};
    vector<double> point_B{1, 2, 6, 6, 5, 7};
    std::vector<double> err(point_A.size(), 1);
    err = LinearAlgebraUtil::subtract_vector(point_A, point_B);
    for (size_t i = 0; i < err.size(); i++)
    {
        printf("error %f\n", err[i]);
    }
}
