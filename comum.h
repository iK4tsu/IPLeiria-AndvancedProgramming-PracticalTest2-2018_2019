#ifndef COMUM_H
#define COMUM_H

#include <stdint.h>

#define INFO_LOG(msg) printf("[INFO] "msg"\n")
#define INFO_LOG_F(msg, ...) printf("[INFO] "msg"\n", ##__VA_ARGS__)

#define WARN_LOG(msg) printf("[WARNING] "msg"\n")
#define WARN_LOG_F(msg, ...) printf("[WARNING] "msg"\n", ##__VA_ARGS__)

#define ERROR_LOG(msg) printf("[ERROR] "msg"\n")
#define ERROR_LOG_F(msg, ...) printf("[ERROR] "msg"\n", ##__VA_ARGS__)

#define FATAL_LOG(msg)            \
		{                         \
			ERROR_LOG(msg);       \
			exit(EXIT_FAILURE);   \
		}
#define FATAL_LOG_F(msg, ...)                  \
		{                                      \
			ERROR_LOG_F(msg, ##__VA_ARGS__);   \
			exit(EXIT_FAILURE);                \
		}

#define ASSERT_MSG(cond, msg)              \
		{                                  \
			if (!(cond)) FATAL_LOG(msg);   \
		}
#define ASSERT_MSG_F(cond, msg, ...)                        \
		{                                                   \
			if (!(cond)) FATAL_LOG_F(msg, ##__VA_ARGS__);   \
		}


#define MAX_PORT_VALUE 65535
#define MIN_PORT_VALUE 1

#define MAX_KEY_VALUE 127
#define MIN_KEY_VALUE 1

#define MAX_MESSAGE 1024

typedef struct
{
	uint8_t key;
	char message[MAX_MESSAGE];
} cipher_t;

#endif