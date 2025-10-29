#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conexao.h"
#include "http_client.h"
#include "arquivo.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s http://<host>:<porta>/<arquivo>\n", argv[0]);
        return 1;
    }

    char host[128], caminho[128];
    int porta = 80;

    // Parse simples da URL
    if (sscanf(argv[1], "http://%127[^:/]:%d/%127[^\n]", host, &porta, caminho) == 3) {
        // ok
    } else if (sscanf(argv[1], "http://%127[^/]/%127[^\n]", host, caminho) == 2) {
        porta = 80;
    } else if (sscanf(argv[1], "http://%127[^:/]:%d", host, &porta) == 2) {
        strcpy(caminho, "");
    } else if (sscanf(argv[1], "http://%127[^/]", host) == 1) {
        strcpy(caminho, "");
    } else {
        fprintf(stderr, "URL inválida.\n");
        return 1;
    }

    char caminhoFinal[256];
    snprintf(caminhoFinal, sizeof(caminhoFinal), "/%s", caminho);

    int sock = conectaServidor(host, porta);
    if (sock < 0) return 1;

    enviaRequisicao(sock, caminhoFinal, host);
    recebeResposta(sock, caminhoFinal);

    close(sock);
    return 0;
}