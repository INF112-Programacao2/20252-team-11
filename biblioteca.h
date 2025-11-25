#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

<<<<<<< HEAD
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
=======
#include "livro.h"
#include "usuario.h"

class Biblioteca{
private:
    std::vector<Livro*> _livro;
    std:: vector<Usuario*> _usuario;
public:
    std::vector<Livro*> getLivros();
    std::vector<Usuario*> getUsuarios();
    void addUsuario(Usuario* novoUsuario); 
    void addLivro(Livro* novoLivro); 

};
#endif
>>>>>>> f310884 (modificacoes efetuadas em livro.cpp e biblioteca.cpp, foram adicionadas verificacoes nas funcoes add para verificar se o item a ser adicionado ja existe na lista)
