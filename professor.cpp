#include "professor.h"

//Inclusoes para requisicoes HTTP com cURL e manipulacao de strings
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cstdlib> 
using namespace std;

//FUNCOES AUXILIARES
//Callback para escrita de dados recebidos pelo cURL
//Concatena dados HTTP em uma string fornecida pelo usuario
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Funcao segura para ler variaveis de usuario
//Retorna string vazia se a varaivel nao existir
static std::string getenv_safe(const char* name) {
    const char* v = std::getenv(name);
    return v ? std::string(v) : std::string();
}

//GETTERS
//Retorna as informacoes pessoais do professor
string Professor::getDep() {
    return this->departamento;
}

string Professor::getOrgao() {
    return this->orgao;
}

string Professor::getTelefone() {
    return this->telefone;
}

//FUNCAO DE PARSING DE RESPOSTA
//Analisa resposta do servidor que contem dados no formato JSON-like
//Converte aspas simples para aspas duplas para facilitar
//Extrai campos especificos de um objeto JSON contido na string
vector<string> parseServidor(string texto)
    {try {
        {
        for (char &c : texto)
            if (c == '\'') c = '"';

        //Encontra o objeto JSON dentro do texto
        size_t inicio = texto.find("{");
        size_t fim = texto.find("}");
        if (inicio == string::npos || fim == string::npos)
            return {};    //Retorna vetor vazio se nao encontrar objeto JSON

        //extrai apenas a parte do objeto JSON
        string obj = texto.substr(inicio, fim - inicio + 1);

        //funcao que extrai valor de um campo especifico do JSON
        //procura por padrao "chave" : "valor"
        auto getField = [&](string key) -> string {
            string search = "\"" + key + "\":";
            size_t pos = obj.find(search);
            if (pos == string::npos) return "";

            pos += search.size();

            //Pula espacos em branco
            while (pos < obj.size() && (obj[pos] == ' ')) pos++;

            //Verifica se o valor esta entre aspas
            if (obj[pos] != '"') return "";

            pos++;    //avana para o primeiro caractere do valor

            //encontra o fechamento das aspas
            size_t end = obj.find("\"", pos);
            if (end == string::npos) return "";

            //retorna o valor entre aspas
            return obj.substr(pos, end - pos);
        };

        

        //rxtrai os campos especificos necessarios para a classe Professor
        vector<string> resultado = {
            getField("NomeServidor"),
            getField("Email"),
            getField("Orgao"),
            getField("RamalServidor"),
            getField("Cargo"),
            getField("LotacaoDescricaoCat")
        };


        return resultado;
        }
    }
    catch (exception& e) {
        cerr << "Erro ao parsear resposta do servidor: " << e.what() << endl;
        exit(1);
    }}


//METODO SETINFO

//Obtem informacoes do professor do sistema da UFV
//Faz requisicao para API interna da universidade e parseia resposta

Professor::Professor():
    departamento("None")
{}

void Professor::setInfo() {
try {
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *headers = NULL;
    string response;

    //Prepara nome para URL enconding: substitui espacos por +
    replace(nome.begin(), nome.end(), ' ', '+');

    //Constroi corpo de requisicao POST para consulta por nome
    string corpoReq = "funcao=consultarPorNome&nomeServidor=" + nome + "&cargo=";

    //Configura headers HTTP para simular requisicao de navegador
    headers = curl_slist_append(headers, "Host: www2.dti.ufv.br");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Platform: \"Linux\"");
    headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9");
    headers = curl_slist_append(headers, "Sec-Ch-Ua: \"Not_A Brand\";v=\"99\", \"Chromium\";v=\"142\"");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Mobile: ?0");
    headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    headers = curl_slist_append(headers, "Origin: https://www2.dti.ufv.br");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
    headers = curl_slist_append(headers, "Priority: u=1, i");

    //Inicializacao sessao cURL
    hnd = curl_easy_init();

    //Configuracoes de requisicao cURL
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, "https://www2.dti.ufv.br/consulta_catalogo_telefonico/scripts/main_db.php");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, corpoReq.c_str());
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)corpoReq.size());

    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_REFERER, "https://www2.dti.ufv.br/consulta_catalogo_telefonico/scripts/main.php");
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);

    //Callback para capturar resposta
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);
        
    //Executa a requisicao  
    ret = curl_easy_perform(hnd);
    
    //limpa recursos
    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);

    //parseia a resposta e extrai campos relevantes
    vector<string> campos = parseServidor(response);
    if (campos.empty()) {
        throw runtime_error("Resposta do servidor invalida ou campos nao encontrados.");
    }

    //atualiza atributos do objeto Professor com os dados obtidos
    this->nome = campos[0];
    this->emailInstitucional = campos[1];
    this->orgao = campos[2];
    this->telefone = campos[3];
    this->departamento = campos[5];}
    catch (exception& e) {
        cerr << "Erro ao obter informacoes do professor: " << e.what() << endl;
        exit(1);
    }
}

