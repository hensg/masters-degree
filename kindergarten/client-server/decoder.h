#ifndef __DECODER_H__
#define __DECODER_H__

#include "messages.h"

read_message_t unpack_rd(char* buf[]);
write_message_t unpack_wr(char* buf[]);

#endif
