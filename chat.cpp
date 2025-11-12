//
// Created by pedro on 11/11/2025.
//
#include "chat.h"
#include <bits/stdc++.h>

Chat::Chat(){
	num_mensagens = 0;
	num_participantes = 0;
	size_mensagens = 2;
	size_participantes = 2;
	mensagens = new Mensagem*[size_mensagens];
	participantes = new Usuario*[size_participantes];
}

Chat::~Chat(){
	for (int i = 0; i < num_mensagens; i++){
		delete mensagens[i];
	}
	delete[] mensagens;
	for (int i = 0; i < num_participantes; i++){
		delete participantes[i];
	}
	delete[] participantes;
}

Mensagem** Chat::getMensagens(){
	return mensagens;
}

Usuario** Chat::getParticipantes(){
	return participantes;
}

void Chat::addMensagem(Mensagem* mensagem){
	if (num_mensagens == size_mensagens){
		Mensagem** aux = new Mensagem*[size_mensagens];
		for (int i = 0; i < size_mensagens; i++){
			aux[i] = mensagens[i];
		}
		delete[] mensagens;

		mensagens = new Mensagem*[size_mensagens*2];
		for (int i = 0; i < size_mensagens; i++){
			mensagens[i] = aux[i];
		}
		delete[] aux;
		size_mensagens *= 2;
	}
	mensagens[num_mensagens++] = mensagem;
}

void Chat::addParticipante(Usuario* usuario){
	if (num_participantes == size_participantes){
		Usuario** aux = new Usuario*[size_participantes];
		for (int i = 0; i < size_participantes; i++){
			aux[i] = participantes[i];
		}
		delete[] participantes;

		participantes = new Usuario*[size_participantes*2];
		for (int i = 0; i < size_participantes; i++){
			participantes[i] = aux[i];
		}
		delete[] aux;
		size_participantes *= 2;
	}
	participantes[num_participantes++] = usuario;
}