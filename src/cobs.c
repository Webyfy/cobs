/**
 * @file cobs.c
 * @author dhanesh (dhanesh@webyfy.com)
 * @brief COBS codec
 * 
 * This libray encodes and decodes data to and from COBS (Consistent Overhead Byte Stuffing).
 * Main code is a shameless copy-paste from [COBS Wikipedia page](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing)
 * @version 0.1
 * @date 2021-05-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "cobs.h"
#include <assert.h>

/** COBS encode data to buffer
	@param data Pointer to input data to encode
	@param length Number of bytes to encode
	@param buffer Pointer to encoded output buffer
	@return Encoded buffer length in bytes (0 on error)
	@note Encoded data donot include trailing '0'
*/
size_t cobsEncode(const uint8_t *data, size_t length, uint8_t *buffer)
{
	if ((data == NULL) || (buffer == NULL)){
		return 0;
	}

	uint8_t *encode = buffer;  // Encoded byte pointer
	uint8_t *codep = encode++; // Output code pointer
	uint8_t code = 1;		   // Code value

	for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte)
	{
		if (*byte) // Byte not zero, write it
			*encode++ = *byte, ++code;

		if (!*byte || code == 0xff) // Input is zero or block completed, restart
		{
			*codep = code, code = 1, codep = encode;
			if (!*byte || length)
				++encode;
		}
	}
	*codep = code; // Write final code value

	return encode - buffer;
}

/** In-pace COBS encoding
	@param data Pointer to input data to encode
	@param length Number of bytes to encode
	@return Encoded buffer length in bytes (0 on error)
	@note Encoded data donot include trailing '0'
	@attention make sure, the size of data to be >= maxCobsEncodedLength(length)
*/
size_t cobsInPlaceEncode(uint8_t *data, size_t length)
{
    if ((data == NULL) || length == 0)
    {
        return 0;
    }

    memmove((data + 1), data, length);
    length++;
    size_t zero_offset_index = 0;
    while (zero_offset_index < length)
    {
        uint8_t offset = 1;
        for (size_t j = zero_offset_index + 1;
             j < length && (offset < 255) && data[j] != 0;
             j++, offset++)
            ;
        if (offset == 0xFF)
        {
            memmove(data + zero_offset_index + 255, data + zero_offset_index + 254, length - zero_offset_index);
            length++;
        }
        data[zero_offset_index] = offset;
        zero_offset_index += offset;
    }

    return length;
}

/** Inplace decoding of COBS (encoded)data
 * @param data Pointer to input data to decode
 * @param length Number of bytes to decode
 * @param data Pointer to decoded output data
 * @return Number of bytes successfully decoded (0 returned on error)
 * @note Stops decoding if delimiter byte is found
 * @note can't differentiate between error and empty data (when encoded data is {0x01})
*/
size_t cobsInPlaceDecode(uint8_t *encoded_data, size_t length)
{
    if ((encoded_data == NULL))
    {
        return 0;
    }

    if (encoded_data[length - 1] == 0)
    {
        length--;
    }

    const uint8_t *byte = encoded_data; // Encoded input byte pointer
    size_t decode_index = 0;

    for (uint8_t code = 0xff, block = 0; byte < encoded_data + length; --block)
    {
        if (block)
        { // Decode block byte
            encoded_data[decode_index] = *byte;
            decode_index++;
            byte++;
        }
        else
        {
            if (code != 0xff)
            {
                encoded_data[decode_index] = 0;
                decode_index++;
            }
            block = code = *byte++; // Next block length
            if (code == 0x00)       // Delimiter code found
                break;
        }
    }

    return decode_index;
}

/**
 * @brief calculates the max length of encoded message 
 * 
 * calculates the maximum length required for encoded message in bytes 
 * (excluding trailing 0) from the length of data
 * 
 * @param data_length length of data in bytes
 * @return length of encoded message
 */
size_t maxCobsEncodedLength(size_t data_length)
{
	return data_length + data_length / 254 + 1;
}

