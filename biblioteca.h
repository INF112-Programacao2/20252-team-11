#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include <string>

#include "livro.h"
#include "usuario.h"

class Biblioteca {

    private:
    //colecoes de biblioteca
    //lista de livros da biblioteca e de usuarios cadastrados
    std::vector<Livro*> _livros;        
    std::vector<Usuario*> _usuarios;

    public:
    //Construtor e destrutor
    Biblioteca();
    ~Biblioteca();

    //Getters - retornam copias do vetor de livros (getLivros) e do vetor de usuarios (getUsuarios)
    std::vector<Livro*> getLivros() const;
    std::vector<Usuario*> getUsuarios() const;

    //Metodos que adicionam elementos - usuario e livro
    void addUsuario(Usuario* novoUsuario);
    void addLivro(Livro* livro);


};



#endif
