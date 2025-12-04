#ifndef CHAT_H
#define CHAT_H

#include "usuario.h"
#include <vector>
#include <utility>

//classe chat
class Chat {
	private:

		std::vector<std::pair<int, Usuario>> participantes;     // array dinamico de ponteiros para participantes
		Livro livro;

	public:
	//construtor e destrutor
		Chat(Livro livro);
		~Chat();

		std::vector<std::pair<int, Usuario>> getParticipantes();		//retorna array de participantes
		void addParticipante(Usuario usuario, int id);	//adiciona participante

		Livro& getLivro();
};


#endif





