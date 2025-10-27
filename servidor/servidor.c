#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#include "servidor.h"

// INICIA O SERVIDOR EM UMA PORTA ESPECIFICA E RETORNA O SOCKET CRIADO. PODE REALIZAR MAIS TENTATIVAS CASO A PORTA ESTEJA OCUPADA
int iniciaServidor(int porta, int maxTentativas, int numConexoes){
     // CRIANDO O SOCKET
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1){
        printf("Não foi possível criar o socket\n");
        return -1;
    } else {
        printf("Socket criado\n");
    }

    // CONECTANDO SOCKET A UM IP / PORTA
    struct sockaddr_in serverAddr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port        = htons(porta)
    };
    
    int resulTentativaSocket;
    for(int i=0; i<maxTentativas; i++){
        resulTentativaSocket = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof serverAddr);
        if(resulTentativaSocket == 0) break;
        printf("Não foi possível conectar o socket à porta %d\n", porta);
        porta++;
        serverAddr.sin_port = htons(porta);
    }
    if(resulTentativaSocket < 0) {
        printf("%d tentativas de conectar o socket sem sucesso\n",maxTentativas);
        return -2;
    }

    // MARCANDO O SOCKET COMO OUVINTE
    if(listen(serverSocket, numConexoes) == -1){
        printf("O servidor não consegue ouvir\n");
        return -3;
    } else {
        printf("O servidor está ouvindo a porta %d\n",porta);
    }
    return serverSocket;
}

// CONECTA UM CLIENTE AO SERVIDOR E RETORNA O SOCKET DO CLIENTE
int conectaCliente(int serverSocket){
    struct sockaddr_in clientAddr;
    int csize  = sizeof clientAddr;

    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &csize);
    if(clientSocket == -1){
        printf("Um cliente não conseguiu conectar\n");
        return -1;
    }

    // CLIENTE CONECTADO. VERIFICANDO QUEM É
    char host[256];
    char service[256];
    
    memset(host, 0, 256);
    memset(service, 0, 256);
    
    if(getnameinfo((struct sockaddr *) &clientAddr, csize, host, 256, service, 256, 0) == 0){
        printf("%s conectado na porta %s\n", host, service);
    }
    else
    {
        inet_ntop(AF_INET, &clientAddr.sin_addr, host, 256);
        printf("%s conectado na porta %d\n", host, ntohs(clientAddr.sin_port));
    }

    return clientSocket;
}