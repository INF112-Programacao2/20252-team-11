#ifndef CHAT_H
#define CHAT_H

#include "mensagem/model/mensagem.hpp"
#include "usuario.h"

//classe chat
class Chat {
	private:
		Mensagem** mensagens;        // array dinamico de ponteiros para mensagens
		Usuario** participantes;     // array dinamico de ponteiros para participantes
		int num_participantes;       // numero atual de participantes
		int num_mensagens;           // numero atual de mensagens
		int size_mensagens;          // capacidade do array mensagens
		int size_participantes;      // capacidade do array participantes

	public:
	//construtor e destrutor
		Chat();
		~Chat();

		Mensagem** getMensagens();			//retorna array de mensagens
		Usuario** getParticipantes();		//retorna array de participantes
		void addMensagem(Mensagem* mensagem);	//adiciona mensagem
		void addParticipante(Usuario* usuario);	//adiciona participante
};


#endif

