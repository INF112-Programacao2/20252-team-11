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

