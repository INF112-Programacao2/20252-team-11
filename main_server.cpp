//
// Created by pedro on 11/11/2025.
//
#include <bits/stdc++.h>
#include "server.h"

int main(){
	Server chat = Server(12345);
	chat.listen_socket();
	chat.run();
	chat.close();
	return 0;
}