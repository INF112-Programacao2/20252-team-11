#include <iostream>
#include "usuario.h"
#include <string>
#include <regex>
using namespace std;


/*
        Demonstração de Usuário.
    Quase todas as funcionalidades de usuário sendo implementadas aqui, pelo menos
    aquelas que envolvem puxar dados do sistema.

*/


int main() {
    Usuario a;

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
    auto livros = a.buscarLivros(pesquisa);

    cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";

    auto debitos = a.searchDebito();

    cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

    for (Usuario::Debito debito : debitos) {
        cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
    }
    cout << "---------------------------------------\n";

}



