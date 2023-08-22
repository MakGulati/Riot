#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm> // for std::shuffle
#include <numeric>   // for std::iota
#include <random>    // for std::default_random_engine
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
// #include <ctime>
#include "blob/iris_aug_train.csv.h"
#include "blob/iris_aug_test.csv.h"

#define LEARNING_RATE 0.001
#define NEPOCH 20
const float RELU_LEAK = 0.001;

using namespace std;

float unitrand()
{
    return (2.0 * (float)rand() / RAND_MAX) - 1.0;
}

float sigmoid(float x)
{
    return 1 / (1 + exp(-x));
}
float relu(float x)
{
    return x > 0 ? x : 0;
}
float drelu(float x)
{
    return x >= 0 ? 1 : 0;
}
vector<float> softmax(vector<float> &z)
{
    float max_val = *max_element(z.begin(), z.end());
    vector<float> exp_values(z.size());
    float sum_exp = 0.0;
    for (unsigned int i = 0; i < z.size(); i++)
    {
        exp_values[i] = exp(z[i] - max_val);
        sum_exp += exp_values[i];
    }
    for (unsigned int i = 0; i < z.size(); i++)
    {
        exp_values[i] /= sum_exp;
    }
    return exp_values;
}

float cross_entropy_loss(vector<float> &y, vector<float> &y_hat)
{
    float loss = 0.0;
    for (unsigned int i = 0; i < y.size(); i++)
    {
        loss -= y[i] * log(y_hat[i] + 1e-9);
    }
    return loss;
}

class Neuron
{
public:
    unsigned int iLayer, iNeuron, nInputs;
    vector<float> weights;
    float bias;
    vector<float> x;
    float y;
    bool isHidden;

    Neuron(unsigned int _iLayer, unsigned int _iNeuron, unsigned int _nInputs, bool _isHidden)
        : iLayer(_iLayer), iNeuron(_iNeuron), nInputs(_nInputs), isHidden(_isHidden)
    {
        weights = vector<float>(nInputs);
        for (auto &w : weights)
            w = unitrand();
        bias = unitrand();
    }

    float feed(vector<float> &_x)
    {
        x = _x;
        float z = bias;
        for (unsigned int i = 0; i < nInputs; ++i)
            z += x[i] * weights[i];
        y = isHidden ? relu(z) : (z);
        return y;
    }

    float dy_dz()
    {
        return isHidden ? drelu(y) : 1;
    }

    float dz_dx(int i)
    {
        return weights[i];
    }

    void update_weights(float dC_dy)
    {
        float dC_dz = dC_dy * dy_dz();
        for (unsigned int i = 0; i < nInputs; ++i)
        {
            float dz_dw = x[i];
            float dC_dw = dC_dz * dz_dw;
            weights[i] -= LEARNING_RATE * dC_dw;
        }

        float dz_db = 1;
        float dC_db = dC_dz * dz_db;
        bias -= LEARNING_RATE * dC_db;
    }
};

class Layer
{
public:
    unsigned int iLayer, nInputs, nNeurons;
    vector<Neuron> neurons;
    vector<float> dC_dy;

    Layer(int _iLayer, int _nInputs, int _nNeurons, bool isHidden = true)
        : iLayer(_iLayer), nInputs(_nInputs), nNeurons(_nNeurons)
    {
        for (unsigned int i = 0; i < nNeurons; ++i)
            neurons.push_back(Neuron(iLayer, i, nInputs, isHidden));
    }

    vector<float> forward(vector<float> &x)
    {
        vector<float> y;
        for (auto &n : neurons)
            y.push_back(n.feed(x));
        return y;
    }

    vector<float> backward(vector<float> &_dC_dy)
    {
        dC_dy = _dC_dy;
        vector<float> dC_dy__prevLayer(nInputs, 0);
        for (unsigned int i = 0; i < nInputs; ++i)
        {
            for (unsigned int j = 0; j < neurons.size(); ++j)
                dC_dy__prevLayer[i] += dC_dy[j] * neurons[j].dy_dz() * neurons[j].weights[i];
        }
        return dC_dy__prevLayer;
    }

    void update_weights()
    {
        for (size_t i = 0; i < neurons.size(); ++i)
        {
            neurons[i].update_weights(dC_dy[i]);
        }
    }
};

class Network
{
public:
    vector<Layer> layers;

    Network(vector<int> nNeuronsEachLayer)
    {
        layers = vector<Layer>();
        unsigned int i;
        for (i = 0; i < nNeuronsEachLayer.size() - 1; ++i)
        {
            layers.push_back(Layer(i, nNeuronsEachLayer[i], nNeuronsEachLayer[i + 1], i < nNeuronsEachLayer.size() - 1));
        }
    }

