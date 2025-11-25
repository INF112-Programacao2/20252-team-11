//
// Created by pedro on 16/11/2025.
//
#include "client.h"

int main() {
	std::string address = "127.0.0.1";
	std::string port = "12345";
	Client client_send(address, port);
	std::string nome, forum;
	std::cout << "Digite o nome do cliente: ";
	std::getline(std::cin, nome);
	std::cout << "Digite o forum do cliente: ";
	std::cin >> forum;
	client_send.connect_socket(nome, forum);
	client_send.run();
	client_send.close();
}
