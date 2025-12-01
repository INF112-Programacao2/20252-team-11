#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef DATABASE_HPP
#define DATABASE_HPP

class Database{
    private:
        sqlite3* db;
        static int callback(void*, int, char**, char**);
    public:
        Database();
        ~Database();

        bool isConnected();
        void conectar(const std::string& nomeArquivo = "betterPergamum.db");
        void desconectar();
        int getLatest();
        
        std::vector<std::vector<std::string>> executarQuery(const std::string&);
};

#endif