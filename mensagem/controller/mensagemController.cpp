#include "mensagemController.hpp"
#include "database.hpp"
#include <iomanip>

MensagemController::MensagemController(int idUsuario) : idUsuario(idUsuario){
    std::string query = "SELECT * FROM mensagem WHERE idUsuario = " + std::to_string(idUsuario) + ";";
    std::vector<std::vector<std::string>> resposta = Database::executarQuery(query.c_str());
    for(std::vector<std::string> linha : resposta){
        int id = -1;
        if(linha[0] != "NULL")
            id = std::stoi(linha[0]);
        std::string conteudo = "";
        if(linha[1] != "NULL")
            conteudo = linha[1];
        std::time_t dataHora = std::time(nullptr);
        if (linha[2] != "NULL") {
            struct std::tm tm{};
            std::istringstream ss(linha[2]);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            if (!ss.fail())
                dataHora = std::mktime(&tm);
        }
        int idAutor = -1;
        if(linha[3] != "NULL")
            idAutor = std::stoi(linha[3]);
        //idResposta
        if (id >= this->mensagens.size())
            this->mensagens.resize(id + 1); 
        this->mensagens[id] = Mensagem(conteudo, dataHora, idAutor);
    }
}

MensagemController::~MensagemController(){}

void MensagemController::setMensagem(std::string conteudo){
    
    //verificar se conteudo nao tem o caractere ' e normalizar

    std::string query = "INSERT INTO mensagem (conteudo, idUsuario) VALUES ('" + conteudo + "', " + std::to_string(this->idUsuario) + ");";
    Database::executarQuery(query.c_str());
    int id = Database::getLatest();
    if (id >= this->mensagens.size())
            this->mensagens.resize(id + 1);
    this->mensagens[id] = Mensagem(conteudo, std::time(nullptr), this->idUsuario);
}

void MensagemController::putMensagem(std::string conteudo, int id){
    
    //verificar se conteudo nao tem o caractere ' e normalizar

    std::string query = "UPDATE mensagem SET conteudo = '" + conteudo + "' WHERE id = " + std::to_string(id) + ";";
    Database::executarQuery(query.c_str());
    if (id >= this->mensagens.size())
        this->mensagens.resize(id + 1);
    this->mensagens[id].setConteudo(conteudo);
}

void MensagemController::deleteMensagem(int id){
    std::string query = "DELETE FROM mensagem WHERE id = " + std::to_string(id) + ";";
    Database::executarQuery(query.c_str());
    this->mensagens.erase(this->mensagens.begin() + id);
}

std::vector<Mensagem> MensagemController::getMensagens(){
    return this->mensagens;
}

Mensagem* MensagemController::getMensagem(int id){
    if (id >= this->mensagens.size())
            this->mensagens.resize(id + 1);
    return &(this->mensagens[id]);
}