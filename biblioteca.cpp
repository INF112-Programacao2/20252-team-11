#include "biblioteca.h"

//construtor e destrutor


//===============================================
//CLASSE DE EXCECAO	
//===============================================

//Inicializa o servidor com configuracoes basicas
//Cria array inicial para chats com capacidade 2
Biblioteca::Biblioteca() : size_chats(2), num_chats(0),
{
	chats = new Chat*[size_chats];		//Alocacao dinamica do array de chats
}


Biblioteca::~Biblioteca() {

	 for (Chat* chat : _chats) {
        delete chat;
    }
	
    //libera memoria de todos os livros
    //limpa os vetores
    _usuarios.clear();
}



//Retorna ponteiro para array de chatd
Chat **Biblioteca::get_chats()
{
	return chats;
}


//===============================================
//ADICAO DE CHATS
//===============================================

//Adiciona um novo chat ao servidor com realocacao dinamica do array
//Implementa padrao de array redimensionavel
void Biblioteca::add_chat(Chat *chat)
{
	if (num_chats >= size_chats)
	{
		Chat **aux = new Chat *[size_chats];
		for (int i = 0; i < size_chats; i++)
		{
			aux[i] = chats[i];	//copia ponteiros
		}
		delete[] chats;		//liber array antigo

		//Aloca novo array com dobro de capacidade		
		chats = new Chat *[size_chats * 2];
		for (int i = 0; i < size_chats; i++)
		{
			chats[i] = aux[i];	//restaura ponteiros
		}
		delete [] aux;		//libera array temporario
		size_chats *= 2;	//atualiza capacidade
	}
	chats[num_chats++] = chat; //adiciona novo chat
}

    //os gets retornam copias do vetor livros e usuarios respectivamente
    std::vector<Livro*> Biblioteca::getLivros() const {
        return _livros;
    }

    std::vector<Usuario*> Biblioteca::getUsuarios() const {
        return _usuarios;
	}

	std::vector<Chat*>Biblioteca::getChat() const {
	return _chats;
	}

    //adiciona usuario a bilblioteca, evitando duplicatas por matricula
    void Biblioteca::addUsuario(Usuario* novoUsuario) {
        if (novoUsuario != nullptr){
            for (Usuario* u : _usuarios) {
                if (u->getMatricula() == novoUsuario->getMatricula()) {
                    return; 
                }
            }
            _usuarios.push_back(novoUsuario);    //adiciona novo usuario
        }
    }

 //adiciona livro a bilblioteca, evitando duplicatas por ID
    void Biblioteca::addLivro(Livro* novoLivro) {
        if (novoLivro != nullptr){
            for(Livro* l : _livros){
                if(l->getId()==novoLivro->getId()){
                    return;
                }
                
            }
            _livros.push_back(novoLivro);    //adiciona novo livro
        }   
    }


	Chat* Biblioteca::getChat(int posicao) {
		if (posicao < 0 || posicao >= (int)_chats.size()) {
			return nullptr;
		}
		return _chats[posicao]
	}







