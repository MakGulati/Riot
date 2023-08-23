#include <iostream>
#include <Eigen/Dense>
#include "nanocbor/nanocbor.h"
#include <vector>
using namespace std;

std::vector<uint8_t> serializeEigenData(const Eigen::MatrixXd &W1,
                                        const Eigen::VectorXd &b1,
                                        const Eigen::MatrixXd &W2,
                                        const Eigen::VectorXd &b2)
{
    nanocbor_encoder_t enc;
    nanocbor_encoder_init(&enc, nullptr, 0);
    nanocbor_fmt_array(&enc, 4); // 4 elements in the array

    nanocbor_fmt_bstr(&enc, W1.data(), W1.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, b1.data(), b1.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, W2.data(), W2.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, b2.data(), b2.size() * sizeof(double));

    size_t cborSize = nanocbor_encoded_len(&enc);
    std::vector<uint8_t> cborData(cborSize);
    nanocbor_encoder_init(&enc, cborData.data(), cborData.size());

    nanocbor_fmt_array(&enc, 4);
    nanocbor_fmt_bstr(&enc, W1.data(), W1.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, b1.data(), b1.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, W2.data(), W2.size() * sizeof(double));
    nanocbor_fmt_bstr(&enc, b2.data(), b2.size() * sizeof(double));

    return cborData;
}

/*
void deserializeEigenData(const std::vector<uint8_t> &cborData,
                          Eigen::MatrixXd &W1_deserialized,
                          Eigen::VectorXd &b1_deserialized,
                          Eigen::MatrixXd &W2_deserialized,
                          Eigen::VectorXd &b2_deserialized)
{
    nanocbor_value_t dec;
    nanocbor_decoder_init(&dec, cborData.data(), cborData.size());

    size_t numElements;
    nanocbor_get_array_length(&dec, &numElements);
    nanocbor_skip(&dec);

    W1_deserialized.resize(hiddenSize, inputSize);
    b1_deserialized.resize(hiddenSize);
    W2_deserialized.resize(outputSize, hiddenSize);
    b2_deserialized.resize(outputSize);

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, W1_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, b1_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, W2_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, b2_deserialized.data(), numElements / sizeof(double));
}
*/
int main()
{
    int inputSize = 4;
    int hiddenSize = 2;
    int outputSize = 3;

    Eigen::MatrixXd W1 = Eigen::MatrixXd::Random(hiddenSize, inputSize);
    Eigen::VectorXd b1 = Eigen::VectorXd::Zero(hiddenSize);
    Eigen::MatrixXd W2 = Eigen::MatrixXd::Random(outputSize, hiddenSize);
    Eigen::VectorXd b2 = Eigen::VectorXd::Zero(outputSize);

    // Output the  matrices and vectors
    cout << "W1:\n"
         << W1 << endl;
    cout << "b1:\n"
         << b1 << endl;
    cout << "W2:\n"
         << W2 << endl;
    cout << "b2:\n"
         << b2 << endl;
    // Serialize
    std::vector<uint8_t> cborData = serializeEigenData(W1, b1, W2, b2);

    // // Deserialize
    // Eigen::MatrixXd W1_deserialized;
    // Eigen::VectorXd b1_deserialized;
    // Eigen::MatrixXd W2_deserialized;
    // Eigen::VectorXd b2_deserialized;

    // deserializeEigenData(cborData, W1_deserialized, b1_deserialized, W2_deserialized, b2_deserialized);

    // // Output the deserialized matrices and vectors
    // std::cout << "W1_deserialized:\n"
    //           << W1_deserialized << std::endl;
    // std::cout << "b1_deserialized:\n"
    //           << b1_deserialized << std::endl;
    // std::cout << "W2_deserialized:\n"
    //           << W2_deserialized << std::endl;
    // std::cout << "b2_deserialized:\n"
    //           << b2_deserialized << std::endl;

    return 0;
}
