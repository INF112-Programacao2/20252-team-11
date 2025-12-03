//
// Created by pedro on 11/11/2025.
//

#ifndef GRUPO_H
#define GRUPO_H
#include "chat.h"
#include <bits/stdc++.h>

class Grupo : public Chat { //Classe grupo herda de chat
	private:
	//nome do grupo
		std::string titulo;
	public:
	//construtor e destutor
		Grupo(std::string titulo);
		~Grupo();

	//retorna titulo
		std::string getTitulo();
};

#endif //GRUPO_H

