#include "usuario.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <cstdlib>   
#include <stdexcept>
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



string Usuario::getNome() {
    return this->nome;
}

float Usuario::getDebito() {
    return this->debito;
}

string Usuario::getMatricula() {
    return this->matricula;
}

string* Usuario::getInteresses() {
    return this->interesses;
}

string Usuario::getCookie() {
    return this->cookie_value;
}



void Usuario::addAmigo(Usuario& amigo) {        // talvez implementar para banco de dados depois..      (ou uma função de salvar que copia os dados de Usuário para o db..)
    this->n_amigos;
    Usuario* amigos_swap = new Usuario[this->n_amigos+1];
    for (int i=0;i<this->n_amigos;i++) amigos_swap[i] = this->amigos[i];
    amigos_swap[this->n_amigos+1] = amigo;
    delete[] this->amigos;
    this->amigos = amigos_swap;
}

std::string Usuario::setCookieValue() {
    CURL *curl;
    CURLcode res;

    std::string cookie_value;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Host: pergamum.ufv.br");
        headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
        headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/139.0.0.0 Safari/537.36");
        headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9,en;q=0.8");

        curl_easy_setopt(curl, CURLOPT_URL, "https://pergamum.ufv.br/biblioteca/index.php");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void*, size_t size, size_t nmemb, void*) {
            return size * nmemb;
        });

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            struct curl_slist *cookies;
            struct curl_slist *nc;
            res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

            if (res == CURLE_OK && cookies) {
                for (nc = cookies; nc; nc = nc->next) {
                    std::string line = nc->data;

                    if (line.find("PHPSESSID") != std::string::npos) {
                        std::stringstream ss(line);
                        std::string field;
                        std::vector<std::string> parts;

                        while (std::getline(ss, field, '\t')) {
                            parts.push_back(field);
                        }

                        if (!parts.empty()) {
                            //std::cout << parts.back() << std::endl; 
                            cookie_value = parts.back();
                        }
                    }
                }
                curl_slist_free_all(cookies);
            } else {
                std::cerr << "Nenhum cookie encontrado.\n";
            }
        } else {
            std::cerr << "Erro na requisição: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    this->cookie_value = cookie_value;
    return cookie_value;
}

bool Usuario::autenticar(const std::string matricula,
                           const std::string senha)
{
    std::string phpsessid = this->getCookie();
    if (phpsessid.empty()) {
        throw std::runtime_error("No PHPSESSID value provided, should use Session()._create_session first.");
    }

    std::ostringstream cookie_ss;
    cookie_ss << "_ga=GA1.1.2037255739.1755903321; ";
    cookie_ss << "_ga_3GKTCB3HHS=GS2.1.s1755903320" << getenv_safe("o1").c_str() << getenv_safe("g0").c_str()
              << getenv_safe("t1755903320").c_str() << getenv_safe("j60").c_str() << getenv_safe("l0").c_str()
              << getenv_safe("h0").c_str() << "; ";
    cookie_ss << "_ga_V5B0KG5WW5=GS2.1.s1756825688" << getenv_safe("o1").c_str() << getenv_safe("g1").c_str()
              << getenv_safe("t1756825845").c_str() << getenv_safe("j60").c_str() << getenv_safe("l0").c_str()
              << getenv_safe("h0").c_str() << "; ";
    cookie_ss << "PHPSESSID=" << phpsessid << "; ";
    cookie_ss << "_ga_KLR8GVHKBC=GS2.1.s1756844283" << getenv_safe("o1").c_str() << getenv_safe("g1").c_str()
              << getenv_safe("t1756844343").c_str() << getenv_safe("j60").c_str() << getenv_safe("l0").c_str()
              << getenv_safe("h0").c_str() << "; ";
    cookie_ss << "_ga_7H6KMW913P=GS2.1.s1756847215" << getenv_safe("o2").c_str() << getenv_safe("g1").c_str()
              << getenv_safe("t1756848763").c_str() << getenv_safe("j60").c_str() << getenv_safe("l0").c_str()
              << getenv_safe("h0").c_str() << ";";

    std::string cookie_header_value = cookie_ss.str();

    CURL* curl = nullptr;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        throw std::runtime_error("curl_easy_init failed");
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Host: pergamum.ufv.br");
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, "sec-ch-ua: \"Not;A=Brand\";v=\"99\", \"Google Chrome\";v=\"139\", \"Chromium\";v=\"139\"");
    headers = curl_slist_append(headers, "sec-ch-ua-mobile: ?0");
    headers = curl_slist_append(headers, "sec-ch-ua-platform: \"Windows\"");
    headers = curl_slist_append(headers, "Origin: https://pergamum.ufv.br");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/139.0.0.0 Safari/537.36");
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Referer: https://pergamum.ufv.br/biblioteca_s/php/login_usu.php?flag=index.php");
    headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    char* esc_matricula = curl_easy_escape(curl, matricula.c_str(), (int)matricula.size());
    char* esc_senha     = curl_easy_escape(curl, senha.c_str(), (int)senha.size());

    std::ostringstream post_ss;
    post_ss << "flag=index.php"
            << "&login=" << (esc_matricula ? esc_matricula : "")
            << "&password=" << (esc_senha ? esc_senha : "")
            << "&button=Acessar"
            << "&numero_mestre=&ifsp_categ=&lab_com_solicitacao=";

    std::string post_fields = post_ss.str();

    if (esc_matricula) curl_free(esc_matricula);
    if (esc_senha) curl_free(esc_senha);

    curl_easy_setopt(curl, CURLOPT_URL, "https://pergamum.ufv.br/biblioteca_s/php/login_usu.php");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)post_fields.size());
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_header_value.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::string err = curl_easy_strerror(res);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        throw std::runtime_error(std::string("curl_easy_perform failed: ") + err);
    }

    std::regex rgx("<strong>(.*?)</strong>", std::regex::icase);
    std::sregex_iterator it(response.begin(), response.end(), rgx);
    std::sregex_iterator end;
    std::vector<std::string> nomes;
    for (; it != end; ++it) {
        nomes.push_back((*it)[1].str());
    }

    if (nomes.size() > 1) {
        this->nome = nomes[1]; 
    }

//    std::cout << "Nomes extraidos (" << nomes.size() << "):\n";
//   for (size_t i = 0; i < nomes.size(); ++i) {
//        std::cout << " [" << i << "] " << nomes[i] << "\n";
//    }

    // cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    //std::cout << nomes[1];
    bool autenticou=true;
    string chave_er = "Edi";

    return (!(nomes[1].find(chave_er)!=string::npos));
}
