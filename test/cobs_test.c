#include <stdio.h>
#include <string.h>

#include "minunit.h"
#include "../src/cobs.h"

int tests_run = 0;

static char *test_encoded_length()
{
    mu_assert("wrong encoded length on data length 254", maxCobsEncodedLength(254) == 256);
    mu_assert("wrong encoded length on data length 255", maxCobsEncodedLength(255) == 257);
    mu_assert("wrong encoded length on data length 0", maxCobsEncodedLength(0) == 1);
    mu_assert("wrong encoded length on data length 1", maxCobsEncodedLength(1) == 2);
    return 0;
}

static char *test_sample(const uint8_t *data, size_t data_len, const uint8_t *data_encoded)
{
    size_t data_encoded_len = maxCobsEncodedLength(data_len);

    uint8_t encode_buffer[data_encoded_len];
    size_t len = cobsEncode(data, data_len, encode_buffer);
    mu_assert("unexpected encoded data length", len == data_encoded_len);
    mu_assert("must be equal", memcmp(data_encoded, encode_buffer, data_encoded_len) == 0);

    uint8_t decode_buffer[data_encoded_len];
    len = cobsDecode(encode_buffer, data_encoded_len, decode_buffer);
    mu_assert("unexpected decoded data length", len == data_len);
    mu_assert("must be equal", memcmp(data, decode_buffer, data_len) == 0);

    return 0;
}

static char *test_empty_encoding()
{
    uint8_t data[] = {};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x01};

    return test_sample(data, data_len, data_encoded);
    return 0;
}

static char *test_empty_decoding()
{
    uint8_t encoded_data[] = {};
    uint8_t decode_buffer[sizeof(encoded_data)];

    mu_assert("should return 0 (empty data)", cobsDecode(encoded_data, sizeof(encoded_data), decode_buffer)==0);
    
    return 0;
}

static char *test_NULL_encoding()
{
    uint8_t data[] = {0x1, 0x2, 0x3, 0x4};
    size_t data_len = sizeof(data);
    uint8_t encode_buffer[maxCobsEncodedLength(data_len)];

    mu_assert("should return 0 (NULL data)", cobsEncode(NULL, data_len, encode_buffer) == 0);
    mu_assert("should return 0 (NULL buffer)", cobsEncode(data, data_len, NULL) == 0);

    return 0;
}

static char *test_NULL_decoding()
{
    mu_assert("shoul return 0 (NULL data or buffer)", cobsDecode(NULL, 0, NULL)==0);

    return 0;
}

static char *test_0()
{
    uint8_t data[] = {0x00};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x01, 0x01};

    return test_sample(data, data_len, data_encoded);
}

static char *test_0_0()
{
    uint8_t data[] = {0x00, 0x00};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x01, 0x01, 0x01};

    return test_sample(data, data_len, data_encoded);
}

static char *test_0_in_between()
{
    uint8_t data[] = {0x11, 0x22, 0x00, 0x33};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x03, 0x11, 0x22, 0x02, 0x33};

    return test_sample(data, data_len, data_encoded);
}

static char *test_no_0_data()
{
    uint8_t data[] = {0x11, 0x22, 0x33, 0x44};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x05, 0x11, 0x22, 0x33, 0x44};

    return test_sample(data, data_len, data_encoded);
}

static char *test_multiple_trailing_0()
{
    uint8_t data[] = {0x11, 0x00, 0x00, 0x00};
    size_t data_len = sizeof(data);
    uint8_t data_encoded[] = {0x02, 0x11, 0x01, 0x01, 0x01};

    return test_sample(data, data_len, data_encoded);
}

static char *all_tests()
{
    mu_run_test(test_encoded_length);
    mu_run_test(test_0);
    mu_run_test(test_0_0);
    mu_run_test(test_0_in_between);
    mu_run_test(test_no_0_data);
    mu_run_test(test_multiple_trailing_0);
    mu_run_test(test_empty_encoding);
    mu_run_test(test_empty_decoding);
    mu_run_test(test_NULL_encoding);
    mu_run_test(test_NULL_decoding);

    return 0;
}

int main()
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else
    {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
