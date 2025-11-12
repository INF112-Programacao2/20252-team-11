#ifndef MENSAGEM_CONTROLLER_HPP
#define MENSAGEM_CONTROLLER_HPP

#include <string>
#include <ctime>
#include <memory>
#include <vector>
#include "../model/mensagem.hpp"

class MensagemController{
    private:
        int idUsuario;
        std::vector<Mensagem> mensagens;
    public:
        MensagemController(int);
        ~MensagemController();

        void setMensagem(std::string);
        void putMensagem(std::string, int);
        void deleteMensagem(int);
        std::vector<Mensagem> getMensagens();
        Mensagem* getMensagem(int);
};

#endif