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

