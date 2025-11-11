#include <string>

class Usuario {
    private:
        std::string nome;
        std::string matricula;
        std::string senha;
        std::string* interesses;
        std::string* livros;        // mudar para armazenar livros...
        float debito;
        Usuario* amigos;
        int n_amigos;
        std::string emailInstitucional;
        std::string cookie_value;

    public:
        std::string getNome();
        std::string getMatricula();
        std::string* getInteresses();
        float getDebito();
        std::string getCookie();
        bool autenticar(std::string matricula,std::string senha);
        void addAmigo(Usuario& amigo);
        std::string* buscarLivros(); 
        std::string setCookieValue();
};