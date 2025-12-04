//
// Created by pedro on 16/11/2025.
//
#include "client.h"

#define BUFFSIZE 1024 //tamanho maximo do buffer de mensagem

//Construtor e destrutor
Client::Client():
   	client_fd(-1){
	}

Client::~Client(){}


//metodo que recebe mensagens do servidor em loop continuo
//executado em thread separada
void Client::intercepta_msg(){
	while (true) {
    	char msg[BUFFSIZE];
		int bytes_recv;
		try{
			//recebe dados do socket
			bytes_recv = recv(client_fd, msg, sizeof(msg), 0);
        	if (bytes_recv == 0)
        		throw std::runtime_error("Conversa encerrada");
			else if (bytes_recv < 0){
				throw std::runtime_error("Error in recv()");
			}
		} catch (std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			this->close(); 	//fecha conexao em caso de erro
		}
	//processa e exibe mensagem recebida
        for (int i=0; i<bytes_recv; i++){
			if (msg[i] == '~'){ 	//caractere especial para nova linha
				std::cout << '\n';
				continue;
			}
			std::cout << msg[i];
		}
        fflush(stdout); 	//forca exibicao imediata
    }
}

//inicia threads de envio e recebimento
void Client::run(){
	//thread para recebimento de mensagens
	std::thread t1(&Client::intercepta_msg, this);

	//thread para envio de mensagens
	std::thread t2(&Client::send_msg, this);
	
	// move threads para membros da classe
	t_send = std::move(t1);
	t_recv = std::move(t2);

	//espera threads terminarem
	if (t_send.joinable()){
        t_send.join();
	}
	if (t_recv.joinable()){
        t_recv.join();
	}
}

//estabelece conexao com o servidor e envia informacoes iniciais
void Client::connect_socket(std::string address, std::string port, std::string nome, std::string forum, std::string matricula, std::string num_chamada){
	int ret, bytes_sent;
	this->address = address;
	this->port = port;

	//CRIACAO DO SOCKET
	try{
		client_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (client_fd<0){
			throw std::runtime_error("Error in socket()");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}

	//CONFIGURACAO DE HINTS PARA getaddrinfo
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_V4MAPPED;

	//RESOLUCAO DE ENDERECO
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

	//COPIA ENDERECO RESOLVIDO
	memcpy(&serv_addr,res->ai_addr,sizeof(serv_addr));
	freeaddrinfo(res);

	//CONEXAO COM O SERVIDOR
	try{
		ret = connect(client_fd, (sockaddr*) &serv_addr, sizeof(serv_addr));
		if (ret<0){
			throw std::system_error(errno, std::system_category(), "Error in connect()");
		}
	} catch (std::runtime_error& e){
		std::cerr << e.what() << std::endl;
		this->close();
	}
	std::cout << "Conexao estabelecida!\n\n\n";
	std::cout << "Bem-vindo ao fórum do livro: " << forum << "\n\n";

	//ENVIO DE INFORMACOES INICIAIS AO SERVIDOR
	std::string info = nome + '\7' + forum + '\7' + matricula + '\7' + num_chamada + '\0';
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

//metodo que le entrada do usuario e envia mensagens
//executado em thread separada
void Client::send_msg(){
	int bytes_sent;
	while(true){	//loop infinito
		char buff[BUFFSIZE];
		std::string msg;
		std::cin.getline(buff, BUFFSIZE); //le linha do console

		//processa comando "quit"
		if (std::string(buff).compare("quit") == 0){
			msg = std::string(buff);
		}
		else{
			msg = "[!] " + std::string(buff) + "~"; //formata mensagem normal
		}

		//se mensagem nao vazia, envia
		if (std::string(buff).size() != 0){
			try{
				bytes_sent = send(client_fd, msg.c_str(),msg.size(), 0);
				if (bytes_sent<0){
					throw std::runtime_error("Error in send()");
				}

				//se mensagem for "quit", termina
				if (msg.compare("quit")==0){
					throw std::runtime_error("Você saiu do chat");
				}
			} catch (std::runtime_error& e){
				std::cerr << e.what() << std::endl;
				this->close();
			}
		}
	}
}

//define forum associado ao cliente
void Client::setForum(std::string forum){
	this->forum = forum;
}

//retorna forum associado
std::string Client::getForum(){
	return forum;
}

//fecha conexao e e termina programa
void Client::close(){
    shutdown(client_fd, SHUT_RDWR);
    exit(0);
}

