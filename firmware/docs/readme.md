bibliotecas 

- mcp_can

para executar o codigo pelo terminal

requisitos:
- arduino-cli

acessar a pasta do projeto o nome da pasta tem que ser igual ao do arquivo.ino

```bash
arduino-cli board list
``` 

verifica a porta atras de arduino

```bash
arduino-cli compile --fqbn arduino:avr:mega .
```
compila o codigo atrs de erro

```bash
arduino-cli upload -p COM3 --fqbn arduino:avr:mega .
```
faz o upload do codigo na porta

```bash
arduino-cli monitor -p COM3 -c baudrate=9600
```
habilita o monitor serial na porta e o baudrate