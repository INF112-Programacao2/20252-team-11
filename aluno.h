#ifndef ALUNO_H
#define ALUNO_H
#include "usuario.h"
#include <string>

class Aluno : public Usuario { //Aluno herda de usuario
    private:
        //atributos especificos do aluno  
        std::string curso;
        std::string admissao;
        std::string sexo;
        std::string sem;
        std::string CPF;
    public:
        //getters de atributos especificos
        std::string getCPF();
        std::string getCurso();
        std::string getAdmissao();
        std::string getSexo();
        std::string getSem();

        //metodo para buscar debitos do aluno
        std::vector<Debito> searchDebito();        

        //override do metodo virtual da classe base
        void setInfo() override;

};

#endif
