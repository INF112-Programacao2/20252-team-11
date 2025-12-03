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
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


// aux pra curl
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

// limpar html
std::string trim2(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return ""; 
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

std::string iso_8859_1_to_utf8_2(const std::string &latin1) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv_utf8;
    std::wstring wide;

    for (unsigned char c : latin1) {
        wide += static_cast<wchar_t>(c);
    }

    return conv_utf8.to_bytes(wide);
}

// gets
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

string Aluno::getCPF() {
    return this->CPF;
}

vector<string> parse_pessoa_json(const std::string &response) {
    auto j = json::parse(response);
    vector<string> p;
    if (j.is_array() && !j.empty()) {
        auto &obj = j[0];
        //p.push_back(obj.value("Nome", ""));
        p.push_back(obj.value("Nome_Curso", ""));
        p.push_back(obj.value("Ano_admissao", ""));
        p.push_back(obj.value("Sem_admissao", ""));
        p.push_back(obj.value("Sexo", ""));
    }

    return p;
}

string search_personal(string nome) {
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *headers = NULL;
    string response;

    replace(nome.begin(), nome.end(), ' ', '+');
    //cout << nome << "\n";

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

    vector<string> info_nova = parse_pessoa_json(search_personal(this->nome));
    this->curso = info_nova[0];
    this->admissao = info_nova[1];
    this->sexo = info_nova[3];
    this->sem = info_nova[2];

    //cout << this->curso << " " << this->admissao << " " << this->sexo << "\n";
}

std::vector<Usuario::Debito> Aluno::searchDebito() {
    CURL *curl = curl_easy_init();
    std::vector<Debito> resultados;
    std::string phpsessid = this->getCookie();

    if (!curl) return resultados;

    std::string response;
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Host: pergamum.ufv.br");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/139.0.0.0 Safari/537.36 Edg/139.0.0.0");
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Origin: https://pergamum.ufv.br");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
    headers = curl_slist_append(headers, "Referer: https://pergamum.ufv.br/biblioteca_s/meu_pergamum/emp_debito.php");
    headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    std::string cookie = "PHPSESSID=" + phpsessid;
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());

    std::string data = "rs=ajax_mostra_tabela&rst=&rsrnd=1757165872989&rsargs[]=06/09/2024&rsargs[]=06/09/2025";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://pergamum.ufv.br/biblioteca_s/meu_pergamum/emp_debito.php");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        std::string response_clean = response;
        response_clean = std::regex_replace(response_clean, std::regex("\\\\r\\\\n"), " "); 
        response_clean = std::regex_replace(response_clean, std::regex("\\\\'"), "'");    
        response_clean = std::regex_replace(response_clean, std::regex("&nbsp;"), " ");

        std::regex nome_regex("class='box_azul_left'>(.*?)</td>", std::regex::icase);
        std::regex multa_regex("class='box_magenta_c'>(.*?)</td>", std::regex::icase);
        std::regex tag_re("<.*?>"); 

        std::vector<std::string> nomes_sujos;
        auto nomes_begin = std::sregex_iterator(response_clean.begin(), response_clean.end(), nome_regex);
        auto nomes_end = std::sregex_iterator();
        for (auto it = nomes_begin; it != nomes_end; ++it) {
            nomes_sujos.push_back((*it)[1].str());
        }

        std::vector<std::string> multas;
        auto multas_begin = std::sregex_iterator(response_clean.begin(), response_clean.end(), multa_regex);
        auto multas_end = std::sregex_iterator();
        for (auto it = multas_begin; it != multas_end; ++it) {
            multas.push_back((*it)[1].str());
        }

        if (!nomes_sujos.empty()) {
            nomes_sujos.erase(nomes_sujos.begin());
        }
        if (!multas.empty()) {
            multas.erase(multas.begin());
        }

        for (const auto& nome_sujo : nomes_sujos) {
            std::string nome_sem_tags = std::regex_replace(nome_sujo, tag_re, "");
            std::string nome_limpo = trim2(nome_sem_tags);
            nome_limpo = iso_8859_1_to_utf8_2(nome_limpo);
            resultados.push_back({nome_limpo, "0"});
        }

        size_t counter = 0;
        for (const auto& preco : multas) {
            if (counter < resultados.size()) {
                resultados[counter].debt = trim2(preco); 
            }
            counter++;
        }

        double total_debt = 0.0;
        std::regex remove_prefix("R\\$\\s*"); 

        for (const auto& d : resultados) {
            std::string numeric_str = d.debt;
            
            numeric_str = std::regex_replace(numeric_str, remove_prefix, "");
            numeric_str = trim2(numeric_str); 
            std::replace(numeric_str.begin(), numeric_str.end(), ',', '.');

            try {
                total_debt += std::stod(numeric_str);
            } catch (const std::exception& e) {
            }
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << total_debt;
        std::string total_str = ss.str();
        
        std::replace(total_str.begin(), total_str.end(), '.', ',');
        total_str = "R$ " + total_str;

        resultados.insert(resultados.begin(), {"Total", total_str});
   
    } else {
        std::cerr << "Erro na requisição: " << curl_easy_strerror(res) << std::endl;
    }

    
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return resultados;
}




 