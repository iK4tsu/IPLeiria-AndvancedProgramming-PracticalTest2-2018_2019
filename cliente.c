#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "client_opt.h"
#include "comum.h"

int main(int argc, char** argv)
{
	struct gengetopt_args_info args_info;
	ASSERT_MSG(cmdline_parser(argc, argv, &args_info) != -1,
			"Error on cmdline_parser");


	/* Check if port is between [1, 65535] */
	ASSERT_MSG_F(args_info.port_arg > MIN_PORT_VALUE &&
			args_info.port_arg < MAX_PORT_VALUE,
			"Invalid port %d\nMust be between [%d,%d]",
			args_info.port_arg, MIN_PORT_VALUE, MAX_PORT_VALUE);


	/* Check if message if less than 1024 bytes */
	ASSERT_MSG_F(strlen(args_info.message_arg) <= MAX_MESSAGE,
			"Message must have less than %d",
			MAX_MESSAGE);

	struct sockaddr_in server_addr;
	int socket_fd;

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(args_info.port_arg);


	/* Convert string to an IPv4 address */
	switch (inet_pton(AF_INET, args_info.ip_arg, &server_addr.sin_addr))
	{
		case 0: ERROR_LOG("Error converting IPv4 address"); break;
		case -1: ERROR_LOG("Invalid address family"); break;
	}


	/* Create a socket */
	ASSERT_MSG((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1,
			"Error creating socket");


	/* Connect to the server */
	ASSERT_MSG(connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != -1,
			"Failed to connect to server");


	/* Info which is going to be sent/received to/from the server */
	cipher_t client_answer, server_answer;


	/* Put the key in the info */
	if (args_info.key_given)
	{
		/* Check if the cipher key is between [1, 127] */
		ASSERT_MSG_F(args_info.key_arg >= MIN_KEY_VALUE && args_info.key_arg <= MAX_KEY_VALUE,
				"Invalid key %d\nMust be between [%d,%d]\n",
				args_info.key_arg, MIN_KEY_VALUE, MAX_KEY_VALUE);

		client_answer.key = args_info.key_arg;
	}
	else
	{
		client_answer.key = 0;
	}

	/* Put the string in the info */
	strcpy(client_answer.message, args_info.message_arg);


	/* Send info to the server */
	ASSERT_MSG(send(socket_fd, &client_answer, sizeof(cipher_t), 0) != -1,
			"Error on send");


	/* Receive info from the server */
	ASSERT_MSG(recv(socket_fd, &server_answer, sizeof(cipher_t), 0) != -1,
			"Error on recv");


	/* Final output */
	if (!args_info.key_given)
		printf(">> KEY: %d\n", server_answer.key);
	printf(">> MESSAGE: %s\n", server_answer.message);


	close(socket_fd);
	cmdline_parser_free(&args_info);
	return 0;
}
