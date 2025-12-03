#include "usuario.h"
#include "aluno.h"
#include "professor.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    //exibe argumentos da linha de comando (para debug)
    for (int i=0; i<argc; i++) cout << argv[i] << "\n";

    //cria objeto usuario para inicia a busca
    Usuario a;
    string busca;

    //interface simples de linha de comando
    cout << "-------- BUSCA DE LIVROS NO PERGAMUM --------\nDigite a sua busca: ";
    getline(cin, busca);    //le toda a linha

    //realiza busca de livros usando o metodo buscarLivros da classe Usuario
    vector<Usuario::Livro> livros = a.buscarLivros(busca);

    //exibe resultados
   for (auto livro : livros) {
        cout << "\n---------------------------------------\n| Nome:\t\t" << livro.nome << " \n| N.Chamada:\t"<< livro.numero_chamada << "\n---------------------------------------\n";
    }
    
    return 0;
}


