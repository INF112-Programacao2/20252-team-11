//
// Created by pedro on 11/11/2025.
//

#ifndef FORUM_H
#define FORUM_H

#include "chat.h"
#include "livro.h"

class Forum : public Chat{ //Herda de chat
	private:
		//Ponteiro para o livro associado ao forum
		Livro* livro;

	public:
		//Construtor e destrutor
		Forum(Livro* livro);
		~Forum();

		//Getter para livro associado
		Livro *getLivro();
};

#endif //FORUM_H

