//
// Created by pedro on 13/11/2025.
//

#ifndef SERVER_H
#define SERVER_H
#define BUFFSIZE 1024

#include "chat.h"
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <vector>
#include <bits/stdc++.h>

class Server {
	private:
		Chat** chats;
		int num_chats;
		int size_chats;

		int server;
		pollfd fd_totais[200];
		int num_fd;
		int my_port;
		sockaddr_in serv_addr;

		std::vector <Usuario> clients;
	public:
		Server(int port);
		~Server();
		Chat** get_chats();
		void add_chat(Chat* chat);
		void run();
		int getServer();
		void listen_socket();
		int processa_fd(int &ready);
		char* processa_msg(int cli);
		void envia_msg(const char* buff, int bytes, int fd);
		void receber_descritor(int fd);
		void interpreta_msg(const char* buff, int bytes, int fd);
        void close();
};

#endif //SERVER_H

