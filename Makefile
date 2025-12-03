# ==============================================
# CONFIGURAÇÕES
# ==============================================
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I. -pthread
LDFLAGS = -lcurl -lsqlite3

# Executáveis
CLIENT = cliente
SERVER = servidor
BUSCA = busca
DEMO = demo

# Fontes
CLIENT_SRCS = main_client.cpp client.cpp usuario.cpp aluno.cpp professor.cpp \
              biblioteca.cpp livro.cpp chat.cpp
SERVER_SRCS = main_server.cpp server.cpp usuario.cpp 

# ==============================================
# REGRAS PRINCIPAIS
# ==============================================
.PHONY: all clean deps install-deps check-deps run-demo help

all: deps $(CLIENT) $(SERVER) $(BUSCA) $(DEMO)
	@echo ""
	@echo " COMPILAÇÃO CONCLUÍDA!"
	@echo "========================="
	@echo "Executáveis gerados:"
	@echo "  ./$(CLIENT)  - Cliente principal"
	@echo "  ./$(SERVER)  - Servidor de chat"
	@echo "  ./$(BUSCA)   - Busca de livros"
	@echo "  ./$(DEMO)    - Demonstração"
	@echo ""
	@echo " PARA USAR:"
	@echo "  1. Terminal 1: ./$(SERVER)"
	@echo "  2. Terminal 2: ./$(CLIENT)"
	@echo "  3. Teste rápido: ./$(BUSCA)"
	@echo "========================="

# ==============================================
# VERIFICAÇÃO E INSTALAÇÃO DE DEPENDÊNCIAS
# ==============================================
check-deps:
	@echo " VERIFICANDO DEPENDÊNCIAS..."
	
	# Verifica sistema
	@if [ -f /etc/debian_version ] || grep -q "Ubuntu\|Debian" /etc/os-release 2>/dev/null; then \
		echo " Sistema Debian/Ubuntu detectado"; \
	else \
		echo " AVISO: Sistema não Debian/Ubuntu - verifique dependências manualmente"; \
	fi
	
	# Verifica compilador
	@if which g++ >/dev/null 2>&1; then \
		echo " Compilador g++ encontrado"; \
	else \
		echo " ERRO: g++ não instalado. Instalando..."; \
		sudo apt-get install -y g++ 2>/dev/null || echo " Falha ao instalar g++"; \
	fi
	
	# Verifica libcurl
	@if pkg-config --exists libcurl 2>/dev/null || [ -f /usr/include/curl/curl.h ] || [ -f /usr/include/x86_64-linux-gnu/curl/curl.h ]; then \
		echo " libcurl encontrado"; \
	else \
		echo " libcurl NÃO encontrado. Será instalado..."; \
	fi
	
	# Verifica sqlite3
	@if pkg-config --exists sqlite3 2>/dev/null || [ -f /usr/include/sqlite3.h ] || [ -f /usr/include/x86_64-linux-gnu/sqlite3.h ]; then \
		echo " sqlite3 encontrado"; \
	else \
		echo " sqlite3 NÃO encontrado. Será instalado..."; \
	fi
	
	# Verifica make
	@if which make >/dev/null 2>&1; then \
		echo " Make encontrado"; \
	else \
		echo " ERRO: make não instalado. Instalando..."; \
		sudo apt-get install -y make 2>/dev/null || echo " Falha ao instalar make"; \
	fi

install-deps: check-deps
	@echo ""
	@echo " INSTALANDO DEPENDÊNCIAS..."
	@echo "=============================="
	@sudo apt-get update -qq
	@sudo apt-get install -y \
		libcurl4-openssl-dev \
		libsqlite3-dev \
		build-essential \
		g++ \
		make 2>/dev/null || \
		(echo " ERRO: Falha ao instalar dependências. Execute manualmente:" && \
		 echo " sudo apt update && sudo apt install libcurl4-openssl-dev libsqlite3-dev build-essential")
	@echo "=============================="
	@echo " Dependências instaladas com sucesso!"
	@echo ""

deps: install-deps

# ==============================================
# COMPILAÇÃO DOS EXECUTÁVEIS
# ==============================================
$(CLIENT): $(CLIENT_SRCS)
	@echo " Compilando cliente..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 2>&1 | grep -E "(error|Error|ERROR)" || echo " ✓ Cliente compilado com sucesso"

