#ifndef ALUNO_H
#define ALUNO_H
#include "usuario.h"
#include <string>

class Aluno : public Usuario {
    private:
        std::string cpf;
        std::string curso;
        std::string admissao;
    public:
        void sendMessage();
        std::string getCurso();
        std::string getAdmissao();

        void setInfoAluno();

};

#endif
