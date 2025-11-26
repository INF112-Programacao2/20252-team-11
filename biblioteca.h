#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H


#include <vector>
#include <string>
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