    vector<float> feed(vector<float> &_x)
    {
        vector<float> x = _x;
        for (unsigned int i = 0; i < layers.size(); i++)
        {
            x = layers[i].forward(x);
        }
        return softmax(x); // Applying softmax here
    }
};

float trainOneSample(Network &network, vector<float> &x, vector<float> &y)
{
    vector<float> y_hat = network.feed(x); // Now y_hat directly has softmax applied values

    vector<float> dC_dlogits = vector<float>(y.size());
    for (unsigned int i = 0; i < y.size(); ++i)
        dC_dlogits[i] = y_hat[i] - y[i];

    for (int i = network.layers.size() - 1; i >= 0; --i)
    {
        dC_dlogits = network.layers[i].backward(dC_dlogits);
        network.layers[i].update_weights();
    }

    float cross_entropy = cross_entropy_loss(y, y_hat); // using your cross_entropy_loss function

    return cross_entropy;
}

float trainOneEpoch(Network &network, vector<vector<float>> &X, vector<vector<float>> &Y)
{
    float total_loss = 0.0;

    vector<int> indices(X.size());
    std::iota(indices.begin(), indices.end(), 0);
    auto rng = std::default_random_engine{};
    std::shuffle(indices.begin(), indices.end(), rng);

    for (unsigned int i : indices)
    {
        float error = trainOneSample(network, X[i], Y[i]);
        total_loss += error;
    }

    return total_loss / X.size();
}

int predictClass(const vector<float> &y_hat)
{
    return max_element(y_hat.begin(), y_hat.end()) - y_hat.begin();
}

float computeAccuracy(Network &network, vector<vector<float>> &X, vector<vector<float>> &Y)
{
    int correctPredictions = 0;

    for (size_t i = 0; i < X.size(); ++i)
    {
        vector<float> y_hat_test = network.feed(X[i]);
        int predictedLabel = predictClass(y_hat_test);
        int realLabel = max_element(Y[i].begin(), Y[i].end()) - Y[i].begin();

        if (predictedLabel == realLabel)
            correctPredictions++;
    }

    return (float)correctPredictions / X.size();
}

int main()
{
    // srand(time(NULL));

    vector<int> nNeuronsEachLayer = {4, 8, 3}; // Assuming 4 input features and 3 output classes
    Network network(nNeuronsEachLayer);

    vector<vector<float>> X; // Features
    vector<vector<float>> Y; // Labels

    ifstream file("iris_aug_train.csv");
    string line, value;

    while (getline(file, line))
    {
        stringstream ss(line);
        vector<float> features;

        // Read first 4 values as features
        for (unsigned int i = 0; i < 4; ++i)
        {
            getline(ss, value, ',');
            features.push_back(stod(value));
        }
        X.push_back(features);

        // Read last value as label and one-hot encode
        getline(ss, value);
        int label = stoi(value);
        vector<float> oneHot(3, 0.0);
        oneHot[label] = 1.0;
        Y.push_back(oneHot);
    }

    file.close();

    vector<vector<float>> X_test; // Features
    vector<vector<float>> Y_test; // Labels

    ifstream test_file("iris_aug_test.csv");
    string line_test, value_test;

    while (getline(test_file, line_test))
    {
        stringstream ss_test(line_test);
        vector<float> features;

        // Read first 4 values as features
        for (unsigned int i = 0; i < 4; ++i)
        {
            getline(ss_test, value_test, ',');
            features.push_back(stod(value_test));
        }
        X_test.push_back(features);

        // Read last value as label and one-hot encode
        getline(ss_test, value_test);
        int label = stoi(value_test);
        vector<float> oneHot(3, 0.0);
        oneHot[label] = 1.0;
        Y_test.push_back(oneHot);
    }

    test_file.close();

    float accuracy = 0.0;
    for (unsigned int i = 0; i < NEPOCH; ++i)
    {
        float loss = trainOneEpoch(network, X, Y);
        float accuracy_i = computeAccuracy(network, X_test, Y_test);
        accuracy += accuracy_i;

        if (i % 10 == 0)
            printf("Epoch %d, loss: %.4f, accuracy per iter: %.2f%%\n", i + 1, loss, accuracy_i * 100);
    }

    // After training, predict and compare
    puts(" Predictions after training:\n");
    for (size_t i = 0; i < X_test.size(); ++i)
    {
        vector<float> y_hat_test = network.feed(X_test[i]);
        float predictedLabel = predictClass(y_hat_test);
        float realLabel = max_element(Y_test[i].begin(), Y_test[i].end()) - Y_test[i].begin();

        printf("Sample: %d Predicted label = %f, Real label = %f\n", i + 1, predictedLabel, realLabel);
    }

    return 0;
}