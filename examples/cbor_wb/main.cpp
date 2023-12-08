#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "nanocbor/nanocbor.h"
// CBOR serialized size: 449 bytes
using namespace Eigen;
using namespace std;

void serializeEigenMatrix(const MatrixXf &matrix, nanocbor_encoder_t &encoder)
{
    nanocbor_fmt_array(&encoder, matrix.rows() * matrix.cols());

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            nanocbor_fmt_float(&encoder, matrix(i, j));
        }
    }
}

void serializeEigenVector(const VectorXf &vector, nanocbor_encoder_t &encoder)
{
    nanocbor_fmt_array(&encoder, vector.size());

    for (int i = 0; i < vector.size(); ++i)
    {
        nanocbor_fmt_float(&encoder, vector(i));
    }
}

void deserializeEigenMatrix(MatrixXf &matrix, nanocbor_value_t &value)
{
    nanocbor_value_t array;
    nanocbor_enter_array(&value, &array);

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            nanocbor_get_float(&array, &matrix(i, j));
        }
    }
}

void deserializeEigenVector(VectorXf &vector, nanocbor_value_t &value)
{
    nanocbor_value_t array;
    nanocbor_enter_array(&value, &array);

    for (int i = 0; i < vector.size(); ++i)
    {
        nanocbor_get_float(&array, &vector(i));
    }
}

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

    nanocbor_encoder_t enc;
    nanocbor_encoder_init(&enc, nullptr, 0);

    // Serialize W1
    nanocbor_fmt_array(&enc, W1.rows() * W1.cols());
    serializeEigenMatrix(W1, enc);

    // Serialize b1
    serializeEigenVector(b1, enc);

    // Serialize W2
    nanocbor_fmt_array(&enc, W2.rows() * W2.cols());
    serializeEigenMatrix(W2, enc);

    // Serialize b2
    serializeEigenVector(b2, enc);

    size_t cborSize = nanocbor_encoded_len(&enc);
    std::vector<uint8_t> cborData(cborSize);
    nanocbor_encoder_init(&enc, cborData.data(), cborData.size());

    // Serialize W1
    nanocbor_fmt_array(&enc, W1.rows() * W1.cols());
    serializeEigenMatrix(W1, enc);

    // Serialize b1
    serializeEigenVector(b1, enc);

    // Serialize W2
    nanocbor_fmt_array(&enc, W2.rows() * W2.cols());
    serializeEigenMatrix(W2, enc);

    // Serialize b2
    serializeEigenVector(b2, enc);

    // Open a file for writing CBOR data
    std::ofstream cborFile("weights_biases_cbor.bin", std::ios::binary);
    cborFile.write(reinterpret_cast<const char *>(cborData.data()), nanocbor_encoded_len(&enc));
    cborFile.close();

    std::cout << "CBOR serialized size: " << nanocbor_encoded_len(&enc) << " bytes" << std::endl;

    // Deserialize CBOR data
    ifstream input("weights_biases_cbor.bin", ios::binary);
    vector<uint8_t> cborDataRead((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    nanocbor_value_t cborValue;
    nanocbor_decoder_init(&cborValue, cborDataRead.data(), cborDataRead.size());

    // Deserialize W1
    deserializeEigenMatrix(W1, cborValue);

    // Deserialize b1
    deserializeEigenVector(b1, cborValue);

    // Deserialize W2
    deserializeEigenMatrix(W2, cborValue);

    // Deserialize b2
    deserializeEigenVector(b2, cborValue);

    // Display deserialized matrices and vectors
    cout << "Deserialized W1:\n"
         << W1 << endl;
    cout << "Deserialized b1:\n"
         << b1 << endl;
    cout << "Deserialized W2:\n"
         << W2 << endl;
    cout << "Deserialized b2:\n"
         << b2 << endl;

    return 0;
}
