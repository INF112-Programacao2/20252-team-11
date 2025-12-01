#ifndef ALUNO_H
#define ALUNO_H
#include "usuario.h"
#include <string>

class Aluno : public Usuario {
    private:
        std::string curso;
        std::string admissao;
        std::string sexo;
        std::string sem;
    public:
        std::string getCurso();
        std::string getAdmissao();
        std::string getSexo();
        std::string getSem();

        void setInfo() override;

};

#endif
