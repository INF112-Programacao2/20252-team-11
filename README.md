# 📘 Projeto INF112 — User Stories & Cartões CRC

Este repositório contém a documentação de **User Stories** e **Cartões CRC** desenvolvidos pelo **Grupo 11** para a disciplina **INF112**, com foco em funcionalidades integradas a sistemas utilizados na **UFV** (Pergamum, RU e Moodle).

---

## 👥 Integrantes do Grupo 11

| Nome | Matrícula |
|------|------------|
| **Gabriel Spagnol Gomes** | 120571 |
| **Arthur Mendes Vilela** | 120570 |
| **Pedro Silva Francisco** | 120554 |
| **João Vitor Miranda Marcelino** | 113477 |
| **Marco Thullyo São Severino** | 120550 |

---

## 🧩 Escopo do Projeto

O objetivo do projeto é descrever, através de **User Stories** e **Cartões CRC**, as funcionalidades desejadas para um sistema integrado que una informações e serviços da **UFV** — Biblioteca (Pergamum), Restaurante Universitário (RU) e Moodle — em uma única plataforma acessível para alunos e servidores.

---

## 📋 User Stories

### 🔹 Pergamum
1. **Busca de livros** — Permite procurar livros por título, autor, ISBN, número de chamado ou categoria.  
2. **Salvar livro como favorito** — Usuário pode favoritar livros para acesso rápido posterior.  
3. **Exibir livros favoritos** — Lista e detalha livros marcados como favoritos.  
4. **Entrar pelo Pergamum** — Login com matrícula e senha UFV.  
5. **Consulta de débitos** — Exibe livros e valores devidos pelo usuário.  
6. **Renovação de empréstimo** — Permite renovar livros com base no tipo de usuário (aluno/professor).

---

### 🔹 RU
7. **Consulta de cardápio** — Exibe o cardápio mensal e diário do Restaurante Universitário.  
8. **Análise de cardápio** — Mostra informações nutricionais e alertas sobre alergênicos, com análise por IA.

---

### 🔹 Moodle
9. **Consulta de informações acadêmicas** — Exibe disciplinas cursadas por alunos e alunos matriculados em cada disciplina (para professores).

---

### 🔹 Funcionalidades Gerais
10. **Consulta a informações de servidores** — Busca por nome, órgão ou departamento.  
11. **Consulta ao histórico de disciplinas** — Mostra professores e turmas anteriores.  
12. **Comunicação com servidores** — Envio de mensagens ou e-mails automáticos.  
13. **Consulta a informações de alunos** — Busca por nome, curso ou ano de entrada.  
14. **Adicionar amigo** — Permite criar e remover conexões entre usuários autenticados.  
15. **Chat** — Sistema de mensagens entre alunos e servidores.  
16. **Fórum de livros** — Discussões públicas sobre livros do acervo.  
17. **Fórum de cardápios** — Discussões sobre cardápios diários do RU.

---

## 🧠 Cartões CRC

| Classe | Responsabilidades | Colaborações |
|--------|-------------------|--------------|
| **Usuário (Servidores e Alunos)** | Enviar mensagens, gerenciar amizades e interações em fóruns. | Biblioteca, Cardápio, Disciplina |
| **Biblioteca** | Buscar livros, gerenciar favoritos, débitos e renovações. | Usuário |
| **Cardápio** | Exibir dados do RU e análises nutricionais, gerenciar fóruns de cardápios. | Usuário |
| **Disciplina** | Exibir professores, alunos e histórico de disciplinas. | Servidores, Alunos |

---


---

## 🚀 Tecnologias (Sugestões Futuras)

O projeto poderá futuramente ser implementado com:
- **Frontend:** React / Vue.js  
- **Backend:** Node.js / Python (FastAPI ou Flask)  
- **Banco de Dados:** PostgreSQL / Firebase  
- **Integrações:** APIs da UFV, Pergamum, RU e Moodle  

---

## 🏁 Status do Projeto

📄 **Fase atual:** Definição de requisitos e modelagem (User Stories e CRC).  
🛠️ **Próximos passos:** Protótipo funcional e integração de módulos.

---

## 📜 Licença

Este projeto é acadêmico e não possui fins comerciais.  
© 2025 — **Grupo 11 | INF112 — UFV**


## 🧾 Estrutura do Repositório

