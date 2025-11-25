//
// Created by pedro on 13/11/2025.
//
#include "server.h"
#include <cstring>
#include <stdexcept>
#include <stropts.h>
#include <algorithm>

Server::Server(int my_port):
	size_chats(2),
	num_chats(0),
    num_fd(1),
	my_port(my_port){
	chats = new Chat*[size_chats];
}

Server::~Server(){
	delete [] chats;
}

Chat** Server::get_chats(){
	return chats;
}

void Server::listen_socket(){
	int ret, reusar=1;

	try{
		server = socket(AF_INET, SOCK_STREAM, 0);
		if (server<0){
			throw std::runtime_error("socket() failed");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	try{
		ret = setsockopt(server, SOL_SOCKET, AF_INET, (char*) &reusar, sizeof(reusar));
		if (ret<0){
			throw std::runtime_error("setsockopt() failed");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	try{
		ret = ioctl(server, FIONBIO, (char*) &reusar);
		if (ret==-1){
			throw std::runtime_error("ioctl() failed");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(my_port);
	memcpy(&serv_addr.sin_addr,(&in6addr_any),sizeof(in6addr_any));

	try{
		ret = bind(server, (sockaddr *) &serv_addr, sizeof(serv_addr));
		if (ret<0){
			throw std::runtime_error("bind() failed");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	try{
		ret = listen(server, 32);
		if (ret<0){
			throw std::runtime_error("listen() failed");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	memset(&fd_totais, 0, sizeof(fd_totais));
	fd_totais[0].fd = server;
	fd_totais[0].events = POLLIN;

	std::cout << "Estamos online na porta: " << my_port << std::endl;
}


void Server::add_chat(Chat* chat){
	if (num_chats >= size_chats){
		Chat** aux = new Chat*[size_chats];
		for (int i = 0; i < size_chats; i++){
			aux[i] = chats[i];
		}
		delete [] chats;
		chats = new Chat*[size_chats*2];
		for (int i = 0; i < size_chats; i++){
			chats[i] = aux[i];
		}
		delete [] aux;
		size_chats *= 2;
	}
	chats[num_chats++] = chat;
}

void Server::run(){
	int ready;
	int timeout = 3*60*1000;

	while (true){
		try{
			std::cout << "Esperando em poll()" << std::endl;
			ready = poll(fd_totais, num_fd, timeout);
			if (ready<0){
				throw std::runtime_error("poll() failed");
			}
			else if (ready==0){
				throw std::runtime_error("poll() timed out");
			}
		} catch (std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			break;
		}

		try{
			processa_fd(ready);
		} catch (std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			break;
		}
	}
}

int Server::processa_fd(int &ready){
	int cli;
	bool diminuir_array = false;
	for (int fd=0; fd<num_fd && ready>0; fd++){
		try{
			if (fd_totais[fd].revents == POLLIN){
				ready--;
				if (fd_totais[fd].fd==server){
					do{
						try{
							cli = accept(server, nullptr, nullptr);
							if (cli<0){
								if (errno != EWOULDBLOCK)
									throw std::system_error(errno, std::system_category(), "accept() failed");
								else
									throw std::exception();
							}
						} catch (std::system_error& e){
							throw std::runtime_error(e.what());
							break;
						} catch (std::exception& e){
							break;
						}

						fd_totais[num_fd].fd = cli;
						fd_totais[num_fd++].events = POLLIN;
						printf("Nova conexão, file descriptor %d\n", num_fd);

						try{
							this->receber_descritor(fd_totais[num_fd-1].fd);
						} catch (std::runtime_error& e){
							std::cerr << e.what() << std::endl;
							shutdown(fd_totais[fd].fd, SHUT_RDWR);
							fd_totais[fd].fd = -1;
							diminuir_array = true;
							break;
						}
					} while (cli != -1);
				}
				else{
					try{
						char* msg = this->processa_msg(fd_totais[fd].fd);
						this->interpreta_msg(static_cast <const char*> (msg), strlen(msg), fd_totais[fd].fd);
						delete[] msg;
					} catch (std::runtime_error& e){
						std::cerr << e.what() << std::endl;
						shutdown(fd_totais[fd].fd, SHUT_RDWR);
						fd_totais[fd].fd = -1;
						diminuir_array = true;
						break;
					}
				}
			} else if (fd_totais[fd].revents != 0){
				throw std::runtime_error("Error in revents");
			}
		} catch(std::exception& e){
			throw std::runtime_error(e.what());
			break;
		}
	}

	if (diminuir_array){
		diminuir_array = false;
		for (int i=0; i<num_fd; i++){
			if (fd_totais[i].fd == -1){
				for (int j= i; j+1<num_fd; j++){
					fd_totais[j].fd = fd_totais[j+1].fd;
				}
				i--;
				num_fd--;
			}
		}
	}
}

void Server::receber_descritor(int fd){
	try{
		std::cout << "Descritor: \n";
		char* msg1 = processa_msg(fd);
		std::string matricula = std::string(msg1);
		char* msg2 = processa_msg(fd);
		std::string nome = std::string(msg2);
		std::cout << '\n';
		clients.push_back(Usuario(matricula, nome));
		delete[] msg1;
		delete[] msg2;
	} catch (std::runtime_error& e){
		throw std::runtime_error(e.what());
	}
}

void Server::interpreta_msg(const char* buff, int bytes, int fd){
	std::string prefixo(buff, 4);
	if (prefixo.compare("[!] ") == 0){
		for (auto i : clients){
			if (i.second != fd){
				try{
					envia_msg(buff+4, bytes, i.second);
				} catch (std::runtime_error& e){
					std::cerr << e.what() << std::endl;
					this->close();
				}
			}
		}
	}
	else if (prefixo.compare("quit") ==0 ){
		throw std::runtime_error& e;
	}
}

char* Server::processa_msg(int fd){
	int bytes_recv, bytes_sent;
	char* buffer = new char[BUFFSIZE];
	char* msg = new char[BUFFSIZE];
	memset(msg, '\0', BUFFSIZE);
	try{
		bytes_recv = recv(fd, buffer, BUFFSIZE, 0);
		if (bytes_recv<=0)
			if ((errno != EWOULDBLOCK) || bytes_recv==0){
				throw std::runtime_error("Connection interrupted");
			} else{
				throw std::exception();
			}
	} catch (std::runtime_error& e){
		throw std::runtime_error(e.what());
		return msg;
	} catch (std::exception& e){
		return msg;
	}

	std::cout << "Recibidos " << bytes_recv << " bytes, fd: " << fd << std::endl;
	for (int i=0; i<bytes_recv; i++){
		std::cout << buffer[i];
		msg[i] = buffer[i];
	}
	std::cout << '\n';
	return msg;
}

void Server::envia_msg(const char* buff, int bytes, int fd){
	int bytes_sent;
	try{
		bytes_sent = send(fd, buff, bytes, 0);
		if (bytes_sent<0){
			throw std::runtime_error("Error in send()");
		}
	} catch (std::runtime_error& e){
		throw std::runtime_error(e.what());
	}
}

void Server::close(){
	for (int fd=0; fd<num_fd; fd++){
		if (fd_totais[fd].fd >=0 ){
			shutdown(fd_totais[fd].fd, SHUT_RDWR);
		}
	}

	exit(0);
}





