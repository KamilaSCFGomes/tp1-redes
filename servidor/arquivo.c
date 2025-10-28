#include "arquivo.h"

#include <stdio.h>
#include <sys/stat.h>


int verificaDiretorioValido(int argc, char *argv[]){
    if (argc != 2) {
        printf("Especifique o diretório a ser utilizado: %s [caminho]\n", argv[0]);
        return -1;
    }

    const char *caminho = argv[1];
    struct stat info;

    // verifica se o caminho existe se e é um diretório
    if (stat(caminho, &info) != 0){
        printf("Erro ao acessar o diretório\n");
        return -2;
    } else if(!S_ISDIR(info.st_mode)){
        printf("O caminho informado não é um diretório\n");
        return -3;
    } else {
        printf("Diretório válido: %s\n", caminho);
        return 0;
    }
}