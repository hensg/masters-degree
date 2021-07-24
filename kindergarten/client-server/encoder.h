#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "messages.h"

void pack_rd(read_message_t *rd_msg, unsigned char* buf);
void pack_wr(write_message_t *wr_msg, unsigned char* buf);

#endif
