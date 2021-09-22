/**
 * @file cobs.h
 * @author dhanesh (dhanesh@webyfy.com)
 * @brief COBS codec
 * @version 0.1
 * @date 2021-05-31
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef _COBS_H_
#define _COBS_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t cobsEncode(const uint8_t *data, size_t length, uint8_t *buffer);
size_t cobsDecode(const uint8_t *data, size_t length, uint8_t *buffer);
size_t cobsInPlaceEncode(uint8_t *data, size_t length);
size_t cobsInPlaceDecode(uint8_t *encoded_data, size_t length);
size_t maxCobsEncodedLength(size_t data_length);

#ifdef __cplusplus
}
#endif

#endif // _COBS_H_