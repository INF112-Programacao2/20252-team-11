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

//======================================
//FUNCOES AUXILIARES
//=====================================

//Callback para escrita de dados do cURL
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}

//funcao para ler variaveis de ambiente
static std::string getenv_safe(const char* name) {
    const char* v = std::getenv(name);
    return v ? std::string(v) : std::string();
}

//remove espacos em branco do inicio e fim
std::string trim2(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return ""; 
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

//conversao de ISO-8858-1 para UTF-8
std::string iso_8859_1_to_utf8_2(const std::string &latin1) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv_utf8;
    std::wstring wide;

    for (unsigned char c : latin1) {
        wide += static_cast<wchar_t>(c);
    }

    return conv_utf8.to_bytes(wide);
}

//======================================
//GETS
//=====================================

//Retornam seus respectivos valores. Ex: getCurso retorna curso, getSexo retorna sexo, e assim sucessivamente
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

//======================================
//FUNCAO DE PARSING DE JSON PARA DADOS PESSOAIS
//=====================================

//analisa resposta JSON da API de estudantes da UFV
//extrai informacoes academicas do aluno
vector<string> parse_pessoa_json(const std::string &response) {
    auto j = json::parse(response); //parse do JSON
    vector<string> p;
    
    if (j.is_array() && !j.empty()) { //verifica se eh array nao vazio
        auto &obj = j[0];    //pega primeiro objeto (assume unico resultado)

        //extrai campos especificos com valores default
        p.push_back(obj.value("Nome_Curso", ""));
        p.push_back(obj.value("Ano_admissao", ""));
        p.push_back(obj.value("Sem_admissao", ""));
        p.push_back(obj.value("Sexo", ""));
    }

    return p;
}

//======================================
//BUSCA DE DADOS PESSOAIS NA API UFV
//=====================================

bool Aluno::autenticar(std::string matricula, std::string senha)
{
    //Verifica se ha cookie de sessao disponivel
    std::string phpsessid = this->getCookie();
    if (phpsessid.empty()) {
        throw std::runtime_error("No Cookie Value (PHPSESSID) value provided, should use Usuario.setCookieValue() first.");
    }

    //Constroi cookie header com variaveis de ambiente para analytics
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

    //Inicializacao do cURL
    CURL* curl = nullptr;
    CURLcode res;
    std::string response;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        throw std::runtime_error("curl_easy_init failed");
    }

    // Headers HTTP detalhados para simular navegador 
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

    //Escapa caracteres especiais para URL encoding
    char* esc_matricula = curl_easy_escape(curl, matricula.c_str(), (int)matricula.size());
    char* esc_senha     = curl_easy_escape(curl, senha.c_str(), (int)senha.size());

    //Constroi corpo de requisicao POST com credenciais
    std::ostringstream post_ss;
    post_ss << "flag=index.php"
            << "&login=" << (esc_matricula ? esc_matricula : "")
            << "&password=" << (esc_senha ? esc_senha : "")
            << "&button=Acessar"
            << "&numero_mestre=&ifsp_categ=&lab_com_solicitacao=";

    std::string post_fields = post_ss.str();

    if (esc_matricula) curl_free(esc_matricula);
    if (esc_senha) curl_free(esc_senha);

    //Configuracoes da requisicao cURL
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

    //Executa a requisicao
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::string err = curl_easy_strerror(res);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        throw std::runtime_error(std::string("curl_easy_perform failed: ") + err);
    }
    
    //Extrai o nome do usuario da resposta HTML usando regex
    //Procura por tags que geralmente contem o mome no pergamum
    std::regex rgx("<strong>(.*?)</strong>", std::regex::icase);
    std::sregex_iterator it(response.begin(), response.end(), rgx);
    std::sregex_iterator end;
    std::vector<std::string> nomes;
    for (; it != end; ++it) {
        nomes.push_back((*it)[1].str());
    }

    //Se encontrou pelo menos 2 tags strong, assume que o segundo eh o nome
    if (nomes.size() > 1) {     // seta o nome do Usuario
        this->nome = iso_8859_1_to_utf8_2(nomes[1]);  //Converte de latin1 para UTF-8
    }    

//    std::cout << "Nomes extraidos (" << nomes.size() << "):\n";
//   for (size_t i = 0; i < nomes.size(); ++i) {
//        std::cout << " [" << i << "] " << nomes[i] << "\n";
//    }

    //Limpeza dos recursos cURL
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    //Logica de autenticacao = verifica se a resposta contem "Edi"
    bool autenticou=true;
    string chave_er = "Edi";

    // retorna true se NAO encontrar "Edi" no nome (indicando sucesso)
    return (!(nomes[1].find(chave_er)!=string::npos));
}


