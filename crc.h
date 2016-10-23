#ifndef _CRC_H
#define _CRC_H

#include "stypes.h"

u16 crc16(const u8 *data, u32 siz);
u32 crc_be_8bt(const u32 *message, u32 siz);
u32 crc32b(const u8 *msg, u32 siz);

#endif
