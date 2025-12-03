#ifndef CHAT_H
#define CHAT_H

#include "usuario.h"
#include "livro.h"
#include <vector>
#include <utility>

//classe chat
class Chat {
	private:

		std::vector<std::pair<int, Usuario>> participantes;     // array dinamico de ponteiros para participantes
		int num_participantes;       // numero atual de participantes
		int size_participantes;      // capacidade do array participantes
		Livro livro;

	public:
	//construtor e destrutor
		Chat();
		~Chat();

		std::pair<int, Usuario> getParticipante(int id);		//retorna array de participantes
		void addParticipante(Usuario usuario, int id);	//adiciona participante

		Livro& getLivro();
		void setLivro(const Livro& novoLivro);
};


#endif




