#include "http_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

int parseURL(const char *url, char *host, int *porta, char *path) {
    if (!url || !host || !porta || !path) return -1;

    const char *ptr = url;

    // verifica e pula "http://"
    if (strncmp(ptr, "http://", 7) == 0) {
        ptr += 7;
    } else {
        return -1; // só HTTP suportado
    }

    *porta = 80; // porta padrão

    // procura o início do path
    const char *slash = strchr(ptr, '/');
    if (!slash) {
        strcpy(host, ptr);
        strcpy(path, "/");
    } else {
        strncpy(host, ptr, slash - ptr);
        host[slash - ptr] = '\0';
        strcpy(path, slash);
    }

    // verifica se host contém ":porta"
    char *pPorta = strchr(host, ':');
    if (pPorta) {
        *pPorta = '\0';               // separa host da porta
        *porta = atoi(pPorta + 1);    // converte porta
    }

    return 0;
}

// lê resposta do servidor e salva arquivo
int processaRespostaHTTP(int socket, const char *url, char *novoLocal, const char *pastaDestino) {
    char buffer[4096];
    int bytesRecebidos = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRecebidos <= 0) return -1;
    buffer[bytesRecebidos] = '\0';

    char *headerEnd = strstr(buffer, "\r\n\r\n");
    if (!headerEnd) return -1;

    size_t headerLen = headerEnd - buffer + 4;
    char cabecalho[headerLen + 1];
    strncpy(cabecalho, buffer, headerLen);
    cabecalho[headerLen] = '\0';

    int statusCode = 0;
    sscanf(cabecalho, "HTTP/%*s %d", &statusCode);

    // verifica redirecionamento
    if (statusCode == 301 || statusCode == 302) {
        char *loc = strcasestr(cabecalho, "Location:");
        if (loc) {
            loc += 9;
            while (*loc == ' ') loc++;
            char *fim = strstr(loc, "\r\n");
            if (fim) *fim = '\0';
            strncpy(novoLocal, loc, 511);
            novoLocal[511] = '\0';
            return 301;
        }
        return -1;
    }

    if (statusCode != 200) return statusCode;

    // obtém Content-Length
    size_t contentLength = 0;
    char *cl = strcasestr(cabecalho, "Content-Length:");
    if (cl) {
        cl += 15;
        while (*cl == ' ') cl++;
        contentLength = strtoul(cl, NULL, 10);
    }

    // determina nome do arquivo
    const char *nomeArquivo = strrchr(url, '/');
    if (!nomeArquivo || strlen(nomeArquivo) <= 1) nomeArquivo = "index.html";
    else nomeArquivo++;

    mkdir(pastaDestino, 0777);
    char caminhoDestino[512];
    snprintf(caminhoDestino, sizeof(caminhoDestino), "%s/%s", pastaDestino, nomeArquivo);

    int fd = open(caminhoDestino, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd < 0) return -1;

    // escreve corpo já recebido
    size_t bodyLen = bytesRecebidos - headerLen;
    write(fd, headerEnd + 4, bodyLen);

    // lê o restante do corpo, se houver
    size_t totalLido = bodyLen;
    while (totalLido < contentLength) {
        ssize_t lidos = recv(socket, buffer, sizeof(buffer), 0);
        if (lidos <= 0) break;
        write(fd, buffer, lidos);
        totalLido += lidos;
    }

    close(fd);
    printf("Arquivo salvo em: %s\n", caminhoDestino);
    return 200;
}

// monta e envia requisição GET simples
int enviaRequisicaoGET(int sock, const char *host, const char *path) {
    char req[1024];
    snprintf(req, sizeof(req),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: SimpleClient/1.0\r\n"
             "Connection: close\r\n\r\n",
             path, host);

    ssize_t enviado = send(sock, req, strlen(req), 0);
    if (enviado < 0) {
        perror("Erro ao enviar requisição");
        return -1;
    }

    return 0;
}