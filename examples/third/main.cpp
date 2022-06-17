#include <stdio.h>
#include <vector>
#include "synthetic_dataset.h"
using namespace std;
int main(int, char **)
{

    std::vector<float> ms{3.5, 9.3}; //  b + m_0*x0 + m_1*x1
    float b = 1.7;
    puts("Training set:\n");
    SyntheticDataset local_training_data = SyntheticDataset(ms, b, 100);
    int feat_count = local_training_data.get_features_count();
    printf(" feat_count = %d\n", feat_count);
}