bool Professor::autenticar(string nome, string email) {
    Professor a;
    a.setNome(nome);
    a.setInfo();
    this->nome = nome;
    this->emailInstitucional = email;
    if (this->getEmail() != a.getEmail()) {
        return false;
    }
    this->setMatricula(this->getEmail());
    return true;
}

void limpar2() {
    system("clear");
}

void Professor::InteracaoUsuario() {
    //Implementacao do fluxo interativo para professor
    //autenticacao simplificada para professores (nome)
    try {setCookieValue();
    std::cout << "Cookie de sessão: " << getCookie()<< std::endl;
    string nome, mail;
    bool primeiraVez = true;
    while (true){
        if (!primeiraVez) {
            cout << "Informações erradas!\n";
        }
        else {
            primeiraVez = false;
        }
        limpar2();
        cout << "Digite seu Nome: ";
        getline(cin, nome);
        cout << "Digite o seu email: ";
        //cin.ignore();
        getline(cin, mail);
        bool autenticou = autenticar(nome, mail);
        if (autenticou) {
            setInfo(); //busca informacoes do professor
            break;
        }
    }

    //loop principal do menu p/ professores
    while(true){
        std::string escolha;
        cout<<"Escolha a funcão você deseja executar:\n1 - Pesquisa de livros\n2 - Visualizar perfil\n3 - Encerrar programa\nResposta: ";
        cin>>escolha;

        //OPCAO 1: PESQUISA DE LIVROS
        if(escolha=="1"){
            limpar2();
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin.ignore();
            //cin >> pesquisa;
            getline(cin, pesquisa);
            auto livros = buscarLivros(pesquisa);
            cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[0].getNome() << " \n| N.Chamada:\t"<< livros[0].getId() << "\n---------------------------------------\n";
            
            //opcao de se conectar ao servidor de chat
            cout <<"\n\nDeseja acessar o forum do livro?\nSe sim, digite 1,Caso contrario, digite qualquer outro número: ";
            cin>>escolha;
            cin.ignore();
            if(escolha=="1"){
                string address = "127.0.0.1";
                string port = "12345";
                Client cliente;
                string nome;
                cout << "Digite o nome do cliente: ";
                getline(cin, nome);
                cliente.connect_socket(address, port, nome, livros[0].getNome(), this->getDep(), livros[0].getId());
                cliente.run();
                cliente.close();
                }
        }

        //OPCAO 2: VISUALIZAR PERFIL PESSOAL
        if(escolha=="2"){
            limpar2();
            cout << "---------------------------------------\n| Nome:\t\t" << getNome() << "\n" << "| Email: \t" << getEmail() << "\n| Orgao:\t"<< getOrgao() << "\n| Departamento:\t"<< getDep() << "\n| Telefone:\t" << getTelefone() << "\n" << "\n---------------------------------------\n";
        }

        //OPCAO 3: ENCERRAR
        if(escolha=="3"){
            exit(0);
        }
    }
    }
    catch (exception& e) {
        cerr << "Erro na interação do usuário: " << e.what() << endl;
        exit(1);
    }
}
