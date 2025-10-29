#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include "conexao.h"

int conectaServidor(const char* host, int porta) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("Erro ao criar socket"); return -1; }

    struct hostent* servidor = gethostbyname(host);
    if (!servidor) { fprintf(stderr,"Host inválido: %s\n",host); return -1; }

    struct sockaddr_in endereco;
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family = AF_INET;
    endereco.sin_port = htons(porta);
    memcpy(&endereco.sin_addr, servidor->h_addr_list[0], servidor->h_length);

    if (connect(sock, (struct sockaddr*)&endereco, sizeof(endereco)) < 0) {
        perror("Erro ao conectar");
        return -1;
    }

    printf("Conectado ao servidor %s:%d\n", host, porta);
    return sock;
}

int enviaRequisicao(int sock, const char* caminho, const char* host) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             caminho, host);

    send(sock, buffer, strlen(buffer), 0);
    printf("Requisição enviada:\n%s\n", buffer);
    return 0;
}