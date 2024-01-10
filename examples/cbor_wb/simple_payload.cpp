#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include "nanocbor/nanocbor.h"
using namespace Eigen;
using namespace std;
int main()
{
    // Initialize weights and biases
    int inputSize = 5;
    int hiddenSize = 10;
    int outputSize = 3;

    MatrixXf W1 = MatrixXf::Random(hiddenSize, inputSize);
    VectorXf b1 = VectorXf::Zero(hiddenSize);
    MatrixXf W2 = MatrixXf::Random(outputSize, hiddenSize);
    VectorXf b2 = VectorXf::Zero(outputSize);

    cout << "Original W1:\n"
         << W1 << endl;
    cout << "Original b1:\n"
         << b1 << endl;
    cout << "Original W2:\n"
         << W2 << endl;
    cout << "Original b2:\n"
         << b2 << endl;
    // Calculate total size
    size_t totalSize = W1.size() + b1.size() + W2.size() + b2.size();

    // Create a standard array
    float *float_array = new float[totalSize];

    // Flatten matrices and vectors into the array
    size_t index = 0;

    for (int i = 0; i < W1.size(); ++i)
        float_array[index++] = W1(i);
    for (int i = 0; i < b1.size(); ++i)
        float_array[index++] = b1(i);
    for (int i = 0; i < W2.size(); ++i)
        float_array[index++] = W2(i);
    for (int i = 0; i < b2.size(); ++i)
        float_array[index++] = b2(i);

    // Convert the float array to a uint8_t*
    // uint8_t *buffer = reinterpret_cast<uint8_t *>(float_array);
    uint8_t *buffer = (uint8_t *)float_array;

    // Convert the buffer back to a float array
    // float *float_array_back = reinterpret_cast<float *>(buffer);
    float *float_array_back = (float *)buffer;

        // Create matrices and vectors
    MatrixXf W1_back = Map<MatrixXf>(float_array_back, hiddenSize, inputSize);
    float_array_back += W1_back.size();

    VectorXf b1_back = Map<VectorXf>(float_array_back, hiddenSize);
    float_array_back += b1_back.size();

    MatrixXf W2_back = Map<MatrixXf>(float_array_back, outputSize, hiddenSize);
    float_array_back += W2_back.size();

    VectorXf b2_back = Map<VectorXf>(float_array_back, outputSize);
    cout << "W1_back:\n"
         << W1_back << endl;
    cout << "b1_back:\n"
         << b1_back << endl;
    cout << "W2_back:\n"
         << W2_back << endl;
    cout << "b2_back:\n"
         << b2_back << endl;
    return 0;
}
