#include <iostream>                      //ADD TRY CATCH
#include "usuario.h"
#include "aluno.h"
#include "professor.h"
#include <string>
#include <regex>
#include <curl/curl.h>
#include <fstream>
#include <codecvt>
#include "client.h"
using namespace std;

void InteracaoUsuario(Usuario a) {
    a.setCookieValue();
    std::cout << "Cookie de sessão: " << a.getCookie()<< std::endl;
    string matricula, senha;
    cout << "Digite sua matricula: ";
    cin >> matricula;
    cout << "Digite a sua senha da BBT: ";
    cin >> senha;

    bool autenticou = a.autenticar(matricula, senha);

    if (autenticou) {
        a.setInfo();
    }
    else 
        cout << "Matrícula ou senha inválida!\n";
    while(true){  
        int escolha;
        cout<<"Escolha a função que você deseja executar:\n1 - Pesquisa de livros\n2 - Visualizar perfil\n3 - Encerrar programa\n";
        cin>>escolha;
        if (escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            try {
                auto livros = a.buscarLivros(pesquisa);
                cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";
                }
            catch (exception&  e) {
                cerr << e.what() << endl;;
            }
        }
        if(escolha==2){
            cout << "---------------------------------------\n| Nome:\t\t" << a.getNome() << "\n" << "| Email: \t" << a.getEmail() <<"\n| CPF:\t\t" << "\n---------------------------------------\n";

        }
        if (escolha==3){
            exit(0);
        }

    }
}

void InteracaoAluno(Aluno b) {
    b.setCookieValue();
    std::cout << "Cookie de sessão: " << b.getCookie()<< std::endl;
    string matricula, senha;
    cout << "Digite sua matricula: ";
    cin >> matricula;
    cout << "Digite a sua senha da BBT: ";
    cin >> senha;

    bool autenticou = b.autenticar(matricula, senha);

    if (autenticou) {
        b.setInfo();
    }
    else 
        cout << "Matrícula ou senha inválida!\n";
    while(true){  
        int escolha;
        cout<<"Escolha a função que você deseja executar:\n1 - Pesquisa de livros\n2 - Consultar o débito\n3 - Visualizar perfil\n4 - Encerrar programa\n";
        cin>>escolha;
        if (escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            try {
                auto livros = b.buscarLivros(pesquisa);
                cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";
                cout <<"\n\nDeseja acessar o forum do livro?\n\t->Se sim, digite 1.\t\t->Se não, digite qualquer outro número. ";
                cin>>escolha;
                if(escolha=1){
                    string address = "127.0.0.1";
                    string port = "12345";
                    Client cliente;
                    string nome;
                    cout << "Digite o nome do cliente: ";
                    getline(cin, nome);
                    cliente.connect_socket(address, port, nome, "Abelardo", "123");
                    cliente.run();
                    cliente.close();
                }
            }
            catch (exception&  e) {
                cerr << e.what() << endl;;
            }
        }
        if(escolha==2){
            auto debitos = b.searchDebito();

            cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

            for (Usuario::Debito debito : debitos) {
                cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
            }
            cout << "---------------------------------------\n";
        }
        if(escolha==3){
            cout << "---------------------------------------\n| Nome:\t\t" << b.getNome() << "\n" << "| Email: \t" << b.getEmail() <<"\n| CPF:\t\t" << b.getCPF() << "\n| Curso:\t"<< b.getCurso() << "\n| Admissão:\t"<< b.getAdmissao() << "/" << b.getSem() << "\n| Sexo:\t\t" << b.getSexo() << "\n---------------------------------------\n";

        }
        if (escolha==4){
            exit(0);
        }

    }
}

void InteracaoProfessor(Professor c) {
    c.setCookieValue();
    std::cout << "Cookie de sessão: " << c.getCookie()<< std::endl;
    string nome;
    cout << "Digite seu Nome: ";
    getline(cin, nome);
    bool autenticou = c.autenticar(nome);
    if (autenticou) {
        c.setInfo();
    }
    while(true){
        int escolha;
        cout<<"Escolha a funcão você deseja executar:\n1 - Pesquisa de livros\n2 - Visualizar perfil\n3 - Encerrar programa\n";
        cin>>escolha;
        if(escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            auto livros = c.buscarLivros(pesquisa);
            cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";
            cout <<"\n\nDeseja acessar o forum do livro?\nSe sim, digite 1,Caso contrario, digite qualquer outro número: ";
            cin>>escolha;
            if(escolha=1){
                string address = "127.0.0.1";
                string port = "12345";
                Client cliente;
                string nome;
                cout << "Digite o nome do cliente: ";
                getline(cin, nome);
                cliente.connect_socket(address, port, nome, "Abelardo", "123");
                cliente.run();
                cliente.close();
                }
        }
        if(escolha==2){
            cout << "---------------------------------------\n| Nome:\t\t" << c.getNome() << "\n" << "| Email: \t" << c.getEmail() << "\n| Orgao:\t"<< c.getOrgao() << "\n| Departamento:\t"<< c.getDep() << "\n| Telefone:\t\t" << c.getTelefone() << "\n" << "\n---------------------------------------\n";
        }
        if(escolha==3){
            exit(0);
        }
    }
}


int main() {
    Usuario a;
    Aluno b;
    Professor c;

    cout << "Selecione no que deseja fazer login:\n0 - Usuario\n1 - Aluno\n2 - Professor\nResposta: ";
    int resp; 
    cin >> resp;
    cin.ignore();
    switch (resp)
    {
        case 0:
            InteracaoUsuario(a);
            break;
        case 1:
            InteracaoAluno(b);
            break;
        case 2:
            InteracaoProfessor(c);
            break;
        default:
            break;
    }

    return 0;
}