#ifndef USUARIO_H
#define USUARIO_H
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <curl/curl.h>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <sys/socket.h>

class Usuario{
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
        //gets
        std::string getNome();
        std::string getMatricula();
        std::vector<std::string> getInteresses();
        float getDebito();
        std::string getCookie();
        std::string getEmail();
        void setNome(std::string nome);
        void setMatricula(std::string matricula);
        // funcionalidades
        bool autenticar(std::string matricula,std::string senha);       // overload
        bool autenticar(std::string nome);
        void addAmigo(Usuario& amigo);
        void virtual setInfo();     // seta o CPF e email de aluno (talvez com o professor não funcione)
        std::vector<Livro> buscarLivros(std::string busca); 
        std::string setCookieValue();
        std::vector<Debito> searchDebito();
};

#endif
