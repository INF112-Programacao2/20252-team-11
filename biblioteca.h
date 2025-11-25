#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <vector>
#include <string>

#include "livro.h"
#include "usuario.h"

class Biblioteca {

    private:

    std::vector<Livro*> livros;
    std::vector<Usuario*> usuarios;

    public:

    Biblioteca();
    ~Biblioteca();

    std::vector<Livro*> getLivros() const;
    std::vector<Usuario*> getUsuarios() const;


    void addUsuario(Usuario* usuario);
    void addLivro(Livro* livro) const;


};


#endif
