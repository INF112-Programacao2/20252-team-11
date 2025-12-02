#ifndef LIVRO_H
#define LIVRO_H

#include <iostream>
#include <vector>
#include "usuario.h"


class Livro{
private:
    Usuario *_leitorAtual;
    std::string _nome;
    std::string _id;
    std::string _autor;
    std::vector<std::string> _interesses;
    std::vector<Usuario*> _leitores;
public:
    Livro(Usuario *usuario, std::string nome, std::string id);
    std::string getId();
    std::string getNome();
    Usuario *getLeitorAtual();
    void registrarLeitorAtual();
    void setLeitorAtual(Usuario *novoleitor);
};
#endif