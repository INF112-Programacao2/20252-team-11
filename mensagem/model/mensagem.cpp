#include "mensagem.hpp"

Mensagem::Mensagem(std::string conteudo, std::time_t dataHora, int autor, std::weak_ptr<Mensagem> respostaPara)
    : conteudo(conteudo), dataHora(dataHora), autor(autor), respostaPara(respostaPara) {}


Mensagem::Mensagem(){}

Mensagem::~Mensagem(){}

std::string Mensagem::getConteudo(){return this->conteudo;}
std::time_t Mensagem::getDataHora(){return this->dataHora;}
int Mensagem::getAutor(){return this->autor;}
std::weak_ptr<Mensagem> Mensagem::getRespostaPara(){return this->respostaPara.lock();}

void Mensagem::setConteudo(std::string conteudo){this->conteudo = conteudo;}
void Mensagem::setRespostaPara(std::weak_ptr<Mensagem> respostaPara){this->respostaPara = respostaPara;}