#include "biblioteca.h"

Biblioteca::Biblioteca(){};

Biblioteca::~Biblioteca() {

    for (Livro* livro : _livros) {
        delete livro;
    }

    _livros.clear();
    _usuarios.clear();

}

    std::vector<Livro*> Biblioteca::getLivros() const {
        return _livros;
    }

    std::vector<Usuario*> Biblioteca::getUsuarios() const {
        return _usuarios;
    }


    void Biblioteca::addUsuario(Usuario* novoUsuario) {
        if (novoUsuario != nullptr){
            for (Usuario* u : _usuarios) {
                if (u->getMatricula() == novoUsuario->getMatricula()) {
                    return; 
                }
            }
            _usuarios.push_back(novoUsuario);
        }
    }

    void Biblioteca::addLivro(Livro* novoLivro) {
        if (novoLivro != nullptr){
            for(Livro* l : _livros){
                if(l->getId()==novoLivro->getId()){
                    return;
                }
                
            }
            _livros.push_back(novoLivro);
        }   
    }






