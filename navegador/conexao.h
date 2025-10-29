#ifndef CONEXAO_H
#define CONEXAO_H

int conectaServidor(const char* host, int porta);
int enviaRequisicao(int sock, const char* caminho, const char* host);

#endif