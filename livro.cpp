#include <iostream>
#include "livro.h"

Livro::Livro(Usuario *usuario, std::string nome, Forum *forum, std::string id, std::string autor, std::vector <std::string> interesses): _leitorAtual(usuario), _nome(nome), _forum(forum),_id(id),_autor(autor), _interesses(interesses){}



std::string Livro::getId(){
    return _id;
}
std::string Livro::getNome(){
    return _nome;
}
std::string Livro::getAutor(){
    return _autor;
}
std::vector <std::string> Livro:: getInteresses(){
    return _interesses;
}
Usuario* Livro::getLeitorAtual(){
    return _leitorAtual;
}

Forum* Livro::getForum(){
    return _forum;
}
void Livro::AddNaListaDeLeitores() {
    if (_leitorAtual != nullptr) {
        for (Usuario* leitor : _leitores) {
            if (leitor->getMatricula() == _leitorAtual->getMatricula()) {
                return; 
            }
        }
        _leitores.push_back(_leitorAtual);
    }
}
void Livro:: setLeitorAtual(Usuario *novoLeitor){
    _leitorAtual=novoLeitor;
}
