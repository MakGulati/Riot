// helper.h

#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <Eigen/Dense>
using namespace std;

vector<uint8_t> serializeEigenMatrix(const Eigen::MatrixXf &matrix);
Eigen::MatrixXf deserializeEigenMatrix(const vector<uint8_t> &cborData, int rows, int cols);
vector<uint8_t> serializeEigenVector(const Eigen::VectorXf &vector);
Eigen::VectorXf deserializeEigenVector(const vector<uint8_t> &cborData, int rows);
pair<Eigen::MatrixXf, Eigen::MatrixXf> dataLoader(istringstream &iss, int numFeatures, int numLabels);

#endif // HELPER_H
