// g++ iris_clf.cpp -I/usr/include/eigen3  -o clf
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "blob/iris_data/iris_aug_train.csv.h"
#include "blob/iris_data/iris_aug_test.csv.h"
#include "helper.h"

using namespace Eigen;
using namespace std;

class NeuralNetwork
{
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
        : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize)
    {
        // Initialize weights and biases
        W1 = MatrixXf::Random(hiddenSize, inputSize);
        b1 = VectorXf::Zero(hiddenSize);
        W2 = MatrixXf::Random(outputSize, hiddenSize);
        b2 = VectorXf::Zero(outputSize);
    }

    VectorXf relu(const VectorXf &z)
    {
        return z.array().max(0.0);
    }
    VectorXf softmax(const VectorXf &z)
    {
        VectorXf expZ = z.array().exp();
        return expZ / expZ.sum();
    }
    VectorXf relu_derivative(const VectorXf &z)
    {
        return (z.array() > 0.0).cast<float>();
    }
    VectorXf forward(const VectorXf &x)
    {
        hiddenLayer = relu(W1 * x + b1);
        outputLayer = softmax(W2 * hiddenLayer + b2);
        return outputLayer;
    }

    void train(const MatrixXf &X, const MatrixXf &y, float learningRate, int epochs)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (int i = 0; i < X.rows(); ++i)
            {
                // Forward pass
                VectorXf x = X.row(i);
                VectorXf target = y.row(i);
                forward(x);

                // Backpropagation
                VectorXf dL_dz2 = outputLayer - target;
                VectorXf dL_dh = W2.transpose() * dL_dz2;
                VectorXf dL_dz1 = dL_dh.array() * relu_derivative(hiddenLayer).array();
                ;

                // Update weights and biases
                W2 -= learningRate * dL_dz2 * hiddenLayer.transpose();
                b2 -= learningRate * dL_dz2;
                W1 -= learningRate * dL_dz1 * x.transpose();
                b1 -= learningRate * dL_dz1;
            }
        }
    }

    int predict(const VectorXf &x)
    {
        VectorXf probabilities = forward(x);
        int predictedClass = 0;
        float maxProbability = probabilities[0];

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
    MatrixXf getW1() const
    {
        return W1;
    }

    MatrixXf getW2() const
    {
        return W2;
    }

    VectorXf getb1() const
    {
        return b1;
    }

    VectorXf getb2() const
    {
        return b2;
    }

    // Setter functions
    void setW1(const MatrixXf &newW1)
    {
        W1 = newW1;
    }

    void setW2(const MatrixXf &newW2)
    {
        W2 = newW2;
    }

    void setb1(const VectorXf &newb1)
    {
        b1 = newb1;
    }

    void setb2(const VectorXf &newb2)
    {
        b2 = newb2;
    }

private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    MatrixXf W1, W2;
    VectorXf b1, b2;
    VectorXf hiddenLayer, outputLayer;
};

int main()
{

    constexpr size_t iris_aug_train_csv_size = sizeof(iris_aug_train_csv);

    std::istringstream iss_train(std::string(reinterpret_cast<const char *>(iris_aug_train_csv), iris_aug_train_csv_size));
    int numFeatures = 4; // Number of features
    int numLabels = 3;   // Number of label classes

    pair<MatrixXf, MatrixXf> data_train = dataLoader(iss_train, numFeatures, numLabels);
    MatrixXf X_train = data_train.first;
    MatrixXf y_train = data_train.second;
    // Create and train the neural network
    int inputSize = X_train.cols();
    int hiddenSize = 2;
    int outputSize = y_train.cols();
    NeuralNetwork nn(inputSize, hiddenSize, outputSize);

    float learningRate = 0.01;
    int epochs = 1;
    puts("Started Training...");
    nn.train(X_train, y_train, learningRate, epochs);
    puts("Completed Training...");
    MatrixXf W1_trained = nn.getW1();
    std::vector<uint8_t> cborData_W1 = serializeEigenMatrix(W1_trained);
    VectorXf b1_trained = nn.getb1();
    std::vector<uint8_t> cborData_b1 = serializeEigenVector(b1_trained);
    MatrixXf W2_trained = nn.getW2();
    std::vector<uint8_t> cborData_W2 = serializeEigenMatrix(W2_trained);
    VectorXf b2_trained = nn.getb2();
    std::vector<uint8_t> cborData_b2 = serializeEigenVector(b2_trained);

    // cout << "Serialized Data (Hexadecimal):\n";
    // for (const uint8_t byte : cborData)
    // {
    //     printf("%02X ", static_cast<int>(byte));
    // }
    constexpr size_t iris_aug_test_csv_size = sizeof(iris_aug_test_csv);

    std::istringstream iss_test(std::string(reinterpret_cast<const char *>(iris_aug_test_csv), iris_aug_test_csv_size));

    pair<MatrixXf, MatrixXf> data_test = dataLoader(iss_test, numFeatures, numLabels);
    MatrixXf X_test = data_test.first;
    MatrixXf y_test = data_test.second;

    // Predict and print results
    for (int i = 0; i < X_test.rows(); ++i)
    {
        VectorXf x = X_test.row(i);
        int activatedNeurons = nn.predict(x);
        printf("Sample %d: Activated neurons in hidden layer = %d\n", i + 1, activatedNeurons);
    }

    return 0;
}
