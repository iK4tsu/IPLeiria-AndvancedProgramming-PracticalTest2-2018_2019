#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include "server_opt.h"
#include "comum.h"

void server_logic(int client_fd);

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


	int server_fd, client_fd;
	socklen_t client_len;

	struct sockaddr_in server_addr, client_addr;


	/* Create a socket */
	ASSERT_MSG((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1,
			"Error creating socket");


	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(args_info.port_arg);


	/* Bind the server */
	ASSERT_MSG(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != -1,
			"Failing to bind");


	/* Have the server listening for a socket connection */
	ASSERT_MSG(listen(server_fd, 5) != -1,
			"Failed to listen");


	printf("Server in port: %d\n", args_info.port_arg);


	while(1)
	{
		client_len = sizeof(client_addr);


		/* Accept a connection */
		ASSERT_MSG((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len)) >= 0,
				"Failed on accept");


		/* Create a process to deal with the answer */
		switch (fork())
		{
			case 0: server_logic(client_fd); break;
			case -1: FATAL_LOG("Failed on fork"); break;
		}
	}


	cmdline_parser_free(&args_info);
	return 0;
}


void server_logic(int client_fd)
{
	/* Info which is going to be sent/received to/from the client */
	cipher_t server_answer, client_answer;


	/* Receive info from the client */
	ASSERT_MSG((recv(client_fd, &client_answer, sizeof(cipher_t), 0)) != -1,
			"Error on recv");


	int i = 0;
	char ch;
	char* start = client_answer.message;
	srand(time(NULL));


	/*
	 * True: decipher the message
	 * False: cipher the message
	 */
	server_answer.key = client_answer.key ?
			client_answer.key : 1 + (uint8_t)(rand() % 127+1);


	/* Job */
	while (*start)
	{
		server_answer.message[i++] = (ch = *start ^ server_answer.key) >= 32 ?
			ch : *start;
		start++;
	}


	/* Send info to the client */
	ASSERT_MSG(send(client_fd, &server_answer, sizeof(cipher_t), 0) != -1,
			"Failed on send");

	close(client_fd);
	exit(0);
}
