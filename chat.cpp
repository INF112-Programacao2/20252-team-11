//
// Created by pedro on 11/11/2025.
//
#include "chat.h"
#include <iostream>

//construtor que inicializa arrays dinamicos de capacidade 2
Chat::Chat(){}

//destrutor - libera memoria alocada
Chat::~Chat(){}

//Retorna uma referencia ao livro
Livro& Chat::getLivro() {
	return livro;
}

void Chat::setLivro(const Livro& novoLivro) {
livro = novoLivro; //usa operador de atribuicao
}

//retorna array de participantes
std::pair <int, Usuario> Chat::getParticipante(int id){
	try{
		if (id < 0 || id > participantes.size()){
			throw std::exception();
		}
	} catch (std::exception &e){
		std::cerr << "Id invalido";
		exit(-1);
	}
	return participantes[id];
}

//adiciona participante com redimensionamento automatico do array
void Chat::addParticipante(Usuario usuario, int id){
	participantes.push_back(std::make_pair(id, usuario));
}



