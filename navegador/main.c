#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "conexao.h"
#include "http_client.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <URL>\n", argv[0]);
        return 1;
    }

    char url[512];
    strncpy(url, argv[1], sizeof(url)-1);
    url[sizeof(url)-1] = '\0';

    char host[256], path[256];
    int porta;
    if (parseURL(url, host, &porta, path) < 0) {
        fprintf(stderr, "URL inválida.\n");
        return 1;
    }

    int maxRedirs = 5;
    int status;
    char novoLocal[512];
    strncpy(novoLocal, url, sizeof(novoLocal)-1);
    novoLocal[sizeof(novoLocal)-1] = '\0';

    for (int i = 0; i < maxRedirs; i++) {
        if (parseURL(novoLocal, host, &porta, path) < 0) break;

        int sock = conectaServidor(host, porta);
        if (sock < 0) {
            fprintf(stderr, "Erro ao conectar %s:%d\n", host, porta);
            return 1;
        }

        if (enviaRequisicaoGET(sock, host, path) < 0) {
            close(sock);
            return 1;
        }

        status = processaRespostaHTTP(sock, novoLocal, novoLocal, "arquivos_baixados");
        close(sock);
        printf("Resposta: %d\n",status);

        if (status == 301 || status == 302) {
            printf("Seguindo redirecionamento: %s\n", novoLocal);
            continue;
        }
        break; // não é redirecionamento, encerra
    }

    if (status != 200) {
        fprintf(stderr, "Falha ao baixar arquivo. Código HTTP: %d\n", status);
        return 1;
    }

    printf("Download concluído com sucesso!\n");
    return 0;
}
