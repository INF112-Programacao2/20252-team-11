#include "usuario.h"
#include "aluno.h"
#include "professor.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {

    for (int i=0; i<argc; i++) cout << argv[i] << "\n";
    Usuario a;
    string busca;
    cout << "-------- BUSCA DE LIVROS NO PERGAMUM --------\nDigite a sua busca: ";
    getline(cin, busca);
    vector<Usuario::Livro> livros = a.buscarLivros(busca);
   for (auto livro : livros) {
        cout << "\n---------------------------------------\n| Nome:\t\t" << livro.nome << " \n| N.Chamada:\t"<< livro.numero_chamada << "\n---------------------------------------\n";
    }
    
    return 0;
}


