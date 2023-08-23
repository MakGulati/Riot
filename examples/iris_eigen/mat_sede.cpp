#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "nanocbor/nanocbor.h"

std::vector<uint8_t> serializeEigenMatrix(const Eigen::MatrixXf &matrix)
{
    nanocbor_encoder_t enc;
    nanocbor_encoder_init(&enc, nullptr, 0);
    nanocbor_fmt_array(&enc, matrix.rows() * matrix.cols());

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            nanocbor_fmt_float(&enc, matrix(i, j));
        }
    }

    size_t cborSize = nanocbor_encoded_len(&enc);
    std::vector<uint8_t> cborData(cborSize);
    nanocbor_encoder_init(&enc, cborData.data(), cborData.size());

    nanocbor_fmt_array(&enc, matrix.rows() * matrix.cols());

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.cols(); ++j)
        {
            nanocbor_fmt_float(&enc, matrix(i, j));
        }
    }

    return cborData;
}
Eigen::MatrixXf deserializeEigenMatrix(const std::vector<uint8_t> &cborData, int rows, int cols)
{
    Eigen::MatrixXf matrix(rows, cols);

    nanocbor_value_t dec;
    nanocbor_decoder_init(&dec, cborData.data(), cborData.size());

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            float value;

            if (nanocbor_get_float(&dec, &value) == NANOCBOR_OK)
            {
                matrix(i, j) = value;
            }
            else
            {
                matrix(i, j) = 0.0; 
            }
        }
    }

    return matrix;
}

int main()
{
    Eigen::MatrixXf originalMatrix = Eigen::MatrixXf::Random(3, 3);

    // Serialize
    std::vector<uint8_t> cborData = serializeEigenMatrix(originalMatrix);

    // Deserialize
    Eigen::MatrixXf deserializedMatrix = deserializeEigenMatrix(cborData, 3, 3);

    // Output the deserialized matrix
    std::cout << "Original Matrix:\n"
              << originalMatrix << std::endl;
    std::cout << "Deserialized Matrix:\n"
              << deserializedMatrix << std::endl;

    return 0;
}
