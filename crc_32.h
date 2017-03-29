#pragma once
#include <cstdint>
#include <stdlib.h>

uint32_t updateCRC32(unsigned char ch, uint32_t crc);
uint32_t crc32buf(char *buf, size_t len);
