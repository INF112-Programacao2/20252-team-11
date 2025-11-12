#ifndef CHAT_H
#define CHAT_H

#include "mensagem.h"
#include "usuario.h"

class Chat {
	private:
		Mensagem** mensagens;
		Usuario** participantes;
		int num_participantes;
		int num_mensagens;
		int size_mensagens;
		int size_participantes;
	public:
		Chat();
		~Chat();
		Mensagem** getMensagens();
		Usuario** getParticipantes();
		void addMensagem(Mensagem* mensagem);
		void addParticipante(Usuario* usuario);
};


#endif
