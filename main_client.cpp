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
void InteracaoUsuario(Usuario &a) {
    
    //Obtem cookie de sessao do sistema pergamum
    a.setCookieValue();
    std::cout << "Cookie de sessão: " << a.getCookie()<< std::endl;

    //Socilita credenciais de sessao
    string matricula, senha;
    cout << "Digite sua matricula: ";
    cin >> matricula;
    cout << "Digite a sua senha da BBT: ";
    cin >> senha;

    //Tenta autenticar no sistema pergamum
    bool autenticou = a.autenticar(matricula, senha);

    if (autenticou) {
        a.setInfo();    //obtem informacoes adicionais ao usuario
    }
    else 
        cout << "Matrícula ou senha inválida!\n";

    //loop principal do menu interativo
    while(true){  
        int escolha;
        cout<<"Escolha a função que você deseja executar:\n1 - Pesquisa de livros\n2 - Visualizar perfil\n3 - Encerrar programa\n";
        cin>>escolha;

        //OPCAO 1: BUSCA DE LIVROS
        if (escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            try {
                //Busca livros no sistema
                auto livros = a.buscarLivros(pesquisa);
                cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";
                }
            catch (exception&  e) {
                cerr << e.what() << endl;;    //exibe werro se busca falhar
            }
        }

        //OPCAO 2: VISUALIZAR PERFIL
        //exibe informacoes basicas do usuario
        if(escolha==2){
            cout << "---------------------------------------\n| Nome:\t\t" << a.getNome() << "\n" << "| Email: \t" << a.getEmail() <<"\n| CPF:\t\t" << "\n---------------------------------------\n";

        }
        //OPCAO 3: ENCERRAR PROGRAMA
        if (escolha==3){
            exit(0);
        }

    }
}

//FLUXO INTERATIVO PARA ALUNOS
void InteracaoAluno(Aluno &b) {

    //configuracao inicial parecida com usuario
    b.setCookieValue();
    std::cout << "Cookie de sessão: " << b.getCookie()<< std::endl;
    string matricula, senha;
    cout << "Digite sua matricula: ";
    cin >> matricula;
    cout << "Digite a sua senha da BBT: ";
    cin >> senha;

    bool autenticou = b.autenticar(matricula, senha);

    if (autenticou) {
        b.setInfo();    //obtem informacoes especificas de aluno
    }
    else 
        cout << "Matrícula ou senha inválida!\n";

    //Loop principal do menu para alunos
    while(true){  
        int escolha;
        cout<<"Escolha a função que você deseja executar:\n1 - Pesquisa de livros\n2 - Consultar o débito\n3 - Visualizar perfil\n4 - Encerrar programa\nResposta: ";
        cin>>escolha;

        //OPCAO 1: PESQUISA DE LIVROS(com opcao de acessar o forum)
        if (escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            try {
                auto livros = b.buscarLivros(pesquisa);
                cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";

                //oferece opcao de conectar ao servidor de char do livro
                cout <<"\n\nDeseja acessar o forum do livro?\n\t->Se sim, digite 1.\t\t->Se não, digite qualquer outro número. ";
                cin>>escolha;
                if(escolha=1){
                    string address = "127.0.0.1";
                    string port = "12345";
                    Client cliente;
                    string nome;
                    cout << "Digite o nome do cliente: ";
                    getline(cin, nome);

                    //conecta ao servidor de chat
                    cliente.connect_socket(address, port, nome, "Abelardo", "123");
                    cliente.run();
                    cliente.close();
                }
            }
            catch (exception&  e) {
                cerr << e.what() << endl;;
            }
        }

        //OPCAO 2: CONSULTA DE DEBITOS
        if(escolha==2){
            //busca debitos do aluno no sistema
            auto debitos = b.searchDebito();

            cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

            //lista todos os debitos
            for (Usuario::Debito debito : debitos) {
                cout << "| Debt:\t" << debito.nome << "\n|\tValor:\t" << debito.debt << "\n";
            }
            cout << "---------------------------------------\n";
        }
        //OPCAO 3: VISUALIZAR PERFIL COMPLETO
        if(escolha==3){
            //exibe todas as informacoes do aluno
            cout << "---------------------------------------\n| Nome:\t\t" << b.getNome() << "\n" << "| Email: \t" << b.getEmail() <<"\n| CPF:\t\t" << b.getCPF() << "\n| Curso:\t"<< b.getCurso() << "\n| Admissão:\t"<< b.getAdmissao() << "/" << b.getSem() << "\n| Sexo:\t\t" << b.getSexo() << "\n---------------------------------------\n";

        }
        //OPCAO 4: ENCERRAR PROGRAMA
        if (escolha==4){
            exit(0);
        }

    }
}

//FLUXO INTERATIVO DO PROFESSOR
void InteracaoProfessor(Professor &c) {

    //autenticacao simplificada para professores (nome)
    c.setCookieValue();
    std::cout << "Cookie de sessão: " << c.getCookie()<< std::endl;
    string nome, dep;
    cout << "Digite seu Nome: ";
    getline(cin, nome);
    cout << "Digite a sigla do seu Departamento (ex: DMA, DPI): ";
    cin.ignore();
    getline(cin, dep);
    bool autenticou = c.autenticar(nome, dep);
    if (autenticou) {
        c.setInfo(); //busca informacoes do professor
    }

    //loop principal do menu p/ professores
    while(true){
        int escolha;
        cout<<"Escolha a funcão você deseja executar:\n1 - Pesquisa de livros\n2 - Visualizar perfil\n3 - Encerrar programa\nResposta: ";
        cin>>escolha;

        //OPCAO 1: PESQUISA DE LIVROS
        if(escolha==1){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            auto livros = c.buscarLivros(pesquisa);
            cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[2].nome << " \n| N.Chamada:\t"<< livros[2].numero_chamada << "\n---------------------------------------\n";
            
            //opcao de se conectar ao servidor de chat
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

        //OPCAO 2: VISUALIZAR PERFIL PESSOAL
        if(escolha==2){
            cout << "---------------------------------------\n| Nome:\t\t" << c.getNome() << "\n" << "| Email: \t" << c.getEmail() << "\n| Orgao:\t"<< c.getOrgao() << "\n| Departamento:\t"<< c.getDep() << "\n| Telefone:\t" << c.getTelefone() << "\n" << "\n---------------------------------------\n";
        }

        //OPCAO 3: ENCERRAR
        if(escolha==3){
            exit(0);
        }
    }
}

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
    if (tipo == 1)
        InteracaoAluno(*(Aluno*)user);

    else if (tipo == 2)
        InteracaoProfessor(*(Professor*)user);

    delete user;
    return 0;
}

//faser interacao como interface em usuario
