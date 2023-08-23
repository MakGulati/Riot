// g++ iris_clf.cpp -I/usr/include/eigen3  -o clf
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "blob/iris_data/iris_aug_train.csv.h"
#include "blob/iris_data/iris_aug_test.csv.h"

using namespace Eigen;
using namespace std;

class NeuralNetwork
{
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
        : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize)
    {
        // Initialize weights and biases
        W1 = MatrixXd::Random(hiddenSize, inputSize);
        b1 = VectorXd::Zero(hiddenSize);
        W2 = MatrixXd::Random(outputSize, hiddenSize);
        b2 = VectorXd::Zero(outputSize);
    }

    VectorXd relu(const VectorXd &z)
    {
        return z.array().max(0.0);
    }
    VectorXd softmax(const VectorXd &z)
    {
        VectorXd expZ = z.array().exp();
        return expZ / expZ.sum();
    }
    VectorXd relu_derivative(const VectorXd &z)
    {
        return (z.array() > 0.0).cast<double>();
    }
    VectorXd forward(const VectorXd &x)
    {
        hiddenLayer = relu(W1 * x + b1);
        outputLayer = softmax(W2 * hiddenLayer + b2);
        return outputLayer;
    }

    void train(const MatrixXd &X, const MatrixXd &y, double learningRate, int epochs)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (int i = 0; i < X.rows(); ++i)
            {
                // Forward pass
                VectorXd x = X.row(i);
                VectorXd target = y.row(i);
                forward(x);

                // Backpropagation
                VectorXd dL_dz2 = outputLayer - target;
                VectorXd dL_dh = W2.transpose() * dL_dz2;
                VectorXd dL_dz1 = dL_dh.array() * relu_derivative(hiddenLayer).array();
                ;

                // Update weights and biases
                W2 -= learningRate * dL_dz2 * hiddenLayer.transpose();
                b2 -= learningRate * dL_dz2;
                W1 -= learningRate * dL_dz1 * x.transpose();
                b1 -= learningRate * dL_dz1;
            }
        }
    }

    int predict(const VectorXd &x)
    {
        VectorXd probabilities = forward(x);
        int predictedClass = 0;
        double maxProbability = probabilities[0];

        for (int i = 1; i < probabilities.size(); ++i)
        {
            if (probabilities[i] > maxProbability)
            {
                maxProbability = probabilities[i];
                predictedClass = i;
            }
        }

        return predictedClass;
    }

    // Getter functions
    MatrixXd getW1() const
    {
        return W1;
    }

    MatrixXd getW2() const
    {
        return W2;
    }

    VectorXd getb1() const
    {
        return b1;
    }

    VectorXd getb2() const
    {
        return b2;
    }
    void serializeParameter():
    {
        // Serialize matrices and vectors to CBOR
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

    // Deserialize CBOR to matrices and vectors
    nanocbor_value_t dec;
    nanocbor_decoder_init(&dec, cborData.data(), cborData.size());

    size_t numElements;
    nanocbor_get_array_length(&dec, &numElements);
    nanocbor_skip(&dec);

    Eigen::MatrixXd W1_deserialized(hiddenSize, inputSize);
    Eigen::VectorXd b1_deserialized(hiddenSize);
    Eigen::MatrixXd W2_deserialized(outputSize, hiddenSize);
    Eigen::VectorXd b2_deserialized(outputSize);

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, W1_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, b1_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, W2_deserialized.data(), numElements / sizeof(double));

    nanocbor_get_bstr_size(&dec, &numElements);
    nanocbor_fmt_bstr_to_double_array(&dec, b2_deserialized.data(), numElements / sizeof(double));

    // Output the deserialized matrices and vectors
    std::cout << "W1_deserialized:\n"
              << W1_deserialized << std::endl;
    std::cout << "b1_deserialized:\n"
              << b1_deserialized << std::endl;
    std::cout << "W2_deserialized:\n"
              << W2_deserialized << std::endl;
    std::cout << "b2_deserialized:\n"
              << b2_deserialized << std::endl;

private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    MatrixXd W1, W2;
    VectorXd b1, b2;
    VectorXd hiddenLayer, outputLayer;
};

pair<MatrixXd, MatrixXd> readCSV(istringstream &iss, int numFeatures, int numLabels)
{

    std::string line;
    std::vector<std::vector<double>> data;

    while (std::getline(iss, line, '\n'))
    {
        std::istringstream line_stream(line);
        std::string cell;
        std::vector<double> row;

        while (std::getline(line_stream, cell, ','))
        {
            double value = std::stof(cell);
            row.push_back(value);
        }

        data.push_back(row);
    }

    int numRows = data.size();
    MatrixXd X(numRows, numFeatures);
    MatrixXd y(numRows, numLabels);
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

int main()
{

    constexpr size_t iris_aug_train_csv_size = sizeof(iris_aug_train_csv);

    std::istringstream iss_train(std::string(reinterpret_cast<const char *>(iris_aug_train_csv), iris_aug_train_csv_size));
    int numFeatures = 4; // Number of features
    int numLabels = 3;   // Number of label classes

    pair<MatrixXd, MatrixXd> data_train = readCSV(iss_train, numFeatures, numLabels);
    MatrixXd X_train = data_train.first;
    MatrixXd y_train = data_train.second;
    // Create and train the neural network
    int inputSize = X_train.cols();
    int hiddenSize = 2;
    int outputSize = y_train.cols();
    NeuralNetwork nn(inputSize, hiddenSize, outputSize);

    double learningRate = 0.01;
    int epochs = 1;
    puts("Started Training...");
    nn.train(X_train, y_train, learningRate, epochs);
    puts("Completed Training...");

    constexpr size_t iris_aug_test_csv_size = sizeof(iris_aug_test_csv);

    std::istringstream iss_test(std::string(reinterpret_cast<const char *>(iris_aug_test_csv), iris_aug_test_csv_size));

    pair<MatrixXd, MatrixXd> data_test = readCSV(iss_test, numFeatures, numLabels);
    MatrixXd X_test = data_test.first;
    MatrixXd y_test = data_test.second;

    // Predict and print results
    for (int i = 0; i < X_test.rows(); ++i)
    {
        VectorXd x = X_test.row(i);
        int activatedNeurons = nn.predict(x);
        // cout << "Sample " << i + 1 << ": Activated neurons in hidden layer = " << activatedNeurons << endl;
        printf("Sample %d: Activated neurons in hidden layer = %d\n", i + 1, activatedNeurons);
    }

    return 0;
}
