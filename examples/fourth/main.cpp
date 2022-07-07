#include <stdio.h>
#include <vector>
#include "synthetic_dataset.h"
#include "line_fit_model.h"

using namespace std;
int main()
{

    std::vector<float> ms{3.5, 9.3, 5}; //  b + m_0*x0 + m_1*x1
    float b = 1.7;
    puts("Training set:\n");
    // std::size_t size_=1000;
    SyntheticDataset local_training_data = SyntheticDataset(ms, b, size_t(1000));
    LineFitModel local_model = LineFitModel(100, 0.001, 3);
    std::tuple<std::size_t, float, float> result = local_model.train_SGD(local_training_data);
    // printf("Result: %zu\n", result[0]);
}
