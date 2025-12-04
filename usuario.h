#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <sys/socket.h>
#include "client.h"

#include "livro.h"


class Usuario{
    protected:
        std::string nome;                        //nome do usuario
        std::string matricula;                   //matricula do usuario
        std::string senha;                       //senha do usuario
        int chatId;                                //chat 
        std::vector<std::string> interesses;      //historico de interesses
        std::vector<std::string> livros;        //lista de livros
        float debito;                            //debito do usuario
        Usuario* amigos;                        //ponteiro para array de amigos

        int n_amigos;                            //contador de amigos
        std::string emailInstitucional;          //e-mail da UFV
        std::string cookie_value;                //valor do cookie para cada sessao

    public: 

        //estrutura do debito (nome associado ao debito e valor)
        struct Debito {
            std::string nome;
            std::string debt;
        };
        Usuario();
        //Os gets retornam seus respectivos valores. getNome retorna nome, getMatricula retorn matricula, e assim sucessivamente...
        std::string getNome();
        std::string getMatricula();
        std::vector<std::string> getInteresses();
        float getDebito();
        std::string getCookie();
        std::string getEmail();
        int getChatId();

        //Os sets definem seus respectivos valores. setNome define nome, assim por conseguinte...
        void setNome(std::string nome);
        void setMatricula(std::string matricula);
        void setchatId(int id);


        //Funcionalidades
        bool virtual autenticar(std::string matricula,std::string senha);  // Autenticacao com matricula e senha (overload)
        void addAmigo(Usuario& amigo);                             // Adiciona amigo
        void virtual setInfo();                                    // Define informacoes do usuario (metodo virtual)
        std::vector<Livro> buscarLivros(std::string busca);       // Busca livros baseado em string de busca
        std::string setCookieValue();                             // Define e retorna valor do cookie
        void virtual InteracaoUsuario();                        // Fluxo interativo do usuario
};

#endif
