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
#include <netdb.h>

class Client{
	private:
        int client_fd;
		std::string address;
		std::string port;
		sockaddr_in serv_addr;
		addrinfo hints, *res;

		std::thread t_send;
		std::thread t_recv;
	public:
		Client(std::string address, std::string port);
		~Client();
		void run();
		void prepara_msg();
		void intercepta_msg();
		void connect_socket(std::string nome, std::string forum);
		void envia_msg(const char* buff, int bytes, int fd);
        void close();
};

#endif //CLIENT_H
