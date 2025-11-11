#ifndef LIVRO_H
#define LIVRO_H

#include <iostream>
#include <vector>
#include "usuario.h"
#include "forum.h"


class Livro{
private:
    Usuario *_leitorAtual;
    std::string _nome;
    Forum *_forum;
    std::string _id;
    std::string _autor;
    std::vector<std::string> _interesses;
    std::vector<Usuario*> _leitores;
public:
    Livro(Usuario *usuario, std::string nome, Forum *forum, std::string id, std::string autor, std::vector <std::string> interesses);
    std::string getId();
    std:: string getNome();
    Forum *getForum();
    Usuario *getLeitorAtual();
    std::vector <std::string> getInteresses();
    std::string getAutor();
    void AddNaListaDeLeitores();
    void setLeitorAtual(Usuario *novoleitor);
};
#endif