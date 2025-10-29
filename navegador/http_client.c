#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "http_client.h"

#define TAM_BUFFER 1024

int recebeResposta(int sock, const char* caminhoOriginal) {
    char buffer[TAM_BUFFER];
    int bytesRecebidos;
    int encontrouCabecalho = 0;
    int primeiraParte = 1;

    const char* nomeArquivo = strrchr(caminhoOriginal,'/');
    if (!nomeArquivo || strlen(nomeArquivo)<=1)
        nomeArquivo = "index.html";
    else
        nomeArquivo++; // pula '/'

    while ((bytesRecebidos = recv(sock, buffer, TAM_BUFFER, 0)) > 0) {
        if (!encontrouCabecalho) {
            char* fimCabecalho = strstr(buffer, "\r\n\r\n");
            if (fimCabecalho) {
                encontrouCabecalho = 1;
                int pos = fimCabecalho - buffer + 4;
                int tamanhoCorpo = bytesRecebidos - pos;
                salvaArquivo(nomeArquivo, buffer + pos, tamanhoCorpo, primeiraParte);
                primeiraParte = 0;
            }
        } else {
            salvaArquivo(nomeArquivo, buffer, bytesRecebidos, primeiraParte);
            primeiraParte = 0;
        }
    }

    printf("Arquivo salvo em arquivos_baixados/%s\n", nomeArquivo);
    return 0;
}