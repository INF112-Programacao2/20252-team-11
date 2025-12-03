//
// Created by pedro on 11/11/2025.
//
#include "chat.h"
#include <bits/stdc++.h>

//construtor que inicializa arrays dinamicos de capacidade 2
Chat::Chat(){
	num_mensagens = 0;
	num_participantes = 0;
	size_mensagens = 2;
	size_participantes = 2;
	mensagens = new Mensagem*[size_mensagens];
	participantes = new Usuario*[size_participantes];
}

//destrutor - libera memoria alocada
Chat::~Chat(){

	//libera cada mensagem individualmente
	for (int i = 0; i < num_mensagens; i++){
		delete mensagens[i];
	}
	delete[] mensagens; //libera array

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



//retorna array de mensagens
Mensagem** Chat::getMensagens(){
	return mensagens;
}

//retorna array de participantes
Usuario** Chat::getParticipantes(){
	return participantes;
}

//adiciona mensagens com redimensionamento automatico do array
void Chat::addMensagem(Mensagem* mensagem){

	//verifica se precisa redimensionar
	if (num_mensagens == size_mensagens){

		//cria array temporario
		Mensagem** aux = new Mensagem*[size_mensagens];
		for (int i = 0; i < size_mensagens; i++){
			aux[i] = mensagens[i]; //copia ponteiros
		}
		delete[] mensagens; //libera array antigo

		//aloca novo array com o dobro do tamanho
		mensagens = new Mensagem*[size_mensagens*2];
		for (int i = 0; i < size_mensagens; i++){
			mensagens[i] = aux[i];
		}
		delete[] aux; //libera array temporario
		size_mensagens *= 2; //atualiza capacidade
	}
	mensagens[num_mensagens++] = mensagem;	//adiciona mensagem
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


