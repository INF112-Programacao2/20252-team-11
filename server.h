//
// Created by pedro on 13/11/2025.
//

#ifndef SERVER_H
#define SERVER_H

//Define o tamanho do buffer para leitura/escrita de sockets
#define BUFFSIZE 1024

#include "client.h"
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <vector>
#include <bits/stdc++.h>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include "livro.h"
#include "mensagem/database.hpp"
#include "forum.h"
#include <ctime>
#include <sstream>

//Classe server
class Server {
	private:

		//Socket do servidor
		int server;					// Descritor de socket do servidor
		pollfd fd_totais[200];		// Array de estruturas pollfd para multiplexacao
		int num_fd;					//Numero de descritores monitorados
		int my_port;				//Porta na qual o servidor escuta
		sockaddr_in serv_addr;		//Estrutura de endereco do servidor

		//Gerenciamento de clientes e dados
		std::map <int, Usuario*> clients;
		std::vector<Livro*> livros;

	public:
		//Constutor e destrutor
		Server(int port);
		~Server();

		void run();		//inicia o loop do servidor

		//gerenciamento do socket
		int getServer();               // retorna o descritor de socket do servidor
		void listen_socket();          // coloca o socket em modo de escuta (listen)
		void processa_fd(int &ready);  // processa descritores prontos retornados por poll()
		char* processa_msg(int index); // processa mensagem recebida de um cliente especifico

		//comunicacao
		void envia_msg(const char* buff, int bytes, int fd);	//envia mensagem para cliente
		void receber_descritor(int index);						//aceita nova conexao de cliente
		void interpreta_msg(const char* buff, int bytes, Usuario* user, int fd);	//interpreta e processa mensagem recebida
		void mostra_antigas_msg(std::string num_chamada, int fd);
        void close();	//fecha os ervidor
};

#endif //SERVER_H




