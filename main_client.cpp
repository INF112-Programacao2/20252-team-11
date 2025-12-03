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

//A funcao InteracaoUsuario tem o proposito de proporcionar fluxo interativo para usuarios genericos

//main - ponto de entrada principal do cliente
int main() {

    //cria usuario generico
    Usuario *user=nullptr;
    int tipo;
    while (true) {   
        
        cout<<"-------SEJA BEM VINDO AO BETTER PERGAMUM-------\nVoce é:\n1 - Aluno\n2 - Professor\nResposta: ";
        cin>> tipo;
        cin.ignore();
        cout<< "---------------------------------------\n\n\n";
        if(tipo==1){
            user = new Aluno();
            break;
        }
        else if(tipo==2){
            user = new Professor();
            break;
        }
        else {
            cout << "Opção inválida!\nSelecione novamente.\n";
        }
    }

    // Roteamento CORRETO
        user->InteracaoUsuario();

    delete user;
    return 0;
}

