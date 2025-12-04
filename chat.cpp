//
// Created by pedro on 11/11/2025.
//
#include "chat.h"
#include <bits/stdc++.h>
#include <iostream>

//construtor que inicializa arrays dinamicos de capacidade 2
Chat::Chat(Livro livro):livro(livro){}

//destrutor - libera memoria alocada
Chat::~Chat(){}

//Retorna uma referencia ao livro
Livro& Chat::getLivro() {
	return livro;
}

//retorna array de participantes
std::vector<std::pair<int, Usuario>> Chat::getParticipantes(){
	return participantes;
}

//adiciona participante com redimensionamento automatico do array
void Chat::addParticipante(Usuario usuario, int id){
	participantes.push_back(std::make_pair(id, usuario));
}

void Chat::removeParticipantes(std::string matricula){
	for(int i = 0; i < participantes.size();){
		if(participantes[i].second.getMatricula() == matricula){
			if(participantes.begin() + i < participantes.size())
				participantes.erase(participantes.begin() + i);
		}else
			i++;
	}
}
