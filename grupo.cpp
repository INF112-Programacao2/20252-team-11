//
// Created by pedro on 11/11/2025.
//
#include <bits/stdc++.h>
#include "grupo.h"

//Construtor que inicializa atributos
Grupo::Grupo(std::string titulo):
	titulo(titulo){
	Chat();
}

//Destrutor
Grupo::~Grupo(){
	this->~Chat();
}

//Get que retorna o atributo titulo do grupo
std::string Grupo::getTitulo(){
	return this->titulo;

}
