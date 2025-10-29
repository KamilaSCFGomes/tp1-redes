#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

int enviaRequisicaoGET(int sock, const char *host, const char *path);
int processaRespostaHTTP(int socket, const char *url, char *novoLocal, const char *pastaDestino);
int parseURL(const char *url, char *host, int *porta, char *path);

#endif
