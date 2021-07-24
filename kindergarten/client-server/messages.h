#ifndef __MESSAGES_H__
#define __MESSAGES_H__

typedef struct read_message {
    unsigned int position;
    unsigned int size;
} read_message_t;

typedef struct write_message {
    unsigned int position;
    unsigned int size;
    unsigned char* payload[];
} write_message_t;

#endif
