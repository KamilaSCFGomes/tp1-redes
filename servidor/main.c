#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#include "servidor.h"
int TAM_BUFFER = 129;

int main(){

    // INICIANDO O SERVIDOR EM UMA PORTA
    printf("O IP do servidor é:\n");
    system("hostname -I");
    int socketServidor = iniciaServidor(8000, 5, 3);
    if(socketServidor<0) return -2;
   
    while(1){

        // CONECTANDO UM CLIENTE
        int socketCliente = conectaCliente(socketServidor);

        char buffer[TAM_BUFFER];
        while (1){

            // LIMPAR BUFFER
            memset(buffer, 0, TAM_BUFFER);
            
            // RECEBER MENSAGEM
            int bytesReceb = recv(socketCliente, buffer, TAM_BUFFER, 0);

            if(bytesReceb == -1){
                printf("Problema de conexão\n");
                break;
            } else if(bytesReceb == 0){
                printf("Cliente desconectado\n");
                break;
            }

            // MOSTRAR MENSAGEM
            printf("Recebido: %s", buffer);

            // REENVIAR MENSAGEM
            send(socketCliente, buffer, bytesReceb+1, 0);
        }

        close(socketCliente);
    }
    return 0;
}