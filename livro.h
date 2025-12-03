#ifndef LIVRO_H
#define LIVRO_H

#include <iostream>
#include <vector>
#include "usuario.h"

//classe Livro
class Livro{
private:

    Usuario *_leitorAtual;    //ponteiro para o usuario que esta lendo o livro atualmente

    //informacoes relevantes sobre o livro
    std::string _nome;
    std::string _id;
    std::string _autor;
    
    std::vector<std::string> _interesses;    //lista de interesses relacionados ao livro
    std::vector<Usuario*> _leitores;         //historico de todos os leitores que ja acessaram o livro

public:

//construtor
    Livro(Usuario *usuario, std::string nome, std::string id);

//getters para atrivutos basicos
    std::string getId();
    std::string getNome();
    Usuario *getLeitorAtual();      //retorna ponteiro para leitor atual

    void registrarLeitorAtual();    //adiciona leitor atual ao historico
    void setLeitorAtual(Usuario *novoleitor);    //define novo leitor atual
};

#endif
