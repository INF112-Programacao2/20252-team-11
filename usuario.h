#include <string>
#include <vector>

class Usuario {
    private:
        std::string nome;
        std::string matricula;
        std::string senha;
        vector<std::string> interesses;
        vector<std::string> livros;        // mudar para armazenar livros...
        float debito;
        Usuario* amigos;
        int n_amigos;
        std::string emailInstitucional;
        std::string cookie_value;

    public:

        struct Livro {
            std::string nome;
            std::string numero_chamada;
        };
        struct Debito {
            std::string nome;
            std::string debt;
        };

        std::string getNome();
        std::string getMatricula();
        std::vector<std::string> getInteresses();
        float getDebito();
        std::string getCookie();
        bool autenticar(std::string matricula,std::string senha);
        void addAmigo(Usuario& amigo);
        std::vector<Livro> buscarLivros(std::string busca); 
        std::string setCookieValue();
        std::vector<Debito> searchDebito();
};
