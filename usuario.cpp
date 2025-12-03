#include "usuario.h"
#include <codecvt>
#include <locale>
using namespace std;


// funções auxiliares

// converter byte-type
std::string iso_8859_1_to_utf8(const std::string &latin1) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv_utf8;
    std::wstring wide;

    for (unsigned char c : latin1) {
        wide += static_cast<wchar_t>(c);
    }

    return conv_utf8.to_bytes(wide);
}

// gravar dados das requisições
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}

// setar cookies de ambiente 
static std::string getenv_safe(const char* name) {
    const char* v = std::getenv(name);
    return v ? std::string(v) : std::string();
}

// limpar html
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return ""; 
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

// gets
Usuario::Usuario(){
    this->setCookieValue();
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

vector<string> Usuario::getInteresses() {
    return this->interesses;
}

string Usuario::getCookie() {
    return this->cookie_value;
}

string Usuario::getEmail() {
    return this->emailInstitucional;
}



void Usuario::addAmigo(Usuario& amigo) {        // talvez implementar para banco de dados depois..
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

bool Usuario::autenticar(std::string matricula, std::string senha)
{
    std::string phpsessid = this->getCookie();
    if (phpsessid.empty()) {
        throw std::runtime_error("No Cookie Value (PHPSESSID) value provided, should use Usuario.setCookieValue() first.");
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

    if (nomes.size() > 1) {     // seta o nome do Usuario
        this->nome = iso_8859_1_to_utf8(nomes[1]); 
    }

//    std::cout << "Nomes extraidos (" << nomes.size() << "):\n";
//   for (size_t i = 0; i < nomes.size(); ++i) {
//        std::cout << " [" << i << "] " << nomes[i] << "\n";
//    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    //std::cout << nomes[1];
    bool autenticou=true;
    string chave_er = "Edi";

    return (!(nomes[1].find(chave_er)!=string::npos));
}

bool Usuario::autenticar(std::string nome) {
    this->nome = nome;
    return true;
}

std::vector<Usuario::Livro> Usuario::buscarLivros(std::string _nome) { 
    CURL *curl = curl_easy_init();
    std::vector<Livro> resultados;
    std::string phpsessid = this->getCookie();

    if (!curl) return resultados;

    std::string response;
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Host: www.pergamum.ufv.br");
    headers = curl_slist_append(headers, "sec-ch-ua-platform: \"Windows\"");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/139.0.0.0 Safari/537.36");
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Origin: https://www.pergamum.ufv.br");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
    headers = curl_slist_append(headers, "Referer: https://www.pergamum.ufv.br/biblioteca/index.php");
    headers = curl_slist_append(headers, "Accept-Language: pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    std::string cookie =
        "_ga=GA1.1.2037255739.1755903321; "
        "_ga_3GKTCB3HHS=GS2.1.s1755903320;"
        "_ga_V5B0KG5WW5=GS2.1.s1756825688;"
        "_ga_KLR8GVHKBC=GS2.1.s1756844283;"
        "_ga_7H6KMW913P=GS2.1.s1756847215;"
        "PHPSESSID=" + phpsessid;
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());

    char *escaped_nome_ptr = curl_easy_escape(curl, _nome.c_str(), _nome.length());
    if (!escaped_nome_ptr) {
        std::cerr << "Erro ao fazer URL-encode do nome." << std::endl;
        curl_easy_cleanup(curl);
        return resultados;
    }
    std::string escaped_nome(escaped_nome_ptr);
    curl_free(escaped_nome_ptr); 

    std::string data = 
        "rs=ajax_resultados&rst=&rsrnd=1756848921468&rsargs[]=20&rsargs[]=0&rsargs[]=1&rsargs[]=" + escaped_nome +
        "&rsargs[]=&rsargs[]=%2C&rsargs[]=indice&rsargs[]=&rsargs[]=&rsargs[]=&rsargs[]=&rsargs[]=1&rsargs[]=&rsargs[]=&rsargs[]=obra&rsargs[]=68b738dbdeeaf&rsargs[]=&rsargs[]=&rsargs[]=&rsargs[]=";

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.pergamum.ufv.br/biblioteca/index.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
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
        response_clean = std::regex_replace(response_clean, std::regex("\\s+"), " ");
        
        std::regex titulo_regex("<a[^>]*class=['\"]?link_azul['\"]?[^>]*>(.*?)</a>", std::regex::icase);
        std::regex numero_regex("<a[^>]*class=['\"]?link_reserva['\"]?[^>]*>(.*?)</a>", std::regex::icase);

        auto titulos_begin = std::sregex_iterator(response_clean.begin(), response_clean.end(), titulo_regex);
        auto titulos_end = std::sregex_iterator();


        for (auto it = titulos_begin; it != titulos_end; ++it) {
            std::smatch title_match = *it;
            Livro livro;
            livro.nome = std::regex_replace(title_match[1].str(), std::regex("\\s+"), " ");

            auto start_search_it = title_match.suffix().first;
            
            std::smatch match_num;
            if (std::regex_search(start_search_it, response_clean.cend(), match_num, numero_regex)) {
                livro.numero_chamada = std::regex_replace(match_num[1].str(), std::regex("\\s+"), " ");
            } else {
                livro.numero_chamada = "(sem número)";
            }
            livro.nome = iso_8859_1_to_utf8(livro.nome);
            if (livro.numero_chamada != "Reserva" && livro.nome != "Esta página" && livro.nome != "Todos" && livro.numero_chamada != "(sem número)" && livro.nome != "Referência" && livro.nome != "Marc")
                resultados.push_back(livro);
        }
    } else {
        std::cerr << "Erro na requisição: " << curl_easy_strerror(res) << std::endl;
    }

    //cout << response << endl;

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (resultados.size() == 0) {
        throw std::length_error("No results for the search!\n");
    }
    return resultados;
}


void Usuario::setInfo() {
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

    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);
}

void Usuario::setNome(std::string nome){
    this->nome = nome;
}

void Usuario::setMatricula(std::string matricula){
    this->matricula = matricula;
}

void Usuario::setForum(std::string forum){
    this->forum = forum;
}
