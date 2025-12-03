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
        //gets que retornam dados especificos
        std::string getOrgao();
        std::string getDep();
        std::string getTelefone();

        //adiciona o forum
        void addForum();

        //define a informacao  
        void setInfo() override;
};

#endif
