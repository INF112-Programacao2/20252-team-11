//
// Created by pedro on 11/11/2025.
//
#include "forum.h"

//Construtor : associa forum a um livro e chama construtor da classe base
Forum::Forum(Livro* livro):
	livro(livro){
		Chat();
	}

//Destrutor
Forum::~Forum(){
	this->~Chat();
}

//retorna ponteiro para livro associado ao forum
Livro* Forum::getLivro(){
	return this->livro;
};

