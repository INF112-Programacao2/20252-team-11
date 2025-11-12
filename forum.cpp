//
// Created by pedro on 11/11/2025.
//
#include "forum.h"

Forum::Forum(Livro* livro):
	livro(livro){
		Chat();
	}

Forum::~Forum(){
	this->~Chat();
}

Livro* Forum::getLivro(){
	return this->livro;
};
