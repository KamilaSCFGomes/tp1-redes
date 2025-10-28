#include "http.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

// not found
void resp404(int clientSocket){
    const char *msg =
        "HTTP/1.0 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>404 - Not Found</h1></body></html>";

    write(clientSocket, msg, strlen(msg));
}

// bad request
void resp400(int clientSocket){
    const char *msg =
        "HTTP/1.0 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>400 - Bad Request</h1></body></html>";

    write(clientSocket, msg, strlen(msg));
}

// unauthorized
void resp401(int clientSocket){
    const char *msg =
        "HTTP/1.0 401 Unauthorized\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>401 - Unauthorized</h1></body></html>";

    write(clientSocket, msg, strlen(msg));
}

// lista os arquivos do diretorio e envia para o cliente
int enviaLista(int clientSocket, const char* caminho){
    DIR *dir = opendir(caminho);
    if(!dir){
        resp404(clientSocket);
        return 404;
    }

    const char *header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";
    write(clientSocket, header, strlen(header));

    write(clientSocket, "<html><body><h1>Index:</h1><ul>\n", 32);

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        char line[512];
        snprintf(line, sizeof(line), "<li><a href=\"%s\">%s</a></li>\n",
                 entry->d_name, entry->d_name);
        write(clientSocket, line, strlen(line));
    }

    write(clientSocket, "</ul></body></html>", 20);
    closedir(dir);
    return 200;
}

// envia um o arquivo especificado para o cliente
int enviaArquivo(int clientSocket, const char* caminho, int tamBuffer){
    int fd = open(caminho, O_RDONLY);
    if(fd < 0){
        resp404(clientSocket);
        return 404;
    }

    struct stat st;
    fstat(fd, &st);

    char header[256];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %ld\r\n"
             "Content-Type: text/html\r\n"
             "\r\n", st.st_size);
    write(clientSocket, header, strlen(header));

    char buffer[tamBuffer];
    ssize_t bytes;
    while((bytes = read(fd, buffer, sizeof(buffer))) > 0){
        write(clientSocket, buffer, bytes);
    }

    close(fd);
    return 200;
}

// Recebe uma requisição, processa e responde o cliente
int processaRequisicao(int clientSocket, char* request, const char* diretorio, int tamBuffer){

    // separa metodo e parametro
    char metodo[8] = "";
    memset(metodo, 0, strlen(metodo));
    char parametro[strlen(request)];
    memset(parametro, 0, strlen(parametro));
    sscanf(request, "%s %s", metodo, parametro);
    
    // confere se o metodo é get
    if(strcmp(metodo, "GET")!=0){
        resp400(clientSocket); // bad request
        return 400;
    }
    // evita sair da pasta com ..
    if(strstr(parametro, "..")!=NULL){
        resp401(clientSocket); // nao autorizado
        return 401;
    }

    // monta o caminho do arquivo
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s%s", diretorio, parametro);

    // verifica se o caminho existe
    struct stat buffer;
    int status = stat(caminho, &buffer);
    if(status != 0){
        resp404(clientSocket); // not found
        return 404;
    }

    // verifica se o caminho é um diretorio
    if(S_ISDIR(buffer.st_mode)){

        // verifica se o diretorio tem um index
        char caminhoIndex[512];
        snprintf(caminhoIndex, sizeof(caminhoIndex), "%s/index.html", caminho);

        if(stat(caminhoIndex, &buffer) == 0){ // se sim, envia o index
            return enviaArquivo(clientSocket, caminhoIndex, tamBuffer);
        }
        // se nao tem index, monta e envia a lista
        return enviaLista(clientSocket, caminho);

    } // se nao for um diretorio, envia o arquivo
    return enviaArquivo(clientSocket, caminho, tamBuffer);
}