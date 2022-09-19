#include <stdio.h>
#include <vector>
#include "synthetic_dataset.h"
#include "line_fit_model.h"
#include <tuple>
#include "nanocbor/nanocbor.h"

int main()
{
    uint8_t payload_buffer[128];
    nanocbor_encoder_t enc;
    float m0_local = 3.512735;
    float m1_local = 9.285732;
    float m2_local = 4.994352;
    float b_local = 1.366232;
    nanocbor_encoder_init(&enc, payload_buffer, sizeof(payload_buffer));
    nanocbor_fmt_array(&enc, 4);
    nanocbor_fmt_float(&enc, m0_local);
    nanocbor_fmt_float(&enc, m1_local);
    nanocbor_fmt_float(&enc, m2_local);
    nanocbor_fmt_float(&enc, b_local);
    size_t payload_len = nanocbor_encoded_len(&enc);
    printf("\n**************payload_len: %d", payload_len);

    // std::vector<float> ms{3.5, 9.3, 5};
    // float b = 1.7;
    // puts("Training set:\n");
    // SyntheticDataset local_training_data = SyntheticDataset(ms, b, 1000);
    // SyntheticDataset local_testing_data = SyntheticDataset(ms, b, 100);
    // LineFitModel local_model = LineFitModel(1000, 0.001, 3);
    // std::tuple<size_t, float, float> output_train = local_model.train_SGD(local_training_data);
    // printf("Training Loss: %f\n", std::get<1>(output_train));
    // printf("Training Accuracy: %f\n", std::get<2>(output_train));

    // std::tuple<size_t, float, float> output_test = local_model.evaluate(local_testing_data);
    // printf("testing Loss: %f\n", std::get<1>(output_test));
    // printf("testing Accuracy: %f\n", std::get<2>(output_test));

    return 0;
}
