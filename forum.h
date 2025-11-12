//
// Created by pedro on 11/11/2025.
//

#ifndef FORUM_H
#define FORUM_H

#include "chat.h"
#include "livro.h"

class Forum : public Chat{
	private:
		Livro* livro;
	public:
		Forum(Livro* livro);
		~Forum();
		Livro *getLivro();
};

#endif //FORUM_H
