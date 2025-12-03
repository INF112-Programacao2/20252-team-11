#ifndef CHAT_H
#define CHAT_H

#include "usuario.h"
#include "livro.h"

//classe chat
class Chat {
	private:

		Usuario** participantes;     // array dinamico de ponteiros para participantes
		int num_participantes;       // numero atual de participantes
		int size_participantes;      // capacidade do array participantes
		Livro livro;

	public:
	//construtor e destrutor
		Chat();
		~Chat();

		Usuario** getParticipantes();		//retorna array de participantes
		void addParticipante(Usuario* usuario);	//adiciona participante

		Livro& getLivro();
		void setLivro(const Livro& novoLivro);
};


#endif



