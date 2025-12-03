#include "biblioteca.h"

//construtor e destrutor
Biblioteca::Biblioteca(){};

Biblioteca::~Biblioteca() {

    //libera memoria de todos os livros
    for (Livro* livro : _livros) {
        delete livro;
    }

    //limpa os vetores
    _livros.clear();
    _usuarios.clear();
}

    //os gets retornam copias do vetor livros e usuarios respectivamente
    std::vector<Livro*> Biblioteca::getLivros() const {
        return _livros;
    }

    std::vector<Usuario*> Biblioteca::getUsuarios() const {
        return _usuarios;
    }

    //adiciona usuario a bilblioteca, evitando duplicatas por matricula
    void Biblioteca::addUsuario(Usuario* novoUsuario) {
        if (novoUsuario != nullptr){
            for (Usuario* u : _usuarios) {
                if (u->getMatricula() == novoUsuario->getMatricula()) {
                    return; 
                }
            }
            _usuarios.push_back(novoUsuario);    //adiciona novo usuario
        }
    }

 //adiciona livro a bilblioteca, evitando duplicatas por ID
    void Biblioteca::addLivro(Livro* novoLivro) {
        if (novoLivro != nullptr){
            for(Livro* l : _livros){
                if(l->getId()==novoLivro->getId()){
                    return;
                }
                
            }
            _livros.push_back(novoLivro);    //adiciona novo livro
        }   
    }







