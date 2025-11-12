//
// Created by pedro on 11/11/2025.
//
#include <bits/stdc++.h>
#include "grupo.h"

Grupo::Grupo(std::string titulo):
	titulo(titulo){
	Chat();
}

Grupo::~Grupo(){
	this->~Chat();
}

std::string Grupo::getTitulo(){
	return this->titulo;
}