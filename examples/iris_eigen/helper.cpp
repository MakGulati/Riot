#include "helper.h"
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
    nanocbor_value_t arr;

    nanocbor_decoder_init(&dec, cborData.data(), cborData.size());
    nanocbor_enter_array(&dec, &arr);
    {

        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                float value;

                if (nanocbor_get_float(&arr, &value) > 0)
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
}
std::vector<uint8_t> serializeEigenVector(const Eigen::VectorXf &vector)
{
    nanocbor_encoder_t enc;
    nanocbor_encoder_init(&enc, nullptr, 0);
    nanocbor_fmt_array(&enc, vector.rows());

    for (int i = 0; i < vector.rows(); ++i)
    {

        nanocbor_fmt_float(&enc, vector(i));
    }

    size_t cborSize = nanocbor_encoded_len(&enc);
    std::vector<uint8_t> cborData(cborSize);
    nanocbor_encoder_init(&enc, cborData.data(), cborData.size());

    nanocbor_fmt_array(&enc, vector.rows());

    for (int i = 0; i < vector.rows(); ++i)
    {

        nanocbor_fmt_float(&enc, vector(i));
    }

    return cborData;
}
Eigen::VectorXf deserializeEigenVector(const std::vector<uint8_t> &cborData, int rows)
{
    Eigen::VectorXf vector(rows);

    nanocbor_value_t dec;
    nanocbor_value_t arr;

    nanocbor_decoder_init(&dec, cborData.data(), cborData.size());
    nanocbor_enter_array(&dec, &arr);
    {

        for (int i = 0; i < rows; ++i)
        {

            float value;

            if (nanocbor_get_float(&arr, &value) > 0)
            {
                vector(i) = value;
            }
            else
            {
                vector(i) = 0.0;
            }
        }
    }

    return vector;
}

pair<Eigen::MatrixXf, Eigen::MatrixXf> dataLoader(istringstream &iss, int numFeatures, int numLabels)
{

    std::string line;
    std::vector<std::vector<float>> data;

    while (std::getline(iss, line, '\n'))
    {
        std::istringstream line_stream(line);
        std::string cell;
        std::vector<float> row;

        while (std::getline(line_stream, cell, ','))
        {
            float value = std::stof(cell);
            row.push_back(value);
        }

        data.push_back(row);
    }

    int numRows = data.size();
    Eigen::MatrixXf X(numRows, numFeatures);
    Eigen::MatrixXf y(numRows, numLabels);
    y.setZero();

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numFeatures; ++j)
        {
            X(i, j) = data[i][j];
        }

        int labelIndex = static_cast<int>(data[i].back()); // Last value is the label
        y(i, labelIndex) = 1.0;
    }

    return make_pair(X, y);
}
