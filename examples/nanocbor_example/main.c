#include <stdio.h>

#include "nanocbor/nanocbor.h"

int main(void)
{
    uint8_t payload_buffer[128];
    nanocbor_encoder_t enc;
    float m0_local = 3.51;
    float m1_local = 9.28;
    float m2_local = 4.99;
    float b_local = 1.366;
    nanocbor_encoder_init(&enc, payload_buffer, sizeof(payload_buffer));
    nanocbor_fmt_array(&enc, 4);
    nanocbor_fmt_float(&enc, m0_local);
    nanocbor_fmt_float(&enc, m1_local);
    nanocbor_fmt_float(&enc, m2_local);
    nanocbor_fmt_float(&enc, b_local);
    size_t payload_len = nanocbor_encoded_len(&enc);
    // printf("\n**************payload_len: %hhn", payload_buffer);

    nanocbor_value_t it;
    nanocbor_decoder_init(&it, (uint8_t *)payload_buffer, payload_len);
    nanocbor_value_t arr; /* Array value instance */
    float temp_val;
    if (nanocbor_enter_array(&it, &arr) < 0)
    {
        return 404;
    }
    while (!nanocbor_at_end(&arr))
    {
        printf("Array: %d\n", nanocbor_get_float(&arr, &temp_val));
        printf("\n:%f", temp_val);
    }
    puts("\n********************************");

    return 0;
}
