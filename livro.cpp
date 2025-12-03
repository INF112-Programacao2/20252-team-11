#include <iostream>
#include "livro.h"

//Construtor 
Livro::Livro(Usuario *usuario, std::string nome, std::string id): _leitorAtual(usuario), _nome(nome),_id(id){}

//Cada get retorna seu dado correspondente
std::string Livro::getId(){
    return _id;
}
std::string Livro::getNome(){
    return _nome;
}

Usuario* Livro::getLeitorAtual(){
    return _leitorAtual;
}

//Adiciona leitor atual ao vetor de leitores historicos
//Evita duplicatas verificando matricula
void Livro::registrarLeitorAtual() {
    
    if (_leitorAtual != nullptr) {    //verifica se ha leitor atual
        //percorre leitores existentes para evitar duplicacao
        
        for (Usuario* leitor : _leitores) {
            if (leitor->getMatricula() == _leitorAtual->getMatricula()) {
                return; //leitor ja registrado, nao faz nada
            }
        }
        _leitores.push_back(_leitorAtual); //Adiciona novo leitor ao historico
    }
}
//define novo leitor atual(substitui o anterior)
void Livro::setLeitorAtual(Usuario *novoLeitor){
    _leitorAtual=novoLeitor;
}

