#include <iostream>
#include "livro.h"

//Construtor 
Livro::Livro(std::string nome, std::string id): _nome(nome),_id(id){}

//Cada get retorna seu dado correspondente
std::string Livro::getId(){
    return _id;
}
std::string Livro::getNome(){
    return _nome;
}

void Livro::setId(std::string id) {
    this->_id = id;
}

void Livro::setNome(std::string nome) {
    this->_nome = nome;
}
