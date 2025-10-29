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
├── navegador/
│   ├── arquivo.c
│   ├── arquivo.h
│   ├── conexao.c
│   ├── conexao.h
│   ├── http_client.c
│   ├── http_client.h
│   ├── main.c
│   └── Makefile
├── servidor/
│   ├── arquivo.c
│   ├── arquivo.h
│   ├── http_servidor.c
│   ├── http_servidor.h
│   ├── main.c
│   ├── servidor.c
│   ├── servidor.h
│   └── Makefile
└── Makefile
```

# Instalação

Certifique-se de que todas as bibliotecas necessárias estão instaladas.
Depois, compile o código utilizando makefile.

## Bibliotecas utilizadas


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

Os executáveis serão gerados na pasta mãe, enquanto os objetos permanecerão nas subpastas de seus respectivos projetos.

Para limpar os arquivos gerados, execute um comando make clean na pasta mãe.

``` bash
make clean
```

# Utilização

## Navegador

Após compilar, execute o arquivo ```boli_navegador```, informando a URL completa do recurso a ser acessado.

``` bash
./boli_navegador [URL]
```

Exemplo de uso:

``` bash
./boli_navegador http://localhost:8000/index.html
```


O programa criará uma conexão com o servidor especificado na URL e enviará uma requisição HTTP do tipo ```GET```. Após receber a resposta, ele analisará o cabeçalho HTTP e salvará o conteúdo do arquivo, caso o mesmo exista.

Os arquivos baixados são armazenados automaticamente na pasta ```arquivos_baixados```, mantendo o mesmo nome e extensão do arquivo original.

Exemplo:

``` bash
./boli_navegador http://localhost:8000/imagens/foto.png
```


resultará em: ```arquivos_baixados/foto.png```


O navegador interpreta as respostas do servidor conforme o protocolo HTTP:

- ```200 - OK ``` -> O arquivo foi encontrado e baixado com sucesso.

- ```301 - Moved Permanently``` -> O navegador seguirá o redirecionamento automaticamente.

- ```400 - Bad request``` -> A requisição enviada é inválida.

- ```401 - Unauthorized``` -> O acesso ao caminho solicitado não é permitido.

- ```404 - Not found``` -> O arquivo ou diretório solicitado não existe.

Após concluir o download do arquivo (ou receber uma resposta de erro), o programa é finalizado automaticamente.

## Servidor
Após compilar, execute o arquivo ```boli_servidor```, especificando o diretório a ser utilizado pelo servidor.

``` bash
./boli_servidor [diretório]
```

O programa verificará a existência do diretório, e então tentará criar um socket e iniciar o servidor, podendo utilizar outros sockets caso não seja possível utilizar o socket especificado. (É possível determinar quantas tentativas podem ser feitas ao chamar a função.) Ele informará no terminal qual foi o socket utilizado.

Utilize o socket especificado e o IP da máquina para se conectar ao servidor. Isso pode ser feito utilizando telnet ou curl, ou acessando o endereço no navegador.

``` bash
telnet localhost 8000
```
```
http://localhost:8000/
```

O servidor avisará que um cliente foi conectado, informando seu nome e porta utilizada, e então está pronto para uso.

O servidor apenas atende a requisições ```GET /[arquivo]``` e responde seguindo o protocolo http:

- ```200 - OK``` -> Sucesso
- ```400 - Bad request``` -> Requisição feita sem utilizar o método ```GET```
- ```401 - Unauthorized``` -> O caminho contém ```..``` (por segurança, para evitar acessar outras pastas)
- ```404 - Not found``` -> O diretório não foi encontrado

Caso o caminho especificado corresponda a um arquivo, ele será enviado, se existir.

Se o caminho corresponder a uma pasta, enviará ```index.html``` se existir. Caso contrário, construirá uma lista do conteúdo da pasta.

Para desconectar o cliente pelo terminal, envie uma requisição vazia.