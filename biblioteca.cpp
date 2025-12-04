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
int Biblioteca::addChat(Chat chat)
{
    _chats.push_back(chat);
    
    // Retorna o índice do elemento que acabamos de inserir
    return _chats.size() - 1; 
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


Chat& Biblioteca::getChat(int id) {
    if (id < 0 || id >= _chats.size()){
        throw std::out_of_range("Chat id invalido");
    }
    return _chats[id];
}

int Biblioteca::findChat(std::string s){
	for(int i=0;i<_chats.size();i++){
		if(_chats[i].getLivro().getNome() == s){
			return i; 
		}
	}
	return -1;
	
}






