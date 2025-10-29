#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stddef.h>

// Salva os dados em um arquivo dentro da pasta especificada
int salvaArquivo(const char *pastaDestino, const char *nomeArquivo, const char *dados, size_t tamanho);

#endif