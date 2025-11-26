#ifndef USUARIO_H
#define USUARIO_H
#include <string>
#include <vector>

class Usuario {
    protected:
        std::string nome;
        std::string matricula;
        std::string senha;
        std::vector<std::string> interesses;        // vai ser só o histórico
        std::vector<std::string> livros;        // mudar para armazenar livros...
        float debito;
        Usuario* amigos;
        int n_amigos;
        std::string emailInstitucional;
        std::string CPF;
        std::string cookie_value;

    public:

        struct Livro {
            std::string nome;
            std::string numero_chamada;
        };
        struct Debito {
            std::string nome;
            std::string debt;
        };

        Usuario();
        Usuario(std::string matricula, std::string senha);
        //gets
        std::string getNome();
        std::string getMatricula();
        std::vector<std::string> getInteresses();
        float getDebito();
        std::string getCookie();
        std::string getEmail();
        std::string getCPF();
        // funcionalidades
        bool autenticar(std::string matricula,std::string senha);
        void addAmigo(Usuario& amigo);
        virtual void setInfo();     // seta o CPF e email de aluno (talvez com o professor não funcione)    (isso tem que ser polimorfismo btw, implementar no Usuario e no Professor)
        std::vector<Livro> buscarLivros(std::string busca); 
        std::string setCookieValue();
        std::vector<Debito> searchDebito();
};

#endif
