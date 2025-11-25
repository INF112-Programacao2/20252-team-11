<<<<<<< HEAD
#include "biblioteca.h"

Biblioteca::Biblioteca();

Biblioteca::~Biblioteca() {

    for (Livro* livro : livros) {
        delete livro;
    }

    livros.clear();
    usuarios.clear();

}

    std::vector<Livro*> Biblioteca::getLivros() const {
        return livros;
    }

    std::vector<Usuario*> Biblioteca::getUsuarios() const {
        return usuarios;
    }


    void Biblioteca::addUsuario(Usuario* usuario) {
        if (usuario != nullPtr) 
        usuarios.push_back(usuario);

    }

    void Biblioteca::addLivro(Livro* livro) const {
        if (livro != nullPtr)
        livros.push_back(livro);

    }

=======
#include <biblioteca.h>
#include <iostream>


std::vector <Livro*> Biblioteca::getLivros(){
    return _livro; 
}
std::vector <Usuario*> Biblioteca::getUsuarios(){
    return _usuario;
}
void addUsuario(Usuario* novoUsuario) {
    if (novoUsuario != nullptr) {
        for (Usuario* u : _usuario) {
            if (u->getMatricula() == novoUsuario->getMatricula()) { 
                return; 
            }
        }
        _usuario.push_back(novoUsuario);
    }
}
void addLivro(livro* novoLivro){
        if (novoLivro != nullptr) {
            for (Livro* l : _livro) {
                if (l->getId() == novoLivro->getId()) { 
                    return; 
            }
        }
        _livro.push_back(novoLivro);
    }
}



>>>>>>> f310884 (modificacoes efetuadas em livro.cpp e biblioteca.cpp, foram adicionadas verificacoes nas funcoes add para verificar se o item a ser adicionado ja existe na lista)
