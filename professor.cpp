#include "professor.h"

//Inclusoes para requisicoes HTTP com cURL e manipulacao de strings
#include <curl/curl.h>
#include <string>
#include <iostream>
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

bool Professor::autenticar(std::string nome, std::string senha) {
    this->nome = nome;
    this->setInfo();
    if (this->getDep() == senha)
        return true;
    else return false;
}

//METODO SETINFO

//Obtem informacoes do professor do sistema da UFV
//Faz requisicao para API interna da universidade e parseia resposta
void Professor::setInfo() {

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

    //atualiza atributos do objeto Professor com os dados obtidos
    this->nome = campos[0];
    this->emailInstitucional = campos[1];
    this->orgao = campos[2];
    this->telefone = campos[3];
    this->departamento = campos[5];
}

