# Diretórios
CLIENTE_DIR = navegador
SERVIDOR_DIR = servidor

# Define the executable name
TARGET_CLIENTE = boli_navegador
TARGET_SERVIDOR = boli_servidor

# Default target: builds the executable
all: $(TARGET_CLIENTE) $(TARGET_SERVIDOR)
	clear
	echo sucesso
navegador: $(TARGET_CLIENTE)
servidor: $(TARGET_SERVIDOR)

# regras para cada alvo são executadas dentro de cada diretório
$(TARGET_CLIENTE):
	$(MAKE) -C $(CLIENTE_DIR)

$(TARGET_SERVIDOR):
	$(MAKE) -C $(SERVIDOR_DIR)

# Phony target for cleaning up generated files
.PHONY: clean
clean:
	$(MAKE) -C $(CLIENTE_DIR) clean
	$(MAKE) -C $(SERVIDOR_DIR) clean
	rm -rf arquivos_baixados
	clear