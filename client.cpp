//
// Created by pedro on 16/11/2025.
//
#include "client.h"
#include <cstring>
#include <stdexcept>
#include <errno.h>
#include <thread>
#include <cstring>

#define BUFFSIZE 1024


Client::Client(std::string address, std::string port):
    address(address), port(port), client_fd(-1){}

Client::~Client(){}


void Client::prepara_msg(){
	int bytes_sent;
	while(true){
		char buff[BUFFSIZE];
		std::cin.getline(buff, BUFFSIZE);
        std::string msg = "[!] " + std::string(buff);
		if (std::string(buff).size() != 0){
      	 	try{
				bytes_sent = send(client_fd, msg.c_str(),msg.size(), 0);
				if (bytes_sent<0){
					throw std::runtime_error("Error in send()");
				}
			} catch (std::runtime_error& e){
				std::cerr << e.what() << std::endl;
				this->close();
			}
		}
    }
}

void Client::intercepta_msg(){
	while (true) {
    	char msg[BUFFSIZE];
		try{
			int bytes_recv = recv(client_fd, msg, sizeof(msg), 0);
        	if (bytes_recv == 0)
        		throw std::exception();
			if (bytes_recv < 0){
				throw std::runtime_error("Error in recv()");
			}
		} catch (std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			this->close();
		} catch (std::exception& e){
			continue;
		}

        std::cout << msg << std::endl;
        fflush(stdout);
    }
}

void Client::run(){
	std::thread t1(&Client::intercepta_msg, this);
	std::thread t2(&Client::prepara_msg, this);

	t_send = std::move(t1);
	t_recv = std::move(t2);

	if (t_send.joinable()){
        t_send.join();
	}
	if (t_recv.joinable()){
        t_recv.join();
	}
}

void Client::connect_socket(std::string nome, std::string forum){
	int ret, bytes_sent;
	try{
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (client_fd<0){
			throw std::runtime_error("Error in socket()");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_V4MAPPED;
	try{
		ret = getaddrinfo(address.c_str(),port.c_str(),&hints,&res);
		if (ret!=0){
			throw std::runtime_error("Error in getaddrinfo(), host not found");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}

	std::cout << "Host: " << address << " found.\n";
	memcpy(&serv_addr,res->ai_addr,sizeof(serv_addr));
	freeaddrinfo(res);
	try{
		ret = connect(client_fd, (sockaddr*) &serv_addr, sizeof(serv_addr));
		if (ret<0){
			throw std::system_error(errno, std::system_category(), "Error in connect()");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}
	std::cout << "Conexao estabelecida!\n";

	std::string info = forum + '\0' + nome + '\0';
	try{
		bytes_sent = send(client_fd, info.c_str(), strlen(info.c_str()), 0);
		if (bytes_sent<0){
			throw std::runtime_error("Error in send()");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}
}

void Client::close(){
    shutdown(client_fd, SHUT_RDWR);
    exit(0);
}
