#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef DATABASE_HPP
#define DATABASE_HPP

class Database{
    public:
        static sqlite3* db;
        static int callback(void*, int, char**, char**);
        static void conectar(const std::string& nomeArquivo = "test.db");
        static void desconectar();
        static int getLatest();
        
        static std::vector<std::vector<std::string>> executarQuery(const std::string&);
};

#endif