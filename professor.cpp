#include "professor.h"
using namespace std;

string Professor::getDep() {
    return this->departamento;
}

string Professor::getOrgao() {
    return this->orgao;
}

string Professor::getTelefone() {
    return this->telefone;
}

string curl_setinfo = R"(curl --path-as-is -i -s -k -X $'POST' \
    -H $'Host: www2.dti.ufv.br' -H $'Content-Length: 50' -H $'Sec-Ch-Ua-Platform: \"Linux\"' -H $'Accept-Language: pt-BR,pt;q=0.9' -H $'Sec-Ch-Ua: \"Not_A Brand\";v=\"99\", \"Chromium\";v=\"142\"' -H $'Sec-Ch-Ua-Mobile: ?0' -H $'X-Requested-With: XMLHttpRequest' -H $'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36' -H $'Accept: */*' -H $'Content-Type: application/x-www-form-urlencoded; charset=UTF-8' -H $'Origin: https://www2.dti.ufv.br' -H $'Sec-Fetch-Site: same-origin' -H $'Sec-Fetch-Mode: cors' -H $'Sec-Fetch-Dest: empty' -H $'Referer: https://www2.dti.ufv.br/consulta_catalogo_telefonico/scripts/main.php' -H $'Accept-Encoding: gzip, deflate, br' -H $'Priority: u=1, i' \
    -b $'_ga=GA1.1.1246351.1761589832; _ga_3GKTCB3HHS=GS2.1.s1762993719$o2$g1$t1762993745$j34$l0$h0; PHPSESSID=buuogdhfu27cc69hub5h5k21g7; _pk_id.38.7238=6dfdff2021a86bb2.1764002811.; _pk_ses.38.7238=1' \
    --data-binary $'funcao=consultarPorNome&nomeServidor=salles&cargo=' \
    $'https://www2.dti.ufv.br/consulta_catalogo_telefonico/scripts/main_db.php')";

// retorna orgao, email e telefone (e voip (?))




