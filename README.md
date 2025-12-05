# BetterPergamum

<p align="center">
  <img width="493" height="223" alt="image" src="https://github.com/user-attachments/assets/86f99827-00af-4e03-9643-2fb5b7070d9a" />
</p>

<div align="center">

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-lightgrey)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B-blue)]()
[![Contribuições](https://img.shields.io/badge/contributions-welcome-success)]()

</div>

---

## Integrantes do Grupo 11

| Nome                             | Matrícula |
| -------------------------------- | --------- |
| **Gabriel Spagnol Gomes**        | 120571    |
| **Arthur Mendes Vilela**         | 120570    |
| **Pedro Silva Francisco**        | 120554    |
| **João Vitor Miranda Marcelino** | 113477    |
| **Marco Thullyo São Severino**   | 120550    |

Uma versão mais legível desse texto está disponível na pasta "documentação"


---


**BETTER PERGAMUM** <br>

O Better Pergamum é um sistema de biblioteca desenvolvido em C++ que replica funcionalidades do sistema Pergamum da UFV, com adição de recursos modernos como fóruns de discussão por livro, autenticação integrada e interface baseada em sockets.  <br>
O sistema foi feito como trabalho final da disciplina INF112, ministrada pelo professor Julio Cesar Soares dos Reis, no curso Ciências da Computação da Universidade Federal de Viçosa.


O sistema é composto por dois componentes principais: <br>

- Servidor: Gerencia conexões, fóruns de discussão e armazenamento de mensagens.
- Cliente: Interface para usuários (alunos e professores) acessarem funcionalidades da biblioteca.


---


**Pré-requisitos:** 

Dependências manuais:

- g++ (compilador C++)
- libcurl4-openssl-dev
- libsqlite3-dev
- pthreads (incluído no glibc)


O sistema funciona apenas no Linux, pois foi feito para tal.


---


**Instalação manual** <br>

sudo apt-get update
sudo apt-get install build-essential libcurl4-openssl-dev libsqlite3-dev


**Compilação:** <br>

Compila servidor e cliente

make all
make clean (remove executáveis gerados)

Método alternativo (sem Makefile): <br>

g++ server.cpp aluno.cpp main_server.cpp client.cpp usuario.cpp livro.cpp chat.cpp biblioteca.cpp mensagem/database.cpp -pthread -lcurl -lsqlite3 -o server; ./server


g++ client.cpp main_client.cpp professor.cpp usuario.cpp livro.cpp aluno.cpp -pthread -lcurl -o client; ./client
<br>

**Execução:** <br>
./server
(o servidor iniciará na porta 12345)


./client 
(interface ativa será iniciada)
<br>
⚠️⚠️Atenção!⚠️⚠️

O server precisa estar aberto para o client funcionar. O usuário pode abrir varios clients para interagir um com o outro


---


**Breve tutorial de uso:** <br>
Para usuários (clientes)
<br>
**Execução:** <br>
./client
<br>
**Seleção do tipo de usuário:** <br>

-------SEJA BEM VINDO AO BETTER PERGAMUM-------

Você é:

1 - Aluno

2 - Professor

Resposta: _

---

**Autenticação:** <br>

PARA ALUNOS: <br>
Digite sua matricula: [sua_matricula]
<br>
Digite a sua senha da BBT: [sua_senha]
<br>
PARA PROFESSORES: <br>
Digite seu Nome: [nome_completo]
<br>
Digite o seu email: [email_institucional]
<br>

**Menu dos alunos:**

Escolha a função que você deseja executar:

1 - Pesquisa de livros

2 - Consultar o débito

3 - Visualizar perfil

4 - Encerrar programa

Resposta: _

<br>

**Menu dos professores:**

Escolha a função você deseja executar:

1 - Pesquisa de livros

2 - Visualizar perfil

3 - Encerrar programa

Resposta: _
<br>

---


**Busca de livros(digitando 1):**

PESQUISA DE LIVROS: <br>

Digite os termos para a pesquisa: [termo_de_busca]

Primeiro resultado:

| Nome: [Titulo do Livro]

| N.Chamada: [Numero de Chamada]

<br>
Deseja acessar o forum do livro?

Se sim, digite 1, Caso contrário, digite qualquer outro número: _
<br>

Acesso ao fórum (chat) (pressionando 1 novamente): – cada livro possui seu próprio chat, um espaço de convívio e compartilhamento de conhecimento acerca do livro

Digite seu username para entrar no fórum: [seu_nickname]
<br>

**Exemplo de fórum:** <br>

FORUM DO LIVRO: [Número de Chamada]

[Data] [Nome do Usuário]: [Mensagem anterior]

[Data] [Nome do Usuário]: [Mensagem anterior]

...

Você está online, digite quit para sair~~


**Comandos no chat:** <br>
    • Digite normalmente para enviar mensagens
    • Use quit para sair do fórum

---

Para administradores (servidor)


**Execução:** <br>

./server <br>

**Mensagem:** <br>

Estamos online na porta: 12345 <br>

Esperando em poll()

Nova conexão, file descriptor 1

Cliente [Nome] registrado no indice 1

Recebidos [X] bytes

Inserido: [mensagem] no banco de dados

Encerramento


**Encerramento:** <br>

Pressione Ctrl+C para encerrar o servidor 


---


**Funcionalidades Principais** <br>

1. Autenticação Integrada
    • Alunos: Matrícula + senha do Pergamum
    • Professores: Nome + email institucional (consulta via API UFV)
    • Cookies de sessão: Gerenciamento automático de sessões <br>

2. Busca de Livros
    • Consulta em tempo real ao catálogo Pergamum
    • Exibição de número de chamada
    • Conversão automática de codificação (ISO-8859-1 para UTF-8) <br>

3. Fóruns por Livro
    • Chat em tempo real para cada livro
    • Histórico de mensagens
    • Identificação por número de chamada <br>

4. Gestão de Perfil
    • Alunos: Curso, admissão, CPF, email, débitos
    • Professores: Órgão, departamento, telefone, e-mail
    • Informações obtidas automaticamente das APIs da UFV <br>

5. Sistema de Débitos (Apenas Alunos – pressionando 2 no menu)
    • Consulta de multas pendentes
    • Formatação em Reais (R$) <br>


---


**Componentes do Sistema:** <br>

Estrutura de Arquivos

/
├── Makefile                  (Script de compilação)

├── server.cpp/.h             (Servidor principal)

├── client.cpp/.h             (Cliente principal)

├── usuario.cpp/.h            (Classe base Usuario)

├── aluno.cpp/.h              (Classe Aluno)

├── professor.cpp/.h         (Classe Professor)

├── livro.cpp/.h             (Classe Livro)

├── chat.cpp/.h               (Classe Chat)

├── biblioteca.cpp/.h         (Classe Biblioteca)

├── mensagem/

│   └── database.cpp/.hpp     (Banco de dados SQLite)

├── main_server.cpp           (Ponto de entrada do servidor)

├── main_client.cpp           (Ponto de entrada do cliente)

└── README.md                (Documentação) <br>


---


**Classes Principais:** <br>

Usuario (Classe Base)

- Gerencia autenticação e dados básicos
- Métodos para buscar livros e obter cookie de sessão <br>
    
Aluno (Herda de Usuario)

- Adiciona: curso, admissão, sexo, semestre, CPF
- Método searchDebito() para consultar multas <br>
    
Professor (Herda de Usuario)

- Adiciona: órgão, departamento, telefone
- Autenticação via nome e e-mail institucional <br>
    
Livro

- Armazena: nome e ID (número de chamada)
- Objeto básico para representação de livros <br>
    
Chat

- Gerencia participantes de um fórum
- Lista de pares (ID socket, Usuario) <br>
    
Biblioteca

- “Contêiner” para livros, usuários e chats
- Métodos de busca e adição <br>
    
Server

- Multiplexação com poll()
- Persistência de mensagens em SQLite <br>
    
Client

- Interface de linha de comando
- Threads separadas para envio/recebimento <br>


---


**Banco de Dados:** <br>

Localização
O banco SQLite é criado automaticamente pelo código em mensagem/database.cpp. <br>

Operações:
- Inserção: Ao receber mensagem no chat
- Consulta: Ao entrar no fórum (histórico)
- Escape SQL: Função escapeSql() previne injeção <br>
    

**⚠️⚠️ Tratamento de Erros: ⚠️⚠️** <br>

Exemplo de exceção lançada:


class Saiu_do_chat : public std::runtime_error
// Lançada quando usuário desconecta


**Recuperação de Erros:** <br>
    1. Erro de conexão: Reconexão automática não implementada
    2. Erro de autenticação: Retorna ao prompt de login
    3. Erro de servidor: Cliente é encerrado
    4. Timeout: Servidor usa timeout de 5 minutos no poll()
    

**Logs:** <br>
    • Console do servidor mostra todas as operações
    • Erros são escritos em std::cerr


---


Notas de Desenvolvimento (dev notes): <br>


**Dependências Externas** <br>
    • cURL: Requisições HTTP para Pergamum e APIs UFV
    • SQLite3: Armazenamento persistente de mensagens
    • nlohmann/json: Parsing de respostas JSON  <br>

  
**Codificação** <br>
    • C++: Utiliza features modernas
    • Polimorfismo: Usuario como classe base
    • RAII: Gerenciamento automático de recursos
    • Multithreading: Comunicação assíncrona cliente/servidor <br>


---


**Licença e Atribuições** <br>

O sistema foi baseado no Pergamum UFV (APIs públicas da Universidade Federal de Viçosa). Desenvolvido como projeto acadêmico <br>


**Uso Acadêmico** <br>

Este software é destinado para fins educacionais. 
Distribuído sob a licença MIT. Sinta-se livre para utilizar e contribuir para o projeto. <br>



































```
sudo apt install libcurl4-openssl-dev
sudo apt install build-essential
sudo apt install libsqlite3-dev
```

## Licença

Distribuído sob a licença MIT. Sinta-se livre para utilizar e contribuir para o projeto.
