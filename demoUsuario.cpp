#include <iostream>
#include "usuario.h"
#include <string>
#include <regex>
#include "client.h"
using namespace std;


/*
        Demonstração de Usuário.
    Quase todas as funcionalidades de usuário sendo implementadas aqui, pelo menos
    aquelas que envolvem puxar dados do sistema.

    compilação: g++ main.cpp usuario.cpp -o main -lcurl

*/


int main() {
    Usuario a;

    string address = "127.0.0.1";
    string port = "12345";
    Client client_send(address, port);
        
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

    string nome, forum;
    cout << "Digite o nome do cliente: ";
    getline(cin, nome);
    cout << "Digite o forum do cliente: ";
    cin >> forum;
    client_send.connect_socket(nome, forum);
    client_send.run();
    client_send.close();
}



