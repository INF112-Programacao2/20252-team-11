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

class Client{
	private:
//atributos privados
		std::string address; 	//endereco ip do servidor
		std::string port;		//porta do servidor
		sockaddr_in serv_addr; 	//estrutura de endereco do servidor
		addrinfo hints, *res;	//resolucao de DNS

	protected:
		int client_fd;           // Descritor do socket cliente
		std::thread t_send;      // Thread para envio de mensagens
		std::thread t_recv;      // Thread para recebimento de mensagens
		std::string forum;       // Forum associado ao cliente

	public:
	//construtor e destrutor
		Client();
		~Client();
	//inicia threads
		void run();
		void intercepta_msg();	//recebe mensagem do servidor
		void connect_socket(std::string address, std::string port, std::string nome, std::string forum, std::string matricula, std::string num_chamada);
		void envia_msg(const char* buff, int bytes, int fd); //envia mensagem
		void send_msg(); //le entrada do usuario e envie mensagem
		void close();	//fecha conexao
		void setForum(std::string forum);	//define forum
		std::string getForum();				//retorna forum
};

#endif //CLIENT_H

