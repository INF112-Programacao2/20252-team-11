#ifndef MENSAGEM_HPP
#define MENSAGEM_HPP

#include <string>
#include <ctime>
#include <memory>

class Usuario{};

class Mensagem{
    private:
        std::string conteudo;
        std::time_t dataHora;
        int autor;
        std::weak_ptr<Mensagem> respostaPara;

    public:
        Mensagem();
        Mensagem(std::string, std::time_t, int, std::weak_ptr<Mensagem> = std::weak_ptr<Mensagem>());
        ~Mensagem();

        std::string getConteudo();
        std::time_t getDataHora();
        int getAutor();
        std::weak_ptr<Mensagem> getRespostaPara();

        void setConteudo(std::string);
        void setRespostaPara(std::weak_ptr<Mensagem>);
};


#endif