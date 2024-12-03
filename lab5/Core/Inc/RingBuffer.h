/*
 * RingBuffer.h
 *
 *  Created on: Dec 2, 2024
 *      Author: DELL
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#define BUFFER_LENGTH 100

#include <stdio.h>

struct RingBuffer {
	uint8_t data[BUFFER_LENGTH];
	uint8_t read_idx, write_idx;
	uint16_t size;
};

void initBuffer();

uint8_t pushBuffer(uint8_t item);
uint8_t popBuffer();

uint8_t* frontBuffer();
uint8_t* flushBuffer();

uint16_t getBufSize();
void clearBuffer();

#endif /* INC_RINGBUFFER_H_ */
