#include "usuario.h"
#include <string>


class Professor : public Usuario {
    private:
        std::string orgao;
        std::string departamento;
        std::string telefone;
    
    public:
        
        std::string getOrgao();
        std::string getDep();
        std::string getTelefone();
        void addForum();
        void sendMessage();
};
