#ifndef HTTP_SERVIDOR_H
#define HTTP_SERVIDOR_H

// Recebe uma requisição, processa e responde o cliente
int processaRequisicao(int clientSocket, char* request, const char* diretorio, int tamBuffer);

#endif