#include <iostream>
#include "usuario.h"
#include "aluno.h"
#include "professor.h"
#include <string>
#include <regex>
#include <curl/curl.h>
#include <fstream>
#include <codecvt>
using namespace std;


/*
        Demonstração de Usuário.
    Quase todas as funcionalidades de usuário sendo implementadas aqui, pelo menos
    aquelas que envolvem puxar dados do sistema.

    compilação: g++ main.cpp usuario.cpp -o main -lcurl

*/


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
        cout << "---------------------------------------\n| Nome:\t\t" << a.getNome() << "\n" << "| Email: \t" << a.getEmail() <<"\n| CPF:\t\t" << a.getCPF() << "\n---------------------------------------\n";
    }
    else 
        cout << "Matrícula ou senha inválida!\n";
    cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
    string pesquisa;
    cin >> pesquisa;
//    pesquisa = "Charles Baudelaire : um lírico no auge do capitalismo / 1995 - ( Livros )";
    auto livros = a.buscarLivros(pesquisa);

    for (auto livro : livros) {
        cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livro.nome << " \n| N.Chamada:\t"<< livro.numero_chamada << "\n---------------------------------------\n";
    }
    auto debitos = a.searchDebito();

    cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

    for (Usuario::Debito debito : debitos) {
        cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
    }
    cout << "---------------------------------------\n";
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
        cout << "---------------------------------------\n| Nome:\t\t" << b.getNome() << "\n" << "| Email: \t" << b.getEmail() <<"\n| CPF:\t\t" << b.getCPF() << "\n| Curso:\t"<< b.getCurso() << "\n| Admissão:\t"<< b.getAdmissao() << "\n| Sexo:\t\t" << b.getSexo() << "\n| Sem:\t\t" << b.getSem() << "\n---------------------------------------\n";
    }
    else 
        cout << "Matrícula ou senha inválida!\n";
    cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
    string pesquisa;
    cin >> pesquisa;
//    pesquisa = "Charles Baudelaire : um lírico no auge do capitalismo / 1995 - ( Livros )";
    auto livros = b.buscarLivros(pesquisa);

//    for (auto livro : livros) {
//        cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livro.nome << " \n| N.Chamada:\t"<< livro.numero_chamada << "\n---------------------------------------\n";
//    }

    cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";

    auto debitos = b.searchDebito();

        cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

        for (Usuario::Debito debito : debitos) {
            cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
        }
        cout << "---------------------------------------\n";

}

void InteracaoProfessor(Professor b) {
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
        cout << "---------------------------------------\n| Nome:\t\t" << b.getNome() << "\n" << "| Email: \t" << b.getEmail() <<"\n| CPF:\t\t" << b.getCPF() << "\n| Orgao:\t"<< b.getOrgao() << "\n| Departamento:\t"<< b.getDep() << "\n| Telefone:\t\t" << b.getTelefone() << "\n" << "\n---------------------------------------\n";
    }
    else 
        cout << "Matrícula ou senha inválida!\n";
    cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
    string pesquisa;
    cin >> pesquisa;
//    pesquisa = "Charles Baudelaire : um lírico no auge do capitalismo / 1995 - ( Livros )";
    auto livros = b.buscarLivros(pesquisa);

//    for (auto livro : livros) {
//        cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livro.nome << " \n| N.Chamada:\t"<< livro.numero_chamada << "\n---------------------------------------\n";
//    }

    cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";

    auto debitos = b.searchDebito();

        cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

        for (Usuario::Debito debito : debitos) {
            cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
        }
        cout << "---------------------------------------\n";

}


int main() {
    Usuario a;
    Aluno b;
    Professor c;

    cout << "Selecione no que deseja fazer login:\n0 - Usuario\n1 - Aluno\n2 - Professor\nResposta: ";
    int resp; cin >> resp;
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

}