//Consulta API publica de estudantes da ufv para obter informacoes academicas
string search_personal(string nome) {
    CURLcode ret;
    CURL *hnd;
    struct curl_slist *headers = NULL;
    string response;

    //prepara para URL enconding (substitui espacos por +)
    replace(nome.begin(), nome.end(), ' ', '+');

    //constroi corpo da requisicao POST
    string corpoReq = "Consultar=Consultar&campus=1&ano=&nomeFormando=" 
                        + nome +
                    "&codCurso=";

    //configura headers HTTP para simular navegador
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

    //inicializa e configura cURL
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
    
    //Callback para recuperar resposta
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &response);

    //executa requisicao
    ret = curl_easy_perform(hnd);

    //limpeza de recursos
    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);

    return response;    //retorna resposta JSON
}


//======================================
//IMPLEMENTACAO DO METODO SETINFO
//=====================================

//Override do metodo virtual da classe base
//Obtem informacoes pessoais e academicas do aluno
void Aluno::setInfo() {   

    //Obtem informacoes do pergamum (herdada de usuario)
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

    //configura headers para simular navegador
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

    //configuracoes da requisicao cURL
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
    
    //Extrai cpf e email do html do pergamum
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

    //limpeza recursos
    curl_easy_cleanup(hnd);
    curl_slist_free_all(headers);

    //Obtem informacoes academicas da API UV 
    vector<string> info_nova = parse_pessoa_json(search_personal(this->nome));

    //dados
    this->curso = info_nova[0];
    this->admissao = info_nova[1];
    this->sexo = info_nova[3];
    this->sem = info_nova[2];
}

//======================================
//IMPLEMENTACAO DO METODO SEARCHDEBITO
//=====================================

//busca debitos/multas do aluno no sistema pergamum
std::vector<Usuario::Debito> Aluno::searchDebito() {
    CURL *curl = curl_easy_init();
    std::vector<Debito> resultados;
    std::string phpsessid = this->getCookie();

    if (!curl) return resultados;

    std::string response;
    struct curl_slist *headers = NULL;

    //configura headers para requisicao AJAX
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
    
    //parametros fixos 06/09/2024 a 06/09/2025
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

        //limpeza e processamento da resposta HTML
        std::string response_clean = response;
        response_clean = std::regex_replace(response_clean, std::regex("\\\\r\\\\n"), " ");  //remove newlines
        response_clean = std::regex_replace(response_clean, std::regex("\\\\'"), "'");       //remove escapes
        response_clean = std::regex_replace(response_clean, std::regex("&nbsp;"), " ");      //substitui espacos HTML

        //regex para extrair nomes dos livros
        std::regex nome_regex("class='box_azul_left'>(.*?)</td>", std::regex::icase);

        //regex para extrair valores das multas
        std::regex multa_regex("class='box_magenta_c'>(.*?)</td>", std::regex::icase);

        //regex para remover tags HTML
        std::regex tag_re("<.*?>"); 

        //extrai nomes
        std::vector<std::string> nomes_sujos;
        auto nomes_begin = std::sregex_iterator(response_clean.begin(), response_clean.end(), nome_regex);
        auto nomes_end = std::sregex_iterator();
        for (auto it = nomes_begin; it != nomes_end; ++it) {
            nomes_sujos.push_back((*it)[1].str());
        }

        //extrai valores das multas
        std::vector<std::string> multas;
        auto multas_begin = std::sregex_iterator(response_clean.begin(), response_clean.end(), multa_regex);
        auto multas_end = std::sregex_iterator();
        for (auto it = multas_begin; it != multas_end; ++it) {
            multas.push_back((*it)[1].str());
        }

        //remove cabecalhos das tabelas (primeiro elemento de cada vetor)
        if (!nomes_sujos.empty()) {
            nomes_sujos.erase(nomes_sujos.begin());  //remove "nome do material"
        }
        if (!multas.empty()) {
            multas.erase(multas.begin());         //remove valor do material R$
        }

        //processa nomes - remove tags, limpa espacos, converte enconding
        for (const auto& nome_sujo : nomes_sujos) {
            std::string nome_sem_tags = std::regex_replace(nome_sujo, tag_re, "");
            std::string nome_limpo = trim2(nome_sem_tags);
            nome_limpo = iso_8859_1_to_utf8_2(nome_limpo);
            resultados.push_back({nome_limpo, "0"});        //valor inicial "0" 
        }

        //associa multa aos nomes correspondentes
        size_t counter = 0;
        for (const auto& preco : multas) {
            if (counter < resultados.size()) {
                resultados[counter].debt = trim2(preco);  //atualiza valor
            }
            counter++;
        }    

        //calcula total dos debitos
        double total_debt = 0.0;
        std::regex remove_prefix("R\\$\\s*");  //remove R$ dos valores

        for (const auto& d : resultados) {
            std::string numeric_str = d.debt;
            
            numeric_str = std::regex_replace(numeric_str, remove_prefix, ""); 
            numeric_str = trim2(numeric_str); 
            std::replace(numeric_str.begin(), numeric_str.end(), ',', '.');     //converte para double

            try {
                total_debt += std::stod(numeric_str); //soma ao total
            } catch (const std::exception& e) {
            }
        }

        //formata total no padrao brasileiro R$ X,XX
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << total_debt;
        std::string total_str = ss.str();
        
        std::replace(total_str.begin(), total_str.end(), '.', ','); //troca . por ,
        total_str = "R$ " + total_str;    //adiciona "R$ "

        //insere linha de total no inicio do vetor
        resultados.insert(resultados.begin(), {"Total", total_str});
   
    } else {
        std::cerr << "Erro na requisição: " << curl_easy_strerror(res) << std::endl;
    }

    //limpeza de recursos
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return resultados;    //retorna vetor de debitos
}

