//
// Created by pedro on 13/11/2025.
//
#include "server.h"
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>

//===============================================
//CLASSE DE EXCECAO	
//===============================================

//Inicializa o servidor com configuracoes basicas
//Cria array inicial para chats com capacidade 2
Server::Server(int my_port) : num_fd(1),my_port(my_port) {}


Server::~Server() {
}

//===============================================
//FUNCAO AUXILIAR: ESCAPE DE STRING PARA SQL
//===============================================

//Escapa aspas simples em strings para uso seguro em queries SQL	
std::string escapeSql(const std::string& s){
	std::string out;
	for (char c : s)
		out += (c == '\'') ? "''" : std::string(1, c);
	return out;
};



//===============================================
//CLASSE DE EXCECAO	
//===============================================

//Excecao lancada quando o usuario sai do chat/conexao
//Herda de std::runtime_error para integracao com os sistemas de excecao do C++
class Saiu_do_chat : public std::runtime_error
{
public:
	Saiu_do_chat(std::string nome, std::string msg) : std::runtime_error(std::string(nome + msg).c_str()) {}
};


//Configura e inicializa socket do servidor
//Cria socket, define opcoes, faz bind e listen na porta especificada
void Server::listen_socket()
{
	int ret, reusar = 1;

	//CRIACAO DO SOCKET
	try
	{
		server = socket(AF_INET, SOCK_STREAM, 0);
		if (server < 0)
		{
			throw std::runtime_error("socket() failed");
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		exit(-1);
	}

	//CONFIGURA REUSO DE ENDERECO
	try
	{
		ret = setsockopt(server, SOL_SOCKET, AF_INET, (char *)&reusar, sizeof(reusar));
		if (ret < 0)
		{
			throw std::runtime_error("setsockopt() failed");
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	//CONFIGURA SOCKET COMO NAO BLOQUEANTE
	try
	{
		ret = ioctl(server, FIONBIO, (char *)&reusar);
		if (ret == -1)
		{
			throw std::runtime_error("ioctl() failed");
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	//CONFIGURA ESTRUTURA DE ENDERECO
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(my_port);
	memcpy(&serv_addr.sin_addr, (&in6addr_any), sizeof(in6addr_any));

	//BIND DO SOCKET AO ENDERECO
	try
	{
		ret = bind(server, (sockaddr *)&serv_addr, sizeof(serv_addr));
		if (ret < 0)
		{
			throw std::runtime_error("bind() failed");
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	//COLOCA SOCKET EM MODO DE ESCUTA
	try
	{
		ret = listen(server, 32);
		if (ret < 0)
		{
			throw std::runtime_error("listen() failed");
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		shutdown(server, SHUT_RDWR);
		exit(-1);
	}

	//CONFIGURA POLL PARA MONITORAMENTO
	memset(&fd_totais, 0, sizeof(fd_totais));
	fd_totais[0].fd = server;		// Primeiro descritor é o socket servidor
	fd_totais[0].events = POLLIN;	// Monitora por dados para ler

	std::cout << "Estamos online na porta: " << my_port << std::endl;
}


//===============================================
//LOOP PRINCIPAL DO SERVIDOR
//===============================================
//Metodo principal que executa o loop infinito do servidor
//Usa poll() para multiplexacao de I/O com timeout de 5 minutos
void Server::run()
{
	int ready;
	int timeout = 5 * 60 * 1000; //5 minutos em milissegundos

	while (true)		//loop infinito do servidor
	{
		try
		{
			std::cout << "Esperando em poll()" << std::endl;

			//Chama poll() para esperar por atividade nos sockets
			ready = poll(fd_totais, num_fd, timeout);
			if (ready < 0)
			{
				throw std::runtime_error("poll() failed");
			}
			else if (ready == 0)
			{
				throw std::runtime_error("poll() timed out");
			}
		}
		catch (std::runtime_error &e)
		{
			std::cerr << e.what() << std::endl;
			break;	//Sai do loop em caso de erro
		}

		//Processa descritores prontos
		try{
			processa_fd(ready);
		} catch (std::runtime_error& e){
			std::cerr << e.what() << std::endl;
			break;
		}
	}
}

//===============================================
//PROCESSAMENTO DE DESCRITORES
//===============================================
//Processa descritores de socket que estao prontos para I/O
//Aceita novas conexoes ou processa mensagens de clientes existentes

void Server::mostrar_antigas_msg(std::string num_chamada, int fd){
	std::cout << "Número de chamada: " << num_chamada << std::endl;
    Database database;
    database.conectar();

    std::string safeCallNum = escapeSql(num_chamada);

    std::string query = "SELECT conteudo, data FROM mensagem WHERE numChamado = '" + safeCallNum + "';";
    
    std::vector<std::vector<std::string>> ret = database.executarQuery(query);

	std::string buffr = "FORUM DO LIVRO: " + std::to_string(num_chamada) + "\n";
    std::cout << "carregando.";
	for(const auto& linha : ret){
        std::string conteudo = linha[0];
		std::string date = linha[1];
		
		conteudo.reserve(conteudo.size() + 1);
    	date.reserve(date.size() + 1);

        if (conteudo.empty() || conteudo.back() != '~') conteudo.push_back('~');
    	if (date.empty() || date.back() != '~') date.push_back('~');
		
		buffr.reserve(date.size() + conteudo.size());

		buffr += date;
		buffr += conteudo;
		buffr += "\n";
		std::cout << ".";
    }
	std::cout << std::endl;
	
    database.desconectar();

    buffr += "Você está online, digite quit para sair~~";
	envia_msg(buffer.c_str(), buffer.size(), fd);
}

void Server::processa_fd(int &ready)
{
	int cli;
	bool diminuir_array = false; //flag para compactar array se cliente desconectar

//itera por todos os descritores ate processar todos os 'ready'	
	for (int i = 0; i < num_fd && ready > 0; i++)
	{
		try
		{
			//verifica se descritor tem dados para ler
			if (fd_totais[i].revents == POLLIN)
			{
				ready--;	//decrementa contador de prontos

				//CASO 1: eh o socket servidor (nova conexao)
				if (fd_totais[i].fd == server)
				{	
					//Loop para aceitar multiplas conexoes pendentes
					do
					{
						try
						{	
							//Aceita nova conexao 
							cli = accept(server, nullptr, nullptr);
							if (cli < 0)
							{
								if (errno != EWOULDBLOCK)
									throw std::system_error(errno, std::system_category(), "accept() failed");
								else
									throw std::exception();	//Nao ha mais conexoes pendentes
							}
						}
						catch (std::system_error &e)
						{
							throw std::runtime_error(e.what());
							break;	//Sai do loop accept
						}
						catch (std::exception &e)
						{
							break;
						}

						//Adiciona novo cliente ao array poll
						fd_totais[num_fd].fd = cli;
						fd_totais[num_fd++].events = POLLIN;
						printf("Nova conexão, file descriptor %d\n", num_fd - 1);

						//Processa handshake inicial do cliente
						try
						{
							this->receber_descritor(num_fd - 1);
						}
						catch (std::runtime_error &e)
						{
							shutdown(fd_totais[i].fd, SHUT_RDWR);
							fd_totais[i].fd = -1;
							diminuir_array = true;
							break;
						}

						//ENVIA HISTORICO DE MENSAGENS DO BANCO DE DADOS	
		                try{
							std::cout << "Mensagens antigas\n";
		                	mostrar_antigas_msg(biblioteca.getChat(clients.at(num_fd - 1)->getChatId()).getLivro().getId(), fd_totais[num_fd-1].fd);
						}catch(std::exception& e){
			                std::cerr << e.what() << std::endl;
		                }
					} while (cli != -1);	//Continua aceitando ate nao haver mais conexoes
				}

				//CASO 2: CLIENTE EXISTENTE
				else
				{
					try
					{	
						//Processa mensagem recebida
						char *msg = this->processa_msg(i);
						this->interpreta_msg(static_cast<const char *>(msg), strlen(msg), clients[i], fd_totais[i].fd);
						delete[] msg;	//Libera buffer da mensagem
					}
					catch (std::runtime_error &e)
					{
						//Cliente desconectou ou erro
						std::cout << e.what() << std::endl;
						shutdown(fd_totais[i].fd, SHUT_RDWR);
						fd_totais[i].fd = -1;
						diminuir_array = true;
						break;
					}
				}
				//Verifica por outros eventos de erro
			} else if (fd_totais[i].revents != 0){
				throw std::runtime_error("Error in revents");
			}
		}
		catch (std::exception &e)
		{
			throw std::runtime_error(e.what());
			break;
		}
	}

	//Compacta array se houver descritores removidos
	if (diminuir_array){
		diminuir_array = false;
		for (int i = 0; i < num_fd; i++)
		{
			if (fd_totais[i].fd == -1)	//Encontra slot vazio
			{
				biblioteca.getChat(clients[i]->getChatId()).removeParticipantes(clients.at(i)->getMatricula());
				//Desloca elementos para preencher espacos
				for (int j = i; j + 1 < num_fd; j++)
				{
					fd_totais[j].fd = fd_totais[j + 1].fd;
					auto aux = clients[j + 1];
					clients[j] = aux;
				}
				clients.erase(std::prev(clients.end())); //Remove ultimo elemento do mapa
				i--;	//reavalia mesma posicao
				num_fd--;	//reduz contador total
			}
		}
	}
}

//===============================================
//HANDSHAKE DO CLIENTE
//===============================================

//Processa mensagem inicial do cliente e configura objeto Usuario
void Server::receber_descritor(int index)
{
    try
    {
       // int fd = fd_totais[index].fd;

        char* raw_msg = processa_msg(index);
        if (raw_msg == nullptr) return;
        std::string msg_str(raw_msg);
        delete[] raw_msg;

        size_t p1 = msg_str.find('\7');
        size_t p2 = msg_str.find('\7', p1 + 1);
        size_t p3 = msg_str.find('\7', p2 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
             throw std::runtime_error("Protocolo de handshake invalido");
        }

        std::string nome = msg_str.substr(0, p1);
        std::string nome_livro = msg_str.substr(p1 + 1, p2 - (p1 + 1));
        std::string matricula = msg_str.substr(p2 + 1, p3 - (p2 + 1));
        std::string n_chamada = msg_str.substr(p3 + 1);

        Usuario* user = new Usuario(); 
        user->setNome(nome);
        user->setMatricula(matricula);

        Livro* livro = new Livro(nome_livro, n_chamada);
        livros.push_back(livro); 

        int chatId = biblioteca.findChat(livro->getNome());
        
        // Lógica de criação de chat
        if (chatId == -1){
            Chat chat_geral(*livro);
            chatId = biblioteca.addChat(chat_geral);
        }
        
        user->setchatId(chatId);

        biblioteca.getChat(chatId).addParticipante(*user, index);

        clients.insert({index, user}); 

        std::cout << "Cliente " << nome << " registrado no indice " << index << "\n";

    } catch (std::exception& e){
        std::cerr << "Erro no handshake: " << e.what() << std::endl;
        throw; // Re-lança para o chamador tratar a desconexão
    }
}

//Retorna string com data e hora atual no formato "YYYY-MM-DD HH:MM:SS"
std::string getCurrentDateTimeBRT() {
    setenv("TZ", "America/Sao_Paulo", 1);
    tzset();

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

//interpreta e processa mensagens recebidas dos clientes
void Server::interpreta_msg(const char *buff, int bytes, Usuario *user, int fd)
{

    if (bytes <= 0) return;

    std::string conteudo(buff, bytes);

    if (!conteudo.empty() && conteudo.back() == '\n'){
        conteudo.pop_back();
    }

    if (conteudo.size() < 4) return;

    std::string prefixo = conteudo.substr(0, 4);

    if (prefixo == "[!] ")
    {
    	//envia para todos os clientes exceto o remetente

        std::string msgBody = conteudo.substr(4);
        std::string msgFinal = '[' + user->getNome() + "] " + msgBody;
		std::string date = getCurrentDateTimeBRT() + "~\0";

    	//Armazena mensagem no banco de dados
    	std::string safeMsg = escapeSql(msgFinal);
    	std::string safeNumChamado = escapeSql(biblioteca.getChat(user->getChatId()).getLivro().getId());

    	Database database;
    	database.conectar();
		std::cout << "Inserido: " << safeMsg << std::endl;
		database.executarQuery("INSERT INTO mensagem (conteudo, numChamado, data) VALUES ('" + safeMsg + "','" + safeNumChamado + "','" +  getCurrentDateTimeBRT() +"'); ");
    	database.desconectar();

        Chat& chat = biblioteca.getChat(user->getChatId());
        
        for (auto const& par : chat.getParticipantes())
        {
            int id_participante = par.first; // O índice no fd_totais
            
            // Valida se o índice ainda é válido
            if (id_participante < 0 || id_participante >= num_fd) continue;

            if (fd_totais[id_participante].fd != fd)
            {
                try {
					envia_msg(date.c_str(), date.size(), fd_totais[id_participante].fd);
                    envia_msg(msgFinal.c_str(), msgFinal.size(), fd_totais[id_participante].fd);
                } catch (std::exception &e) {
                    std::cerr << "Falha ao enviar para " << id_participante << "\n";
                }
            }
        }
    }
    else if (prefixo == "quit")
    {
        throw Saiu_do_chat(user->getNome(), " saiu.");
    }
}

//===============================================
//PROCESSAMENTO DE MENSAGENS
//===============================================

//Le mensagem do socket do cliente
//Retorna buffer alocado dinamicamente com a mensagem
char *Server::processa_msg(int index)
{
    int fd = fd_totais[index].fd;

    char *buffer = new char[BUFFSIZE];

    memset(buffer, 0, BUFFSIZE);

    int bytes_recv = recv(fd, buffer, BUFFSIZE, 0);

    if (bytes_recv <= 0)
    {
        delete[] buffer;
        if (bytes_recv == 0) {
            throw Saiu_do_chat(clients[index]->getNome(), " desconectou.");
        } else {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                 throw std::runtime_error("Sem dados (Non-blocking)");
            }
            throw std::runtime_error("Erro no recv");
        }
    }
     std::cout << "Recebidos " << bytes_recv << " bytes\n";

    return buffer; 
}

//ENVIO DE MENSAGENS
//Envia mensagem para um socket especifico
void Server::envia_msg(const char* buff, int bytes, int fd)
{
	int bytes_sent;
	try
	{
		bytes_sent = send(fd, buff, bytes, 0);
		if (bytes_sent < 0)
		{
			throw std::runtime_error("Error in send()");
		}
	} catch (std::runtime_error& e){
		throw std::runtime_error(e.what());
	}
}

//Encerramento de servidor
//Fecha todas as conexoes e encerra o servidor
void Server::close(){
	std::cout << "Server closed" << std::endl;
	for (int fd=0; fd<num_fd; fd++){
		if (fd_totais[fd].fd >=0 ){
			shutdown(fd_totais[fd].fd, SHUT_RDWR);
		}
	}
	exit(0);
}
