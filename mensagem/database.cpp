#include "database.hpp"

Database::Database(){
    this->db = nullptr;
}

Database::~Database(){
    if(this->isConnected())
        this->desconectar();
}

void Database::conectar(const std::string& nomeArquivo){
    if(this->isConnected()) 
        this->desconectar();
    
    int resposta = sqlite3_open(nomeArquivo.c_str(),&this->db);
    if(resposta != SQLITE_OK){
        std::string msg = sqlite3_errmsg(this->db);
        this->desconectar();
        throw std::runtime_error("Erro ao abrir/criar banco: " + msg);
    }
}

void Database::desconectar(){
    if(this->isConnected()){
        sqlite3_close(this->db);
        this->db = nullptr;
    };
}
        
std::vector<std::vector<std::string>> Database::executarQuery(const std::string& query){
    if(!this->isConnected()) 
        throw std::runtime_error("Nao foi feita conexao com o banco de dados");

    char* msgDeErro = nullptr;
    std::vector<std::vector<std::string>> resposta;
    int status = sqlite3_exec(this->db, query.c_str(), this->callback, &resposta, &msgDeErro);    
    
    if(status != SQLITE_OK){
        std::string msg = msgDeErro ? msgDeErro : "Nao foi possivel identificar o erro";
        sqlite3_free(msgDeErro);
        throw std::runtime_error("Erro SQL: " + msg);
    }

    return resposta;
}

int Database::callback(void* data, int argc, char** argv, char** colName){
    std::vector<std::vector<std::string>>* resposta = static_cast<std::vector<std::vector<std::string>>*>(data);
    std::vector<std::string> linha;

    for (int i = 0; i < argc; ++i)
        linha.push_back(argv[i] ? argv[i] : "NULL");
    resposta->push_back(linha);
    return 0;
}

int Database::getLatest(){
    return sqlite3_last_insert_rowid(Database::db);
}

bool Database::isConnected(){
    return (this->db != nullptr);
}