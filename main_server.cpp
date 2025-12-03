//
// Created by pedro on 11/11/2025.
//
#include <bits/stdc++.h>
#include "server.h"

int main(){
	Server chat = Server(12345);	//instancia do servidor na porta 12345
	chat.listen_socket();			//configura socket e coloca em modo de escuta
	chat.run();						//inicia loop principal do servidor
	chat.close();					//fecha servidor 
	return 0;

}
