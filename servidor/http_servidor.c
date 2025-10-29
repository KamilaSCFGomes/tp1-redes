#include "http_servidor.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// not found
void resp404(int clientSocket) {
    const char *body = "<html><body><h1>404 - Not Found</h1></body></html>";
    char header[256];
    snprintf(header, sizeof(header),
        "HTTP/1.0 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n", strlen(body));

    write(clientSocket, header, strlen(header));
    write(clientSocket, body, strlen(body));
}

// bad request
void resp400(int clientSocket) {
    const char *body = "<html><body><h1>400 - Bad Request</h1></body></html>";
    char header[256];
    snprintf(header, sizeof(header),
        "HTTP/1.0 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n", strlen(body));

    write(clientSocket, header, strlen(header));
    write(clientSocket, body, strlen(body));
}

// unauthorized
void resp401(int clientSocket) {
    const char *body = "<html><body><h1>401 - Unauthorized</h1></body></html>";
    char header[256];
    snprintf(header, sizeof(header),
        "HTTP/1.0 401 Unauthorized\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n", strlen(body));

    write(clientSocket, header, strlen(header));
    write(clientSocket, body, strlen(body));
}

// redireciona o endereco com / no final
void redirecionaBarra(int clientSocket, const char *parametro) {
    char novoLocal[512];
    snprintf(novoLocal, sizeof(novoLocal), "%s/", parametro);

    char resposta[1024];
    snprintf(resposta, sizeof(resposta),
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Location: %s\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n", novoLocal);

    send(clientSocket, resposta, strlen(resposta), 0);
}

// lista os arquivos do diretorio e envia para o cliente
int enviaLista(int clientSocket, const char* caminho, const char* parametro) {
    DIR *dir = opendir(caminho);
    if (!dir) { resp404(clientSocket); return 404; }

    // Normaliza prefixo para os hrefs:
    char prefix[512];
    if (!parametro || strlen(parametro) == 0) strcpy(prefix, "/");
    else {
        // garante que começa com / e termina com /
        if (parametro[0] != '/') {
            snprintf(prefix, sizeof(prefix), "/%s", parametro);
        } else {
            strncpy(prefix, parametro, sizeof(prefix)-1);
            prefix[sizeof(prefix)-1] = '\0';
        }
        size_t plen = strlen(prefix);
        if (prefix[plen-1] != '/') {
            // adiciona barra final
            if (plen + 1 < sizeof(prefix)) strcat(prefix, "/");
        }
    }

    // montar corpo em buffer
    char corpo[8192];
    int off = snprintf(corpo, sizeof(corpo),
                       "<html><body><h1>Index of %s</h1><ul>", prefix);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && off < (int)sizeof(corpo) - 256) {
        // ignora '.' e '..'
        if (strcmp(entry->d_name, ".") == 0) continue;
        // monta href completo
        off += snprintf(corpo + off, sizeof(corpo) - off,
                        "<li><a href=\"%s%s\">%s</a></li>",
                        prefix, entry->d_name, entry->d_name);
    }

    off += snprintf(corpo + off, sizeof(corpo) - off, "</ul></body></html>");
    closedir(dir);

    char header[256];
    int hlen = snprintf(header, sizeof(header),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %d\r\n"
                        "Connection: close\r\n"
                        "\r\n", off);
    write(clientSocket, header, hlen);
    write(clientSocket, corpo, off);
    return 200;
}

const char* getMimeType(const char* caminho) {
    const char *ext = strrchr(caminho, '.');
    if (!ext) return "application/octet-stream"; // tipo genérico

    ext++; // pula o ponto

    if (strcasecmp(ext, "html") == 0 || strcasecmp(ext, "htm") == 0) return "text/html";
    if (strcasecmp(ext, "css") == 0)        return "text/css";
    if (strcasecmp(ext, "js") == 0)         return "application/javascript";
    if (strcasecmp(ext, "json") == 0)       return "application/json";
    if (strcasecmp(ext, "png") == 0)        return "image/png";
    if (strcasecmp(ext, "jpg") == 0 || strcasecmp(ext, "jpeg") == 0) return "image/jpeg";
    if (strcasecmp(ext, "gif") == 0)        return "image/gif";
    if (strcasecmp(ext, "svg") == 0)        return "image/svg+xml";
    if (strcasecmp(ext, "ico") == 0)        return "image/x-icon";
    if (strcasecmp(ext, "mp4") == 0)        return "video/mp4";
    if (strcasecmp(ext, "webm") == 0)       return "video/webm";
    if (strcasecmp(ext, "ogg") == 0)        return "video/ogg";
    if (strcasecmp(ext, "mp3") == 0)        return "audio/mpeg";
    if (strcasecmp(ext, "wav") == 0)        return "audio/wav";
    if (strcasecmp(ext, "txt") == 0)        return "text/plain";

    return "application/octet-stream"; // genérico
}


// envia um o arquivo especificado para o cliente
int enviaArquivo(int clientSocket, const char* caminho, int tamBuffer) {
    int fd = open(caminho, O_RDONLY);
    if (fd < 0) {
        resp404(clientSocket);
        return 404;
    }

    struct stat st;
    fstat(fd, &st);

    char header[256];
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: %ld\r\n"
        "Content-Type: %s\r\n"
        "\r\n", st.st_size, getMimeType(caminho));
    write(clientSocket, header, strlen(header));

    char buffer[tamBuffer];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
        write(clientSocket, buffer, bytes);
    }

    close(fd);
    return 200;
}


// Recebe uma requisição, processa e responde o cliente
int processaRequisicao(int clientSocket, char* request, const char* diretorio, int tamBuffer) {
    // separa metodo e parametro
    char metodo[8] = "";
    char parametro[512] = "";
    sscanf(request, "%7s %511s", metodo, parametro);
    
    // confere se o metodo é GET
    if (strcmp(metodo, "GET") != 0) {
        resp400(clientSocket); // bad request
        return 400;
    }

    // evita sair da pasta com ..
    if (strstr(parametro, "..") != NULL) {
        resp401(clientSocket); // nao autorizado
        return 401;
    }

    // monta o caminho do arquivo
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s%s", diretorio, parametro);

    // verifica se o caminho existe
    struct stat buffer;
    if (stat(caminho, &buffer) != 0) {
        resp404(clientSocket); // not found
        return 404;
    }

    // verifica se o caminho é um diretório
    if (S_ISDIR(buffer.st_mode)) {
        size_t len = strlen(parametro);

        // se o caminho não termina com "/", redireciona com barra
        if (len == 0 || parametro[len - 1] != '/') {
            redirecionaBarra(clientSocket, parametro);
            return 301;
        }

        // tenta servir index.html dentro do diretório
        char caminhoIndex[512];
        snprintf(caminhoIndex, sizeof(caminhoIndex), "%s/index.html", caminho);

        if (stat(caminhoIndex, &buffer) == 0) {
            return enviaArquivo(clientSocket, caminhoIndex, tamBuffer);
        }

        // se não houver index, gera e envia listagem
        return enviaLista(clientSocket, caminho, parametro);
    }

    // caso contrário, envia o arquivo diretamente
    return enviaArquivo(clientSocket, caminho, tamBuffer);
}