
#include "WeightsBiases.pb.h" // Include the generated protobuf header
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <pb_encode.h>
#include <pb_decode.h>
// g++ main.cpp WB.pb.cc -I/usr/include/eigen3 -lprotobuf -o chk
// Protobuf serialized size: 465 bytes
using namespace Eigen;
using namespace std;
bool encode_w1(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    std::vector<float> *w1_array = (std::vector<float> *)(*arg);
    for (size_t i = 0; i < w1_array->size(); i++)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_fixed32(stream, &(w1_array->at(i))))
            return false;
    }
    return true;
}
bool encode_b1(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    std::vector<float> *b1_array = (std::vector<float> *)(*arg);
    for (size_t i = 0; i < b1_array->size(); i++)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_fixed32(stream, &(b1_array->at(i))))
            return false;
    }
    return true;
}
bool encode_w2(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    std::vector<float> *w2_array = (std::vector<float> *)(*arg);
    for (size_t i = 0; i < w2_array->size(); i++)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_fixed32(stream, &(w2_array->at(i))))
            return false;
    }
    return true;
}
bool encode_b2(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    std::vector<float> *b2_array = (std::vector<float> *)(*arg);
    for (size_t i = 0; i < b2_array->size(); i++)
    {
        if (!pb_encode_tag_for_field(stream, field))
            return false;
        if (!pb_encode_fixed32(stream, &(b2_array->at(i))))
            return false;
    }
    return true;
}

bool decode_w1(pb_istream_t *stream, const pb_field_t * /*field*/, void **arg)
{
    std::vector<float> *w1_array = (std::vector<float> *)(*arg);
    float value;
    if (!pb_decode_fixed32(stream, &value))
        return false;
    w1_array->push_back(value);
    return true;
}
bool decode_b1(pb_istream_t *stream, const pb_field_t * /*field*/, void **arg)
{
    std::vector<float> *b1_array = (std::vector<float> *)(*arg);
    float value;
    if (!pb_decode_fixed32(stream, &value))
        return false;
    b1_array->push_back(value);
    return true;
}
bool decode_w2(pb_istream_t *stream, const pb_field_t * /*field*/, void **arg)
{
    std::vector<float> *w2_array = (std::vector<float> *)(*arg);
    float value;
    if (!pb_decode_fixed32(stream, &value))
        return false;
    w2_array->push_back(value);
    return true;
}
bool decode_b2(pb_istream_t *stream, const pb_field_t * /*field*/, void **arg)
{
    std::vector<float> *b2_array = (std::vector<float> *)(*arg);
    float value;
    if (!pb_decode_fixed32(stream, &value))
        return false;
    b2_array->push_back(value);
    return true;
}
int main(void)
{
    // Initialize weights and biases
    int inputSize = 5;
    int hiddenSize = 10;
    int outputSize = 3;

    MatrixXf w1 = MatrixXf::Random(hiddenSize, inputSize);
    VectorXf b1 = VectorXf::Zero(hiddenSize);
    MatrixXf w2 = MatrixXf::Random(outputSize, hiddenSize);
    VectorXf b2 = VectorXf::Zero(outputSize);
    // Flatten matrices and vectors into arrays
    std::vector<float> w1_array(w1.data(), w1.data() + w1.size());
    std::vector<float> b1_array(b1.data(), b1.data() + b1.size());
    std::vector<float> w2_array(w2.data(), w2.data() + w2.size());
    std::vector<float> b2_array(b2.data(), b2.data() + b2.size());

    // Create protobuf message
    WeightsBiases weightsBiasesMessage = WeightsBiases_init_zero;
    // Set up callbacks
    weightsBiasesMessage.w1.funcs.encode = &encode_w1;
    weightsBiasesMessage.w1.arg = &w1_array;
    weightsBiasesMessage.b1.funcs.encode = &encode_b1;
    weightsBiasesMessage.b1.arg = &b1_array;
    weightsBiasesMessage.w2.funcs.encode = &encode_w2;
    weightsBiasesMessage.w2.arg = &w2_array;
    weightsBiasesMessage.b2.funcs.encode = &encode_b2;
    weightsBiasesMessage.b2.arg = &b2_array;

    size_t bufferSize;
    if (!pb_get_encoded_size(&bufferSize, WeightsBiases_fields, &weightsBiasesMessage))
    {
        // handle error
        printf("Failed to get encoded size\n");
        return -1;
    }
    uint8_t *buffer = new uint8_t[bufferSize];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);
    if (!pb_encode(&stream, WeightsBiases_fields, &weightsBiasesMessage))
    {
        // handle encoding error
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    // Write to file
    std::ofstream protobufFile("weights_biases_protobuf.bin", std::ios::binary);
    protobufFile.write(reinterpret_cast<char *>(buffer), stream.bytes_written);
    protobufFile.close();

    std::cout << "Protobuf serialized size: " << stream.bytes_written << " bytes" << std::endl;
    // Create new vectors for decoded values
    std::vector<float> decoded_w1_array;
    std::vector<float> decoded_b1_array;
    std::vector<float> decoded_w2_array;
    std::vector<float> decoded_b2_array;

    // Set up decode callbacks
    weightsBiasesMessage.w1.funcs.decode = &decode_w1;
    weightsBiasesMessage.w1.arg = &decoded_w1_array;
    weightsBiasesMessage.b1.funcs.decode = &decode_b1;
    weightsBiasesMessage.b1.arg = &decoded_b1_array;
    weightsBiasesMessage.w2.funcs.decode = &decode_w2;
    weightsBiasesMessage.w2.arg = &decoded_w2_array;
    weightsBiasesMessage.b2.funcs.decode = &decode_b2;
    weightsBiasesMessage.b2.arg = &decoded_b2_array;

    pb_istream_t istream = pb_istream_from_buffer(buffer, stream.bytes_written);
    if (!pb_decode(&istream, WeightsBiases_fields, &weightsBiasesMessage))
    {
        // handle decoding error
        printf("Decoding failed: %s\n", PB_GET_ERROR(&istream));
    }

    // Print the decoded data
    std::cout << "Decoded w1: ";
    for (float value : decoded_w1_array)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "Decoded b1: ";
    for (float value : decoded_b1_array)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "Decoded w2: ";
    for (float value : decoded_w2_array)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "Decoded b2: ";
    for (float value : decoded_b2_array)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    delete[] buffer;
    return 0;
}