#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int salvaArquivo(const char *pastaDestino, const char *nomeArquivo, const char *dados, size_t tamanho) {
    char caminho[512];
    snprintf(caminho, sizeof(caminho), "%s/%s", pastaDestino, nomeArquivo);

    // Cria pasta se não existir
    mkdir(pastaDestino, 0777);

    int fd = open(caminho, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Erro ao criar arquivo");
        return -1;
    }

    write(fd, dados, tamanho);
    close(fd);

    return 0;
}