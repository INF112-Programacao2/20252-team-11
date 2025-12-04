#include "biblioteca.h"

//construtor e destrutor


//===============================================
//CLASSE DE EXCECAO	
//===============================================

//Inicializa o servidor com configuracoes basicas
//Cria array inicial para chats com capacidade 2
Biblioteca::Biblioteca(){}

Biblioteca::~Biblioteca() {}

//===============================================
//ADICAO DE CHATS
//===============================================

//Adiciona um novo chat ao servidor com realocacao dinamica do array
//Implementa padrao de array redimensionavel
void Biblioteca::addChat(Chat chat)
{
	_chats.push_back(chat); //adiciona uma copia do chat no vector
}

    //os gets retornam copias do vetor livros e usuarios respectivamente
	//return _livros, por exemplo, cria uma copia completa do vector que contem os mesmos ponteiros
    std::vector<Livro*> Biblioteca::getLivros() const {
        return _livros; 
    }

    std::vector<Usuario*> Biblioteca::getUsuarios() const {
        return _usuarios;
	}

	Chat Biblioteca::getChat(int id) const {
	    return _chats[id];
	}

    //adiciona usuario a bilblioteca, evitando duplicatas por matricula
	//verifica se nao eh ponteiro nulo e procura se usuario ja existe
    void Biblioteca::addUsuario(Usuario* novoUsuario) {
        if (novoUsuario != nullptr){
            for (Usuario* u : _usuarios) {
                if (u->getMatricula() == novoUsuario->getMatricula()) {
                    return;  //se encontrar, sai sem adicionar
                }
            }
            _usuarios.push_back(novoUsuario);    //adiciona novo usuario
        }
    }

 //adiciona livro a bilblioteca, evitando duplicatas por ID
    void Biblioteca::addLivro(Livro* novoLivro) {
        if (novoLivro != nullptr){ //mesma verificacao de ponteiro, ve se nao eh ponteiro nulo (ponteiro que nao aponta pra nenhum objeto valido)
            for(Livro* l : _livros){
                if(l->getId()==novoLivro->getId()){
                    return;
                }
                
            }
            _livros.push_back(novoLivro);    //adiciona novo livro
        }   
    }


std::vector<Chat> Biblioteca::getChats() {
    return this->_chats;
}
int findChat(std::string s){
	for(int i=0;i<_chats.size();i++){
		if(_chats[i].getlivro().getnome() == s){
			return i; 
		}
	}
	return -1;
	
}




