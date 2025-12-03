#ifndef LIVRO_H
#define LIVRO_H

#include <iostream>
#include <vector>

//classe Livro
class Livro{
private:
    //informacoes relevantes sobre o livro
    std::string _nome;
    std::string _id;

public:

//construtor
    Livro(std::string nome, std::string id);

//getters para atrivutos basicos
    std::string getId();
    std::string getNome();

    void setId(std::string id);
    void setNome(std::string nome);
};

#endif
