#include "professor.h"
#include <curl/curl.h>
#include <string>
#include <iostream>
using namespace std;


size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
static std::string getenv_safe(const char* name) {
    const char* v = std::getenv(name);
    return v ? std::string(v) : std::string();
}

string Professor::getDep() {
    return this->departamento;
}

string Professor::getOrgao() {
    return this->orgao;
}

string Professor::getTelefone() {
    return this->telefone;
}

// aux pra setInfo()
vector<string> parseServidor(string texto)
{
    for (char &c : texto)
        if (c == '\'') c = '"';

    size_t inicio = texto.find("{");
    size_t fim = texto.find("}");
    if (inicio == string::npos || fim == string::npos)
        return {};

    string obj = texto.substr(inicio, fim - inicio + 1);

    auto getField = [&](string key) -> string {
        string search = "\"" + key + "\":";
        size_t pos = obj.find(search);
        if (pos == string::npos) return "";

        pos += search.size();
        while (pos < obj.size() && (obj[pos] == ' ')) pos++;
        if (obj[pos] != '"') return "";

        pos++;

        size_t end = obj.find("\"", pos);
        if (end == string::npos) return "";

        return obj.substr(pos, end - pos);
    };

    vector<string> resultado = {
        getField("NomeServidor"),
        getField("Email"),
        getField("Orgao"),
        getField("RamalServidor"),
        getField("Cargo"),
        getField("LotacaoDescricaoCat")
    };

//    for (string u : resultado) cout << u << "\n";

    return resultado;
}

string personalInfo(string nome)
{
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *headers = NULL;
    string response;

    replace(nome.begin(), nome.end(), ' ', '+');
    //cout << nome << "\n";

    string corpoReq = "funcao=consultarPorNome&nomeServidor=" + nome + "&cargo=";

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

    hnd = curl_easy_init();
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

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

    ret = curl_easy_perform(hnd);

    //cout << "Resposta do servidor:\n" << response << "\n";

    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);
    return response;
}

void Professor::setInfo() {

    CURLcode ret;
    CURL *hnd;
    struct curl_slist *headers = NULL;
    string response;

    replace(nome.begin(), nome.end(), ' ', '+');
    //cout << nome << "\n";

    string corpoReq = "funcao=consultarPorNome&nomeServidor=" + nome + "&cargo=";

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

    hnd = curl_easy_init();
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

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

    ret = curl_easy_perform(hnd);

    //cout << "Resposta do servidor:\n" << response << "\n";

    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);
    vector<string> campos = parseServidor(response);

    this->nome = campos[0];
    this->emailInstitucional = campos[1];
    this->orgao = campos[2];
    this->telefone = campos[3];
    this->departamento = campos[5];
}



