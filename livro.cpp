#include <iostream>
#include "livro.h"

Livro::Livro(Usuario *usuario, std::string nome, std::string id): _leitorAtual(usuario), _nome(nome),_id(id),{}



std::string Livro::getId(){
    return _id;
}
std::string Livro::getNome(){
    return _nome;
}

Usuario* Livro::getLeitorAtual(){
    return _leitorAtual;
}

void Livro::registrarLeitorAtual() {
    if (_leitorAtual != nullptr) {
        for (Usuario* leitor : _leitores) {
            if (leitor->getMatricula() == _leitorAtual->getMatricula()) {
                return; 
            }
        }
        _leitores.push_back(_leitorAtual);
    }
}
void Livro::setLeitorAtual(Usuario *novoLeitor){
    _leitorAtual=novoLeitor;
}
