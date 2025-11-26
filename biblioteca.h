#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include <string>

#include "livro.h"
#include "usuario.h"

class Biblioteca {

    private:

    std::vector<Livro*> _livros;
    std::vector<Usuario*> _usuarios;

    public:

    Biblioteca();
    ~Biblioteca();

    std::vector<Livro*>& getLivros() const;
    std::vector<Usuario*>& getUsuarios() const;


    void addUsuario(Usuario* novoUsuario);
    void addLivro(Livro* livro);


};


#endif