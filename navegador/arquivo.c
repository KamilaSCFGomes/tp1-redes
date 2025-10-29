#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "arquivo.h"

void salvaArquivo(const char* nomeArquivo, const char* dados, int tamanho, int primeiraParte) {
    mkdir("arquivos_baixados", 0777); // cria pasta se não existir
    char caminhoFinal[256];
    snprintf(caminhoFinal, sizeof(caminhoFinal), "arquivos_baixados/%s", nomeArquivo);

    FILE* f = fopen(caminhoFinal, primeiraParte ? "wb" : "ab");
    if (!f) { perror("Erro ao criar arquivo"); return; }

    fwrite(dados, 1, tamanho, f);
    fclose(f);
}