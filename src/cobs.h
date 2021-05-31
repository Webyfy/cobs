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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

size_t cobsEncode(const void *data, size_t length, uint8_t *buffer);
size_t cobsDecode(const uint8_t *buffer, size_t length, void *data);

#ifdef __cplusplus
}
#endif

#endif // _COBS_H_