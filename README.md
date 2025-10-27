# Trabalho Prático 1 - Navegador e Servidor HTTP
Kamila S de C F Gomes - 2023008166

Este é um trabalho prático desenvolvido em outubro de 2025 como parte do critério avaliativo da discliplina de Redes de Computadores, ministrada pelo Prof. Flávio Luiz Schiavoni.

O trabalho foi desenvolvido para máquinas Linux, utilizando a linguagem C.

## Especificações do Trabalho
### Parte 1 - Cliente HTTP

Criar um cliente HTTP, ou navegador, que acesse sites e salve os arquivos recebidos. O navegador pode funcionar em modo texto, com um comando similar ao apresentado:

```
meu_navegador HTTP://www.ufsj.edu.br/teste/imagem.jpg
meu_navegador HTTP://localhost:5050/imagem.pdf
```

O mesmo deverá salvar o arquivo, caso o mesmo esteja disponível, ou retornar erro.

### Parte 2 - Servidor HTTP

Criar um servidor HTTP, ou sevidor de página, que sirva os arquivos de um determinado diretório. Caso o diretório tenha um arquivo chamado index.html, deverá retornar o arquivo. Caso não tenha deverá retornar uma listagem com os arquivos desta pasta. Exemplo:

```
meu_servidor /home/flavio/meusite
```

# Estrutura
Esse trabalho foi desenvolvido em duas partes: Um servidor HTTP, e um navegador HTTP.

Cada projeto possui sua respectiva pasta com os arquivos correspondentes, e há um Makefile mestre na pasta mãe para promover praticidade ao compilar arquivos de diferentes pastas.

Cada projeto possui sua main e módulos separados para melhorar a legibilidade do código.

```
trabalho_redes/
├── cliente/
│   ├── main.c
│   ├── cliente.c
│   ├── cliente.h
│   └── Makefile
├── servidor/
│   ├── main.c
│   ├── servidor.c
│   ├── servidor.h
│   └── Makefile
└── Makefile
```

# Instalação

Certifique-se de que todas as bibliotecas necessárias estão instaladas.
Depois, compile o código utilizando makefile.

## Dependências

## Compilação

Cada projeto possui um Makefile próprio e a pasta mãe possui um Makefile mestre que os coordena, para promover praticidade.

Para compilar um dos projetos, execute um comando make contendo o nome do trabalho desejado na pasta mãe.

``` bash
make servidor
make navegador
```

Se desejar compilar ambos os projetos ao mesmo tempo, apenas execute um comando make simples.

``` bash
make
```

Os executáveis serão gerados na pasta mãe, enquanto os objetos permanecerão nas pastas de seus respectivos projetos.

```
trabalho_redes/
├── cliente/
│   ├── main.c
│   ├── cliente.c
│   ├── cliente.h
│   ├── cliente.o
│   └── Makefile
├── servidor/
│   ├── main.c
│   ├── main.o
│   ├── servidor.c
│   ├── servidor.h
│   ├── servidor.o
│   └── Makefile
├── boli_servidor
├── boli_cliente
└── Makefile
```

Para limpar os arquivos gerados, execute um comando make clean na pasta mãe.

``` bash
make clean
```

## Personalização


# Utilização

## Navegador

## Servidor
Após compilar, execute o arquivo ```boli_servidor```, especificando o diretório a ser utilizado pelo servidor.

``` bash
./boli_servidor [diretório]
```

O programa verificará a existência do diretório, e então tentará criar um socket e iniciar o servidor, podendo utilizar outros sockets caso o especificado não seja possível utilizar o socket especificado. Ele informará no terminal qual foi o socket utilizado.

``` bash
a
```

Utilize o socket especificado para se conectar ao servidor. Isso pode ser feito utilizando telnet

``` bash
telnet localhost 8000
```

O servidor avisará que um cliente foi conectado, informando seu nome e porta utilizada.

``` bash
conectado
```

O servidor apenas atende a requisições ```GET /[arquivo]```.
