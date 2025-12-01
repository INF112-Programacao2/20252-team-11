#include "aluno.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <cstdlib>   
#include <stdexcept>
#include <iomanip>  
#include <algorithm> 
using namespace std;


static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}

static std::string getenv_safe(const char* name) {
    const char* v = std::getenv(name);
    return v ? std::string(v) : std::string();
}

string Aluno::getCurso() {
    return this->curso;
}

string Aluno::getAdmissao() {
    return this->admissao;
}

string Aluno::getSexo() {
    return this->sexo;
}

string Aluno::getSem() {
    return this->sem;
}

string curl_getinfo = R"(curl --path-as-is -i -s -k -X $'POST' \
    -H $'Host: www2.dti.ufv.br' -H $'Content-Length: 77' -H $'Sec-Ch-Ua-Platform: \"Linux\"' -H $'Accept-Language: pt-BR,pt;q=0.9' -H $'Sec-Ch-Ua: \"Not_A Brand\";v=\"99\", \"Chromium\";v=\"142\"' -H $'Sec-Ch-Ua-Mobile: ?0' -H $'X-Requested-With: XMLHttpRequest' -H $'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36' -H $'Accept: application/json, text/javascript, */*; q=0.01' -H $'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H $'Origin: https://www2.dti.ufv.br' -H $'Sec-Fetch-Site: same-origin' -H $'Sec-Fetch-Mode: cors' -H $'Sec-Fetch-Dest: empty' -H $'Referer: https://www2.dti.ufv.br/conacweb/scripts/consultaPublicaEstudantesComVinculo.php' -H $'Accept-Encoding: gzip, deflate, br' -H $'Priority: u=1, i' \
    -b $'_ga=GA1.1.1246351.1761589832; _ga_3GKTCB3HHS=GS2.1.s1762993719$o2$g1$t1762993745$j34$l0$h0; PHPSESSID=buuogdhfu27cc69hub5h5k21g7; _pk_id.38.7238=6dfdff2021a86bb2.1764002811.; _pk_ses.38.7238=1' \
    --data-binary $'Consultar=Consultar&campus=1&ano=&nomeFormando=Arthur+Mendes+Vilela&codCurso=' \
    $'https://www2.dti.ufv.br/conacweb/scripts/consultaPublicaEstudantesComVinculo.php')";

vector<string> split_sentence(string sen) {
    // Create a stringstream object
    stringstream ss(sen);
    
    string word;
    
    vector<string> words;
    vector<string> separado;
    string termo_comp;
   
    for (char letra : sen) {
        termo_comp += letra;
        if (letra=='"') {
            separado.push_back(termo_comp);
            termo_comp="";
        }
    }
    vector<string> novo = {separado[7], separado[11], separado[23], separado[15]};
    vector<string> novo2;
    
    for (string pal : novo){
        pal.erase(std::remove(pal.begin(), pal.end(), '"'), pal.end());
        novo2.push_back(pal);
    }
    return novo2;
}



string search_personal(string nome) {
CURLcode ret;
CURL *hnd;
struct curl_slist *headers = NULL;
string response;

replace(nome.begin(), nome.end(), ' ', '+');
cout << nome << "\n";

string corpoReq = "Consultar=Consultar&campus=1&ano=&nomeFormando=" 
                    + nome +
                  "&codCurso=";

headers = curl_slist_append(headers, "Host: www2.dti.ufv.br");
headers = curl_slist_append(headers, "Sec-Ch-Ua-Platform: \"Linux\"");
headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9");
headers = curl_slist_append(headers, "Sec-Ch-Ua: \"Not_A Brand\";v=\"99\", \"Chromium\";v=\"142\"");
headers = curl_slist_append(headers, "Sec-Ch-Ua-Mobile: ?0");
headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
headers = curl_slist_append(headers, "Accept: application/json, text/javascript, */*; q=0.01");
headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
headers = curl_slist_append(headers, "Origin: https://www2.dti.ufv.br");
headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
headers = curl_slist_append(headers, "Priority: u=1, i");
headers = curl_slist_append(headers, "Cookie: _ga=GA1.1.1246351.1761589832; _ga_3GKTCB3HHS=GS2.1.s1762993719$o2$g1$t1762993745$j34$l0$h0; PHPSESSID=buuogdhfu27cc69hub5h5k21g7; _pk_id.38.7238=6dfdff2021a86bb2.1764002811.; _pk_ses.38.7238=1");

hnd = curl_easy_init();
curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
curl_easy_setopt(hnd, CURLOPT_URL, "https://www2.dti.ufv.br/conacweb/scripts/consultaPublicaEstudantesComVinculo.php");
curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);

curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, corpoReq.c_str());
curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)corpoReq.size());

curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

curl_easy_setopt(hnd, CURLOPT_REFERER, "https://www2.dti.ufv.br/conacweb/scripts/consultaPublicaEstudantesComVinculo.php");
curl_easy_setopt(hnd, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36");
curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
curl_easy_setopt(hnd, CURLOPT_PATH_AS_IS, 1L);
curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

ret = curl_easy_perform(hnd);

vector<string> retorno = split_sentence(response);

curl_easy_cleanup(hnd);
curl_slist_free_all(headers);

return response;

}


void Aluno::setInfo() {       // feito
    CURLcode ret;
    CURL *hnd = curl_easy_init(); 
    
    // requisição
    struct curl_slist *headers;
    std::string html_body;
    std::string phpsessid = this->getCookie(); 
    std::string cookie_header_value = "PHPSESSID=" + phpsessid;

    if (!hnd) {
        std::cerr << "Erro: curl_easy_init() falhou." << std::endl;
        return; 
    }

    headers = NULL;
    headers = curl_slist_append(headers, "Host: pergamum.ufv.br");
    headers = curl_slist_append(headers, "Sec-Ch-Ua: \"Not_A Brand\";v=\"99\", \"Chromium\";v=\"142\"");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Mobile: ?0");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Platform: \"Linux\"");
    headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
    headers = curl_slist_append(headers, "Priority: u=0, i");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    
    curl_easy_setopt(hnd, CURLOPT_COOKIE, cookie_header_value.c_str()); 

    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_URL, "https://pergamum.ufv.br/biblioteca_s/meu_pergamum/dados_pessoais.php");
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(hnd, CURLOPT_REFERER, "https://pergamum.ufv.br/biblioteca_s/meu_pergamum/base.php");
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(hnd, CURLOPT_PATH_AS_IS, 1L);
    curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &html_body);

    ret = curl_easy_perform(hnd);

    if (ret != CURLE_OK) {
        std::cerr << "Erro na requisição cURL: " << curl_easy_strerror(ret) << std::endl;
        curl_easy_cleanup(hnd);
        curl_slist_free_all(headers);
        return; 
    }
    

    // extração de dados..
    std::regex cpf_regex("name=\"txtCPF\"[\\s\\S]*?value=\"([0-9]+)\"");
    std::smatch cpf_match;
    std::string CPF;
    std::string email;

    if (std::regex_search(html_body, cpf_match, cpf_regex) && cpf_match.size() > 1) {
        CPF = cpf_match[1].str();
    } else {
        std::cerr << "Atenção: CPF não encontrado. O HTML retornado foi provavelmente a página de login." << std::endl;
        CPF = "CPF Não Encontrado";
    }

    std::regex email_regex("name=\"txtEmail\"[\\s\\S]*?value=\"(.*?)\"");
    std::smatch email_match;

    if (std::regex_search(html_body, email_match, email_regex) && email_match.size() > 1) {
        email = email_match[1].str();
    } else {
        std::cerr << "Atenção: Email não encontrado. O HTML retornado foi provavelmente a página de login." << std::endl;
        email = "Email Não Encontrado";
    }

    this->emailInstitucional = email;
    this->CPF = CPF;

    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);

    vector<string> info_nova = split_sentence(search_personal(this->nome));
    this->curso = info_nova[0];
    this->admissao = info_nova[1];
    this->sexo = info_nova[2];
    this->sem = info_nova[3];

    //cout << this->curso << " " << this->admissao << " " << this->sexo << "\n";
}



    // retorna nome, curso e ano de admissão (semestre tbm)

/*[{"Nome":"Joao Pedro Silva Soares","Nome_Curso":"Administra\u00e7\u00e3o","Ano_admissao":"2025","Sem_admissao":"1","Titulo_Curso":"Bacharel em Administra\u00e7\u00e3o  
","Sexo":"M","Titulo_Curso_Feminino":"Bacharela em Administra\u00e7\u00e3o 
"},{"Nome":"Jo\u00e3o Pedro Silva Valente","Nome_Curso":"Administra\u00e7\u00e3o","Ano_admissao":"2023","Sem_admissao":"1","Titulo_Curso":"Bacharel em Administra\u00e7\u00e3o",
"Sexo":"M","Titulo_Curso_Feminino":"Bacharela em Administra\u00e7\u00e3o"}*/
//ex..




 