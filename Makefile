CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -I.
LDFLAGS = -lcurl -lws2_32 -lwinmm -lwldap32

CLIENT = cliente.exe
SERVER = servidor.exe
DEMO = demo_usuario.exe

all: $(CLIENT) $(SERVER) $(DEMO)

$(CLIENT): main_cliente.o aluno.o biblioteca.o chat.o client.o forum.o grupo.o livro.o professor.o usuario.o server.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(SERVER): main_server.o aluno.o biblioteca.o chat.o client.o forum.o grupo.o livro.o professor.o usuario.o server.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(DEMO): demoUsuario.o aluno.o biblioteca.o chat.o client.o forum.o grupo.o livro.o professor.o usuario.o server.o
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del *.o $(CLIENT) $(SERVER) $(DEMO)

help:
	@echo "Comandos: make, make clean, make help"

.PHONY: all clean help
