//
// Created by pedro on 11/11/2025.
//

#ifndef GRUPO_H
#define GRUPO_H
#include "chat.h"
#include <bits/stdc++.h>

class Grupo : public Chat {
	private:
		std::string titulo;
	public:
		Grupo(std::string titulo);
		~Grupo();
		std::string getTitulo();
};

#endif //GRUPO_H