void limpar() {
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void Aluno :: InteracaoUsuario(){
    //configuracao inicial parecida com usuario
    setCookieValue();
    std::cout << "Cookie de sessão: " << getCookie()<< std::endl;
    string matricula, senha;
    while (true){    
        cout << "Digite sua matricula: ";
        cin >> matricula;
        cout << "Digite a sua senha da BBT: ";
        cin >> senha;

        bool autenticou = autenticar(matricula, senha);

        if (autenticou) {
            setInfo();    //obtem informacoes especificas de aluno
            break;
        }
        else 
            cout << "Matrícula ou senha inválida!\n";
    }


    //Loop principal do menu para alunos
    while(true){  
        std::string escolha;
        cout<<"Escolha a função que você deseja executar:\n1 - Pesquisa de livros\n2 - Consultar o débito\n3 - Visualizar perfil\n4 - Encerrar programa\nResposta: ";
        cin>>escolha;
        limpar();

        //OPCAO 1: PESQUISA DE LIVROS(com opcao de acessar o forum)
        if (escolha=="1"){
            cout << "\n\n-------------\nPESQUISA DE LIVROS:\n-------------\nDigite os termos para a pesquisa: ";
            string pesquisa;
            cin >> pesquisa;
            try {
                cin.ignore();
                //cin >> pesquisa;
                getline(cin, pesquisa);
                limpar();
                try {
                    auto livros = buscarLivros(pesquisa);                
                    cout << "Primeiro resultado:\n---------------------------------------\n| Nome:\t\t" << livros[0].getNome() << " \n| N.Chamada:\t"<< livros[0].getId() << "\n---------------------------------------\n";
                    cout <<"\n\nDeseja acessar o forum do livro?\nSe sim, digite 1,Caso contrario, digite qualquer outro número: ";
                    cin>>escolha;
                    cin.ignore();
                    //opcao de se conectar ao servidor de chat
                    if(escolha=="1"){
                        string address = "127.0.0.1";
                        string port = "12345";
                        Client cliente;
                        string nome;
                        cout << "Digite o nome do cliente: ";
                        getline(cin, nome);

                        //conecta ao servidor de chat
                        cliente.connect_socket(address, port, nome, livros[0].getNome(), matricula, livros[0].getId());
                        cliente.run();
                        cliente.close();
                    }
                }
                catch (exception& e) {
                    cerr << e.what() << endl;
                }
                            
            }
            catch (exception&  e) {
                cerr << e.what() << endl;;
            }
        }

        //OPCAO 2: CONSULTA DE DEBITOS
        if(escolha=="2"){
            //busca debitos do aluno no sistema
            limpar();
            auto debitos = searchDebito();

            cout << "\n\nDÉBITOS DO USUÁRIO:\nTotal de debitos no momento: " << debitos[0].debt << "\n\nHISTÓRICO DE DÉBITOS:\n---------------------------------------\n";

            //lista todos os debitos
            bool counter = false;
            for (Usuario::Debito debito : debitos) {
                if (!counter) {counter=true;}
                else cout << "| Debt:\t\t" << debito.nome << "\n| Valor:\t\t" << debito.debt << "\n";
            }
            cout << "---------------------------------------\n";
        }
        //OPCAO 3: VISUALIZAR PERFIL COMPLETO
        if(escolha=="3"){
            //exibe todas as informacoes do aluno
            cout << "---------------------------------------\n| Nome:\t\t" << getNome() << "\n" << "| Email: \t" << getEmail() <<"\n| CPF:\t\t" << getCPF() << "\n| Curso:\t"<< getCurso() << "\n| Admissão:\t"<< getAdmissao() << "/" << getSem() << "\n| Sexo:\t\t" << getSexo() << "\n---------------------------------------\n";

        }
        //OPCAO 4: ENCERRAR PROGRAMA
        if (escolha=="4"){
            exit(0);
        }

    }
}





 
