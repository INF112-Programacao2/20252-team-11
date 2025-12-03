//
// Created by pedro on 11/11/2025.
//
#include "chat.h"
#include <bits/stdc++.h>

//construtor que inicializa arrays dinamicos de capacidade 2
Chat::Chat(){
	num_participantes = 0;

	size_participantes = 2;
	participantes = new Usuario*[size_participantes];
}

//destrutor - libera memoria alocada
Chat::~Chat(){
	//libera cada participante individualmente
	for (int i = 0; i < num_participantes; i++){
		delete participantes[i];
	}
	delete[] participantes;	//libera array
}

//Retorna uma referencia ao livro
Livro& Chat::getLivro() {
	return livro;
}

void Chat::setLivro(const Livro& novoLivro) {
livro = novoLivro; //usa operador de atribuicao
}

//retorna array de participantes
Usuario** Chat::getParticipantes(){
	return participantes;
}

//adiciona participante com redimensionamento automatico do array
void Chat::addParticipante(Usuario* usuario){
	
	//verifica se precisa redimensionar
	if (num_participantes == size_participantes){

		//cria array temporario
		Usuario** aux = new Usuario*[size_participantes];
		for (int i = 0; i < size_participantes; i++){
			aux[i] = participantes[i]; //copia ponteiros
		}
		delete[] participantes;	//libera array antigo

		//aloca novo array com dobro de tamanho
		participantes = new Usuario*[size_participantes*2];
		for (int i = 0; i < size_participantes; i++){
			participantes[i] = aux[i];	//restaura ponteiros
		}
		delete[] aux;		//libera array temporario
		size_participantes *= 2;	//dobra a capacidade
	}
	participantes[num_participantes++] = usuario;	//adiciona participante


}


