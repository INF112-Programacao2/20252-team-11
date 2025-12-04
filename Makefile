# Compilador e Flags Gerais
CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread
LDFLAGS = -lcurl

# Flags específicas
SERVER_LDFLAGS = -lsqlite3

# Nomes dos executáveis
SERVER_EXEC = server
CLIENT_EXEC = client

# Fontes do Servidor
SERVER_SRCS = server.cpp aluno.cpp main_server.cpp client.cpp usuario.cpp \
              livro.cpp chat.cpp biblioteca.cpp mensagem/database.cpp

# Fontes do Cliente
CLIENT_SRCS = client.cpp main_client.cpp professor.cpp usuario.cpp \
              livro.cpp aluno.cpp

# Regra padrão 
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Regra para compilar o Server diretamente dos fontes
$(SERVER_EXEC): $(SERVER_SRCS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRCS) -o $@ $(LDFLAGS) $(SERVER_LDFLAGS)
	@echo "Servidor compilado com sucesso!"

# Regra para compilar o Client diretamente dos fontes
$(CLIENT_EXEC): $(CLIENT_SRCS)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRCS) -o $@ $(LDFLAGS)
	@echo "Cliente compilado com sucesso!"

# Instalação de Dependências 
install_deps:
	@echo "Detectando sistema operacional..."
	@if [ -f /etc/arch-release ]; then \
		echo "Arch Linux detectado."; \
		sudo pacman -S --needed base-devel curl sqlite; \
	elif [ -f /etc/debian_version ]; then \
		echo "Debian/Ubuntu detectado."; \
		sudo apt-get update; \
		sudo apt-get install -y build-essential libcurl4-openssl-dev libsqlite3-dev; \
	else \
		echo "Sistema não suportado automaticamente. Por favor instale: g++, libcurl, sqlite3"; \
	fi

# Limpeza dos arquivos executáveis 
clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)
	@echo "Limpeza concluída."

.PHONY: all clean install_deps