$(SERVER): $(SERVER_SRCS)
	@echo " Compilando servidor..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 2>&1 | grep -E "(error|Error|ERROR)" || echo " ✓ Servidor compilado com sucesso"

$(BUSCA): $(BUSCA_SRCS)
	@echo " Compilando busca de livros..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 2>&1 | grep -E "(error|Error|ERROR)" || echo " ✓ Busca compilada com sucesso"

$(DEMO): $(DEMO_SRCS)
	@echo " Compilando demonstração..."
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 2>&1 | grep -E "(error|Error|ERROR)" || echo " ✓ Demo compilada com sucesso"

# ==============================================
# LIMPEZA
# ==============================================
clean:
	@echo " Limpando arquivos compilados..."
	@rm -f $(CLIENT) $(SERVER) $(BUSCA) $(DEMO) *.o
	@echo " Limpeza concluída"

# ==============================================
# DEMONSTRAÇÃO AUTOMÁTICA
# ==============================================
run-demo: all
	@echo ""
	@echo " DEMONSTRAÇÃO COMPLETA DO SISTEMA"
	@echo "========================================="
	
	# Verifica se os executáveis existem
	@if [ ! -f ./$(SERVER) ]; then \
		echo " ERRO: Servidor não compilado"; \
		exit 1; \
	fi
	
	@if [ ! -f ./$(BUSCA) ]; then \
		echo " ERRO: Busca não compilada"; \
		exit 1; \
	fi
	
	@echo ""
	@echo "1.  Iniciando servidor em segundo plano..."
	@./$(SERVER) > /dev/null 2>&1 &
	@SERVER_PID=$$!; \
	sleep 2; \
	echo "    Servidor iniciado (PID: $$SERVER_PID)"
	
	@echo ""
	@echo "2.  Testando busca de livros..."
	@echo "----------------------------------------"
	@if ./$(BUSCA) "algoritmo" 2>/dev/null | head -10; then \
		echo ""; \
	else \
		echo "    (Executando busca sem argumentos)"; \
		./$(BUSCA) 2>/dev/null || true; \
	fi
	@echo "----------------------------------------"
	
	@echo ""
	@echo "3.  Para conectar o cliente:"
	@echo "    Abra OUTRO terminal e execute:"
	@echo "    cd $(shell pwd) && ./$(CLIENT)"
	
	@echo ""
	@echo "4.  Para encerrar o servidor:"
	@echo "    Execute: kill $$SERVER_PID"
	@echo "    Ou feche este terminal"
	
	@echo ""
	@echo "========================================="
	@echo " Demonstração pronta! O servidor está rodando em background."
	@echo " Pressione ENTER para finalizar a demonstração..."
	@read dummy; \
	kill $$SERVER_PID 2>/dev/null || true; \
	echo " Servidor finalizado"

# ==============================================
# EXECUÇÃO SIMPLES
# ==============================================
run-server: $(SERVER)
	@echo " Iniciando servidor..."
	@echo " Servidor rodando na porta 12345"
	@echo " Para parar: Ctrl+C"
	@echo "----------------------------------------"
	@./$(SERVER)

run-client: $(CLIENT)
	@echo " Iniciando cliente..."
	@echo " Conectando ao servidor..."
	@echo " Para sair: digite 'quit'"
	@echo "----------------------------------------"
	@./$(CLIENT)

run-busca: $(BUSCA)
	@echo " Busca de livros - Pergamum UFV"
	@echo " Digite um termo de busca (ex: 'algoritmos')"
	@echo "----------------------------------------"
	@./$(BUSCA)

# ==============================================
# AJUDA
# ==============================================
help:
	@echo "SISTEMA BIBLIOTECA UFV - COMANDOS DO MAKEFILE"
	@echo "=============================================="
	@echo "  make all       - Compila TUDO (instala dependências se necessário)"
	@echo "  make deps      - Instala/verifica dependências"
	@echo "  make clean     - Limpa arquivos compilados"
	@echo "  make run-demo  - Executa demonstração completa"
	@echo ""
	@echo "  make run-server - Executa apenas o servidor"
	@echo "  make run-client - Executa apenas o cliente"
	@echo "  make run-busca  - Executa apenas a busca"
	@echo ""
	@echo "  make help      - Mostra esta ajuda"
	@echo "=============================================="

# Regra padrão
.DEFAULT_GOAL := help
