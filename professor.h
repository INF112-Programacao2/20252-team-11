#ifndef PROFESSOR_H
#define PROFESSOR_H
#include "usuario.h"
#include <string>

//Classe professor. Todo professor eh um usuario
class Professor : public Usuario {
    private:

        //strings que informam seus dados pessoais  
        std::string orgao;
        std::string departamento;
        std::string telefone;
    
    public:
        Professor();
        //gets que retornam dados especificos
        std::string getOrgao();
        std::string getDep();
        std::string getTelefone();

        //define a informacao  
        void setInfo() override;
        bool autenticar(std::string nome, std::string email) override;
        //interacao do professor
        void InteracaoUsuario() override;
};

#endif
