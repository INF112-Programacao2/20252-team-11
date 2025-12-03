//
// Created by pedro on 16/11/2025.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>
#include <errno.h>
#include <thread>
#include <netdb.h>
#include "chat.h"

class Client{
	private:
		std::string address;
		std::string port;
		sockaddr_in serv_addr;
		addrinfo hints, *res;

	protected:
		int client_fd;
		std::thread t_send;
		std::thread t_recv;
		std::string forum;
	public:
		Client();
		~Client();
		void run();
		void intercepta_msg();
		void connect_socket(std::string address, std::string port, std::string nome, std::string forum, std::string matricula);
		void envia_msg(const char* buff, int bytes, int fd);
		void send_msg();
		void close();
		void setForum(std::string forum);
		std::string getForum();
};

#endif //CLIENT_H
