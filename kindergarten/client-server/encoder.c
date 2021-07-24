#include "encoder.h"
#include <stdlib.h>
#include <stdio.h>

void pack_rd(read_message_t *rd_msg, unsigned char* buf) {
    *buf++ = 0x0;
    *buf++ = rd_msg->position>>24;
    *buf++ = rd_msg->position>>16;
    *buf++ = rd_msg->position>>8;
    *buf++ = rd_msg->position;
};

void pack_wr(write_message_t *wr_msg, unsigned char* buf) {
};
