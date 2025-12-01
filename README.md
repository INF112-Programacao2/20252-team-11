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

## Visão Geral

**BetterPergamum** é um projeto acadêmico desenvolvido para a disciplina **INF112** da **Universidade Federal de Viçosa (UFV)**. O sistema unifica funcionalidades inspiradas no Pergamum (sistema de bibliotecas) com mecanismos de interação entre alunos e professores, criando uma plataforma integrada e inteligente para comunicação e organização de estudos.

O objetivo principal é **facilitar a conexão entre estudantes**, permitindo a formação de grupos de estudo com base em perfis acadêmicos semelhantes — tudo isso utilizando informações fornecidas pelo próprio sistema da biblioteca.

---

## Funcionalidades

O sistema realiza autenticação segura utilizando apenas o **número de matrícula** e a **senha da biblioteca**. A partir disso, realiza engenharia reversa para coletar informações públicas, apresentando ao usuário dados como:

### Informações do Usuário
Perfil Básico do Usuário
| Campo | Dado Fornecido |
|---|---|
| Nome Completo | Pergamum |
| E-mail Institucional | Pergamum |
| CPF | Pergamum |

### Usuário for Aluno

| Campo | Dado Acadêmico |
|---|---|
| Curso | Identificação do curso atual |
| Período de Admissão | Ano/semestre de ingresso na UFV |
| Gênero | Sistemas da UFV |

### Usuário for Professor

| Campo | Dado Institucional |
|---|---|
| Órgão | Departamento ou unidade de lotação |
| Departamento | Setor de atuação principal |
| Telefone de Contato | Número de contato profissional |

> [Obs.: Nenhum dado sensível ou credencial é armazenado em banco de dados ou no código. Informações são usadas apenas em memória e descartadas ao final da execução.]

---

## Objetivo do Projeto

O sistema busca:

* Integrar dados acadêmicos ao processo de organização de estudos;
* Promover grupos de estudo baseados em perfis reais de usuários;
* Facilitar a comunicação entre alunos e professores;
* Unir diferentes setores da universidade em uma única ferramenta acessível.

---

## Arquitetura Geral

* Coleta de dados via engenharia reversa do Pergamum
* Processamento interno das informações de usuários
* Agrupamento automático por similaridade acadêmica
* Interface simples e integrada com as funcionalidades essenciais

---

## Integrantes do Grupo 11

| Nome                             | Matrícula |
| -------------------------------- | --------- |
| **Gabriel Spagnol Gomes**        | 120571    |
| **Arthur Mendes Vilela**         | 120570    |
| **Pedro Silva Francisco**        | 120554    |
| **João Vitor Miranda Marcelino** | 113477    |
| **Marco Thullyo São Severino**   | 120550    |

---

## Licença

Distribuído sob a licença MIT. Sinta-se livre para utilizar e contribuir para o projeto.
