//
// Created by pedro on 13/11/2025.
//
#include "server.h"

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


//===============================================
//CLASSE DE EXCECAO	
//===============================================

//Inicializa o servidor com configuracoes basicas
//Cria array inicial para chats com capacidade 2
Server::Server(int my_port) : size_chats(2),
							  num_chats(0),
							  num_fd(1),
							  my_port(my_port)
{
	chats = new Chat*[size_chats];		//Alocacao dinamica do array de chats
}

//Libera recursos alocados dinamicamente
Server::~Server()
{
	delete[] chats;

	for (Livro* l : livros) {
    	delete l;
    }
    livros.clear();
}

//Retorna ponteiro para array de chatd
Chat **Server::get_chats()
{
	return chats;
}

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
//ADICAO DE CHATS
//===============================================

//Adiciona um novo chat ao servidor com realocacao dinamica do array
//Implementa padrao de array redimensionavel
void Server::add_chat(Chat *chat)
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


//===============================================
//LOOP PRINCIPAL DO SERVIDOR
//===============================================
//Metodo principal que executa o loop infinito do servidor
//Usa poll() para multiplexacao de I/O com timeout de 5 minutos
void Server::run()
{
	int ready;
	int timeout = 10 * 60 * 1000; //5 minutos em milissegundos
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
	Database database;
	database.conectar();
	//Recupera todas as mensagens do banco
	std::vector<std::vector<std::string>> ret = database.executarQuery("SELECT conteudo, data FROM mensagem WHERE numChamado = '"+ num_chamada + "';");
	for(std::vector<std::string> re : ret){
		re[0].push_back('~');
		envia_msg(re[0].c_str(), re[0].size(), fd);
        envia_msg(re[1].c_str(), re[1].size(), fd);
	}
	database.desconectar();
	std::string msg = "Você está online~~";
	envia_msg(msg.c_str(), msg.size(), fd);
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
                            mostrar_antigas_msg(@, fd);
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
		int fd = fd_totais[index].fd;
		char *msg = processa_msg(index);	//Le mensagem inicial
		int count = 0;
		for (count = 0; msg[count] != '\7'; count++)	//Encontra primeiro delimitador
			;
		std::string nome = std::string(msg).substr(0, count);
		int count2;
		for (count2 = count + 1; msg[count2] != '\7'; count2++)	//Encontra segundo delimitador
			;
		std::string forum = std::string(msg).substr(count, count2);
		std::string matricula = std::string(msg).substr(count2, std::string(msg).size());

		//Cria e configura objeto do cliente
		Usuario *user = new Aluno;
		user->setNome(nome);
		user->setMatricula(matricula);
        user->setForum(forum);
		clients.insert({index, user});	//Armazena no mapa

		//Cria e armazena o objeto livro (relacionado ao usuario)
		Livro *livro = new Livro(user, forum, id);

		// armazena o livro
		livros.push_back(livro);

		// registra o leitor atual
		livro->registrarLeitorAtual(); 	//registra o usuario como leitor

		delete[] msg;
	} catch (std::runtime_error& e){
		throw std::runtime_error(e.what());
	}
}

//Retorna string com data e hora atual no formato "YYYY-MM-DD HH:MM:SS"
std::string getCurrentDateTime() {
    auto t = std::time(nullptr);
    auto now = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

//interpreta e processa mensagens recebidas dos clientes
void Server::interpreta_msg(const char *buff, int bytes, Usuario *user, int fd)
{
    std::string conteudo(buff);

	//remove newline do final se existir
    if (!conteudo.empty() && conteudo.back() == '\n'){
		conteudo.pop_back();
    }

	//extrai prefixo de 4 caracteres para identificar tipo de mensagem
	std::string prefixo(buff, 4);

	//formata mensagem : "[Nome] mensagem"
	std::string msg = '[' + user->getNome() + "] " + conteudo.substr(4);
	std::string dataHora = getCurrentDateTime();
    dataHora.push_back('~');

	//CASO 1:MENSAGEM DE BROADCAST
	if (prefixo.compare("[!] ") == 0)
	{	
		//Armazena mensagem no banco de dados
        std::string safeMsg = escapeSql(msg);
		std::string safeNumChamado = escapeSql(user->getChat().getLivro().getId());

        Database database;
		database.conectar();
		database.executarQuery("INSERT INTO mensagem (conteudo, numChamado) VALUES ('" + safeMsg + "','" + safeNumChamado + "'); ");
		database.desconectar();
		//envia para todos os clientes exceto o remetente
		for (auto i : clients)
		{
			if (fd_totais[i.first].fd != fd)
			{
				try
				{
					envia_msg(dataHora.c_str(), dataHora.size(), fd_totais[i.first].fd);	//envia timestamps
					envia_msg(msg.c_str(), strlen(msg.c_str()), fd_totais[i.first].fd);		//envia mensagens
				}
				catch (std::exception &e)
				{
					std::cerr << e.what() << std::endl;
					this->close();	//fecha servidor no caso de erro
				}
			}
		}
	}

	//CASO 2: COMANDO QUIT: (desconexao)
	else if (prefixo.compare("quit") == 0)
	{
		throw Saiu_do_chat(user->getNome(), " saiu da conversa");
	}
}

//===============================================
//PROCESSAMENTO DE MENSAGENS
//===============================================

//Le mensagem do socket do cliente
//Retorna buffer alocado dinamicamente com a mensagem
char *Server::processa_msg(int index)
{
	int bytes_recv, bytes_sent;
	int fd = fd_totais[index].fd;
	char *buffer = new char[BUFFSIZE];
	char *msg = new char[BUFFSIZE];
	memset(msg, '\0', BUFFSIZE);
	try
	{
		//recebe dados do socket
		bytes_recv = recv(fd, buffer, BUFFSIZE, 0);
		if (bytes_recv <= 0)
			if ((errno != EWOULDBLOCK) || bytes_recv == 0)
			{
				//cliente desconectou
				throw Saiu_do_chat(clients[index]->getNome(), " interrompeu a conexão");
			}
			else
			{
				//nenhum dado disponivel
				throw std::exception();
			}
	}
	catch (std::runtime_error &e)
	{
		throw std::runtime_error(e.what());
		return msg;
	}
	catch (std::exception &e)
	{
		return msg;
	}

	//Log da mensagem recebida
	std::cout << "Recebidos " << bytes_recv << " bytes, fd: " << fd << std::endl;
	for (int i=0; i<bytes_recv; i++){
		std::cout << buffer[i];	//debug; mostra caracteres
		msg[i] = buffer[i];		//copia para buffer de retorno
	}
	std::cout << '\n';
	return msg;
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
