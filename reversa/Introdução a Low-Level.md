# Descrição do Guia de Low-Level
Idealizado por [Jhayson Jales]
Dificuldade do Guia [Medium]
Material de Apoio [https://github.com/RPISEC/MBE/tree/master]
Ano de inicio [2024 - ∞]
Obs: O guia será realizado em linux, especificamente em distro ubuntu-base
## Pré Requisitos
Para verificar se esse guia é para você, verifique se possui conhecimento básico em:
[Python, C, Assembly, Linux]

**C**
Controle de Fluxo [if, else, for, while]
Manipulação de Ponteiros
Alocação Dinamica de memória
Estrutura de Dados [Array, struct, union]
Manipulação de String [strcpy, strcat, strlen]
Leitura e Escrita de Arquivos
Material de Apoio de [https://www.youtube.com/watch?v=oZeezrNHxVo&list=PLIfZMtpPYFP5qaS2RFQxcNVkmJLGQwyKE]

**Exploração de Binários**
Material de Apoio [https://www.youtube.com/watch?v=Ps3mZWQz01s&list=PLIfZMtpPYFP4MaQhy_iR8uM0mJEs7P7s3]

**Engenharia Reversa**
Material de Apoio [https://www.youtube.com/watch?v=IkUfXfnnKH4&list=PLIfZMtpPYFP6zLKlnyAeWY1I85VpyshAA]

Curso de Ghidra
Material de Apoio [https://www.youtube.com/watch?v=af0kbx8KuWo&list=PLIfZMtpPYFP5orPxcOZBn5cLOu7WG8xuF]

# Tópicos Abordados
Introdução à Arquitetura x86 e x86-64:
Compreensão dos conceitos básicos da arquitetura de processadores x86 e x86-64, incluindo registradores, instruções assembly, pilha e chamadas de sistema (Syscalls).

Buffer Overflows:
Aprender sobre exploração de buffer overflows, uma técnica comum em segurança cibernética, onde você aproveita vulnerabilidades relacionadas a operações de leitura/gravação além dos limites de um buffer.

Format Strings:
Exploração de vulnerabilidades de formato de string, onde você manipula strings de formato em funções de impressão para obter informações sensíveis da memória.

Heap Exploitation:
Compreensão de técnicas de exploração de vulnerabilidades relacionadas à alocação dinâmica de memória (heap), incluindo técnicas como overflow no heap, double free, entre outras.

Shellcoding:
Desenvolvimento de códigos de shell (shellcode) em Assembly para explorar vulnerabilidades e obter controle sobre o fluxo de execução.

Return-Oriented Programming (ROP):
Introdução ao conceito de ROP, uma técnica avançada onde você constrói cadeias de instruções de retorno existentes em um programa para realizar a execução de código arbitrário.

Proteções de Software:
Exploração de técnicas para superar proteções de segurança comuns, como DEP (Data Execution Prevention) e ASLR (Address Space Layout Randomization).

Análise Estática e Dinâmica:
Uso de ferramentas de análise estática e dinâmica, como IDA Pro, radare2, gdb e objdump, para entender o funcionamento interno dos binários.

Segurança em Software:
Compreensão de práticas seguras de programação, identificação de vulnerabilidades comuns e desenvolvimento de habilidades de análise de segurança.

Aplicações Práticas:
Aplicação de conhecimentos adquiridos para resolver desafios práticos, incluindo CTFs (Capture The Flags) e situações do mundo real.

# Semana 1
## Overview/Introdução
Para análises gerais, criamos um programa básico em C
```C
#include <stdio.h>

int main(void) {
	printf("GRIS\n");
	return 0;
}
```

Compilaremos o programa acima usando o make
```
make prog1  
cc     prog1.c   -o prog1
```

Com isso, conseguimos executar o binário e exibir a mensagem "GRIS"
```
./prog1
GRIS
```

### ltrace
Podemos usar o ltrace para vermos quais bibliotecas o nosso programa em C utiliza.

```
strace -e trace=write ./prog1
write(1, "GRIS\n", 5GRIS
)                   = 5
+++ exited with 0 +++
```

**Explicação da flag extraída do manual:**
Como queremos rastrear a função `printf()` passamos o parâmetro `-e trace=write`

```
-e expr     A  qualifying  expression which modifies which events to trace or how to
                   trace them.  The format of the expression 
```

O executável chama a biblioteca, a biblioteca chama o kernel. O kernel usa a syscall write para escrever na tela
![[Pasted image 20240130083111.png]]
No caminho Exec -> Lib -> Kernel conseguimos ver as chamadas das funções, ou seja, se interceptamos qualquer ponto dessas chamadas conseguimos monitorar o comportamento do programa.

### Análise do Prog1.c
**Assembler**
Um assembler é um tipo de programa de computador que recebe instruções básicas e as converte num padrão de bits que o processador do computador pode utilizar para efetuar operações básicas.

**Disassembler**
Se o assembler converte intruções assembly -> bits, o Disassembler pega um conjunto de bits e converte em intruções assembly
```
objdump -d prog1 | less
```

**Exit Status no Return 0:**
Um detalhe importante do comando acima é o trecho `mov    $0x0,%eax`, isso é equivalente o `return 0;` do programa em C
![[Pasted image 20240130084227.png]]
Uma maneira de visualizarmos isso é executarmos o nosso programa e verificar o exit status
```
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ ./prog1
GRIS
                                                                                              
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ echo $?
0

```

**Exit Status no Return 1:**
**Disassembler:**
Para fins de teste, editaremos o arquivo prog1.c mudando o `return 0;` para `return 1;`
```
objdump -d prog1 | less
```

![[Pasted image 20240130085407.png]]
**Saída:**
```
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ ./prog1
GRIS
                                                                                              
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ echo $?
1
```

## Arquivos
**Tamanho de Arquivos**

| Type | Bytes | Bits | Format Specifier |
| ---- | ---- | ---- | ---- |
| int | 2 ou 4 | 16 ou 32 | %d ou %i |
| char | 1 | 8 | %c |
| float | 4 | 32 | %f |
| double | 8 | 64 | %lf |
Obs:  1 byte -> 8 bits

**Testes Práticos:**
O comando abaixo joga a string texto dentro do arquivo1.txt
```
echo texto > arquivo1.txt
echo "texto" > arquivo2.txt
```

As duas strings `texto` possuem o mesmo peso em **bytes**
```
ls -l arquivo1.txt arquivo2.txt 
-rw-rw-r-- 1 romio romio 6 Jan 30 11:39 arquivo1.txt
-rw-rw-r-- 1 romio romio 6 Jan 30 11:46 arquivo2.txt
```

Você pode se perguntar por quê temos 6 bytes se a string tem apenas 5 caracteres e, 1\*5 = 5. Isso ocorre porque o comando echo por padrão adiciona o caracter de quebra de linha `\n`.

Podemos validar isso através dos comandos abaixo:
```
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ echo > arquivo3.txt        
                                                                                              
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ ls -l arqui*                   
-rw-rw-r-- 1 romio romio 6 Jan 30 11:39 arquivo1.txt
-rw-rw-r-- 1 romio romio 6 Jan 30 11:46 arquivo2.txt
-rw-rw-r-- 1 romio romio 1 Jan 30 11:52 arquivo3.txt

```

**Offset:**
Quando nos referimos a um byte em uma posição específica em um arquivo ou na memória, podemos usar os termos "byte na posição 0x88" ou "byte no offset 0x88". O termo "offset" é frequentemente usado para descrever a distância em bytes a partir de um ponto de referência.

**Exercício**
Através do comando abaixo, exibimos o conteudo hexadecimal do executável `/bin/ls`
```
hd /bin/ls | less
```

Diante da imagem abaixo, identifique o byte na posição 0x88
![[Pasted image 20240130120919.png]]
**Resposta** 
é o byte `18` sublinhado em azul. 
0x`00000080` indica o byte `18` 
0x`00000081` indica o byte `03`
0x`00000082` indica o byte `00` 
.
.
.
0x`00000088` indica o byte `18` 

obs: Como estamos usando o comando `hd` (hexdump), todas as medidas estão sendo exibidas em hexadecimal
![[Pasted image 20240130121715.png]]

### Arquivos Binários
Se abstrairmos a ideia de arquivo binário, poderíamos considerar todo arquivo como binário, entretanto, no contexto de Linux nos referimos à arquivos binários como arquivos executáveis.
Por isso, no Linux os executáveis ficando dentro de `/bin`, `/usr/bin` ou `/sbin` (super user bin, os binários do root)
```
┌──(romio@pop-os)-[~/Documents/Esoj/Códigos]
└─$ ls /               
bin   dev  home  lib32  libx32      media  proc      root  sbin  srv  tmp  var
boot  etc  lib   lib64  lost+found  mnt    recovery  run   snap  sys  usr

```

**Exemplo de código assembly:**
```
cat hello.asm   
section .data
msg: db "Gris UFRJ", 10; ou msg: db "Gris UFRJ\n"
len: equ $-msg

section .text
global _start

_start_:
mov edx, len
mov ecx, msg
mov ebx, 1
mov eax, 4; write
int 0x80

mov ebx, 0
mov eax, 1
int 0x80

```

 

Vamos gerar um object-file do arquivo hello.asm, no formato ELF (Executable and Linkable Format) de 32 bits
```
nasm -f elf32 -o hellonasm hello.asm
```


**Visualização do hexadecimal**
```
hd hellonasm | less
```

![[Pasted image 20240130135653.png]]
Obs: Apesar dessas coisas não fazerem total sentido ainda, é importante pra mostrar que primeiro temos o campo de texto e depois o campo de código.
Note que a linha sublinha representa a string `Gris UFRJ`

**[Ascii to Hex](https://web.fe.up.pt/~ee96100/projecto/Tabela%20ascii.htm):**

| Caracter | Hex |
| ---- | ---- |
| G | 47 |
| r | 72 |
| i | d5 |
| s | 73 |
| space | 20 |
| U | 55 |
| F | 46 |
| R | 52 |
| J | 4a |
## Strings de Texto
Vamos analisar um arquivo de texto, para isso criaremos um arquivo chamado programinha.c
```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ cat programinha.c
#include <stdio.h>

int main(void) {
	puts("blog.gris.dcc.ufrj.br");
	return 0;
}


┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ make programinha
cc     programinha.c   -o programinha

```
### 0a
Os analisadores de hexadecimal não possuem representação gráfica para a quebra de linha, com isso acabam representando graficamente o `\n` como `.` 
![[Pasted image 20240130170111.png]]
Como exibido na imagem acima, ao quebrar a linha duas vezes `0a 0a`, a representação gráfica foi `..` 

Consultando a tabela ASCII conseguimos determinar que a string `.` é definida pelo hexadecimal `2e`.  
![[Pasted image 20240130170021.png]]


**Comportamento no Windows:**
No Windows temos dois bytes para fazermos a quebra de linha, o Carriage Return e o Line Feed, sendo respectivamente, `0d` e `0a` em hexadecimal.

Além disso, o hexadecimal pode ser influenciado pelo encoding, tanto no Windows como no Linux
![[Pasted image 20240130153453.png]]

```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ echo árvore | strings
rvore
                                                                                
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ echo árvore | strings -e S
árvore

```


**Analisando o arquivo3.txt:** 
O arquivo3.txt contém apenas a quebra de linha
```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ echo > arquivo3.txt

┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ hd arquivo3.txt 
00000000  0a                                                |.|
00000001
```
Com isso, podemos ver que o hexadecimal de quebra de linha é o 0a, ou seja, **conseguimos deduzir a quantidade de linhas de um arquivo apenas contabilizando a quantidade de `0a`.**


**Analisando programinha.c**
```
hd programinha.c
00000000  23 69 6e 63 6c 75 64 65  20 3c 73 74 64 69 6f 2e  |#include <stdio.|
00000010  68 3e 0a 0a 69 6e 74 20  6d 61 69 6e 28 76 6f 69  |h>..int main(voi|
00000020  64 29 20 7b 0a 09 70 75  74 73 28 22 62 6c 6f 67  |d) {..puts("blog|
00000030  2e 67 72 69 73 2e 64 63  63 2e 75 66 72 6a 2e 62  |.gris.dcc.ufrj.b|
00000040  72 22 29 3b 0a 09 72 65  74 75 72 6e 20 30 3b 0a  |r");..return 0;.|
00000050  7d 0a                                             |}.|
00000052

```
Temos seis `0a`, um para cada linha do programa, isso porque após fechar a chave eu dei um último enter
![[Pasted image 20240130144541.png]]
![[Pasted image 20240130144658.png]]

### Exercício recomendado:
1) edite o pega_string.c para exibir tudo que estiver entre hex `20` e `7e`
2) edite o pega_string.c para exibir apenas letras maiúsculas
3) edite o pega_string.c para exibir apenas letras minúsculas
4) edite o pega_string.c para exibir apenas caracter numéricos

Para realizar os testes, pase como entrada o arquivo abaixo
```
└─$ cat entrada.txt 
abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789

```

Arquivo pega_string.c
```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ cat pega_string.c   
#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    unsigned char byte;

    while (fread(&byte, sizeof(byte), 1, fp)){
        printf("%c", byte);
    }

    printf("\n");
    fclose(fp);


    return 0;
}                                                  
```

Executando o pega_string
```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ make pega_string
cc     pega_string.c   -o pega_string

┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ ./pega_string entrada.txt 
abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789

```

**Resposta 1:**
Basta criarmos um if condicional que verifica se o byte está incluso na range entre `0x20` e `0x7e`.

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    unsigned char byte;

    while (fread(&byte, sizeof(byte), 1, fp)){
    	if(byte >= 0x20 && byte <= 0x7e){
        printf("%c", byte);
	    }
    }

    printf("\n");
    fclose(fp);


    return 0;
}

```
Esse arquivo imprime na tela só o conteúdo que possui representação visual, ou seja, com isso removemos todos os `\n` do arquivo entrada.txt

```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ make resposta1_pega_string   
cc     resposta1_pega_string.c   -o resposta1_pega_string
                                                                                
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ ./resposta1_pega_string entrada.txt 
abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789

```
obs: Note que ainda existe um `\n`, entratanto ele é referente ao `\n` do código, todos os `\n` do arquivo entrada.txt foram removidos.

**Resposta 2:**
Similar ao que fizemos, basta editarmos a condição if, consultando a tabela ASCII, constatamos que os valores maiúsculos A-Z estão no range hexadecimal 0x41 e 0x5a

**Código:**
```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    unsigned char byte;

    while (fread(&byte, sizeof(byte), 1, fp)){
    	if(byte >= 0x41 && byte <= 0x5a){
        printf("%c", byte);
	    }
    }

    printf("\n");
    fclose(fp);


    return 0;
}                  ```

```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ make resposta2_pega_string
cc     resposta2_pega_string.c   -o resposta2_pega_string
                                                                                                                                                                                              
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ ./resposta2_pega_string entrada.txt
ABCDEFGHIJKLMNOPQRSTUVWXYZ
                               
```

**Resposta 3:**
De forma análoga, consultando a tabela ASCII, constatamos que os valores maiúsculos a-z estão no range hexadecimal 0x61 e 0x7a
Código:
```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "rb");
    unsigned char byte;

    while (fread(&byte, sizeof(byte), 1, fp)){
    	if(byte >= 0x61 && byte <= 0x7a){
        printf("%c", byte);
	    }
    }

    printf("\n");
    fclose(fp);


    return 0;
}                
```

```
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ make resposta3_pega_string         
cc     resposta3_pega_string.c   -o resposta3_pega_string
                                                                                                                                                                                              
┌──(romio@pop-os)-[~/Documents/Esoj/semana 1/codigos]
└─$ ./resposta3_pega_string entrada.txt 
abcdefghijklmnopqrstuvwxyz

```


# Semana 2
Vimos na semana 1 que não nos referimos a todo tipo de arquivo como binários. Nesse capítulo vamos estudar o que define um executável/binário.

O conteúdo desse tópico será fortemente baseado na discussão do mente binária. Fica o link de referência ao material original.
Forum - https://www.mentebinaria.com.br/forums/topic/97-formato-pe
Youtube - https://www.youtube.com/@mentebinaria

## Portable Executables (PE)
Os arquivos PE contêm todas as informações necessárias para que o sistema operativo carregue corretamente o código executável, ou seja, os arquivos com extensão `.exe` e `.dll` são arquivos no formato PE. 

Link para Discussão do formato PE - https://stackoverflow.com/questions/33757527/difference-between-exe-and-pe-files

**DOS-Signature:** 
Uma sequência de bytes no início do arquivo que identifica o arquivo como um executável.
Nas imagens abaixo, podemos notar que os dois arquivos .exe começam com a representação ASCII `MZ` e word `4d 5a`.

**Definições de Word:**
Byte = 8 bits
WORD = 2 bytes
DWORD (double WORD) = 4 bytes
QWORD (quad WORD) = 8 bytes

![[Pasted image 20240131114043.png]]
![[Pasted image 20240131114349.png]]
Se trocarmos o hexadecimal `4d` = `m` por `4e` = `n` teríamos um executável corrompido.

### Offset 0x18
Temos uma DWORD (double word) com o valor `0x40000000`. Aqui temos que fazer a seguinte consideração: na arquitetura Intel os bytes são endereçados de forma inversa, mais conhecido como Little Endian. Em Little Endian, os bytes menos significativos (LSB) são armazenados primeiro, por isso devemos inverter estes bytes. Então temos o valor `0x00000040` = `0x40`, este é o deslocamento para DOS-Stub.

![[Pasted image 20240131132248.png]]

### offset 0x3c/e_lfanew
Na posição `0x3c` temos o ponteiro conhecido como o campo `e_lfanew`. O valor encontrado nesse ponteiro, aponta para o início do cabeçalho IMAGE_NT_HEADERS (cabeçalho PE).

![[Pasted image 20240131140735.png]]

Por conta do little endian passamos de `68 01 00 00` para `00 00 01 68` = `0x168`, ou seja, na posição `0x168` temos o inicio do cabeçalho PE
![[Pasted image 20240131140902.png]]

**Importancia da assinatura PE**
A partir da assinatura PE conseguimos algumas informações importantes.

### Cabeçalho COFF
**Definindo arquitetura atráves do campo Machine**
Logo após a assinatura PE na posição 0x168 temos o primeiro campo do cabeçalho COFF que é o `Machine` o valor `4c 01` que define que este executável foi criado para máquinas Intel i386 ou compatíveis.

| Valor em Little-Endian | Arquitetura |
|------------------------|-------------|
| 0x 4c 01               | i386        |
| 0x 64 86                 | AMD64       |
| 0x 00 02                 | Itanium     |
![[Pasted image 20240131143339.png]]

**Definindo o TimeDateStamp****
Depois vem o campo **TimeDateStamp** com o número inteiro de 32 _bits_ (4 _bytes_) com o valor `f0 19 37 64` (Em little-endian)
![[Pasted image 20240131144238.png]]
**Interpretação do TimeDateStamp**
Para conseguirmos interpretar o TimeDateStamp precisamos seguir alguns passos
1. **Conversão para big-endian**
	No hexdump a representação é little-endian `f0 19 37 64`, ou seja, precisamos passar para big-endian, ficando com `64 37 19 f0`. 
2. **Converta para Decimal:**
	conversão do valor hexadecimal `64 37 19 f0` (big-endian) para decimal
	conversão de hexadecimal para decimal [Português] - https://www.youtube.com/watch?v=Vctnbk0RWVY 
Com isso, obtemos o valor decimal `1681332720`, Este timestamp representa a data e hora em que o executável foi vinculado ou compilado, medido em segundos desde 1º de janeiro de 1970 (timestamp UNIX).

A partir dos programas abaixo conseguimos calcular a data da compilação
**Em C:**
```C
#include <stdio.h>
#include <time.h>

int main() {
    time_t timestamp = 1681332720;
    struct tm *timeinfo;

    timeinfo = localtime(&timestamp);

    printf("Data e Hora: %s", asctime(timeinfo));

    return 0;
}

```
**Em Python**
```python
import datetime

timestamp = 1681332720
dt_object = datetime.datetime.fromtimestamp(timestamp)

print("Data e Hora:", dt_object)

```
Com isso, obtemos `Data e Hora: 2023-04-12 17:52:00`
No formato "ano-mês-dia"

Lógico que analisar manualmente cada um dos cabeçalhos daria muito trabalho, existem programas como o objdump que automatiza essas análises
```
 objdump -p ~/Downloads/Install\ League\ of\ Legends\ br.exe | head -n 20

/home/romio/Downloads/Install League of Legends br.exe:     file format pei-i386

Characteristics 0x122
	executable
	large address aware
	32 bit words

Time/Date		Wed Apr 12 17:52:00 2023
Magic			010b	(PE32)
MajorLinkerVersion	14
MinorLinkerVersion	34
SizeOfCode		0065c800
SizeOfInitializedData	03dc6200
SizeOfUninitializedData	00000000
AddressOfEntryPoint	00607b4c
BaseOfCode		00001000
BaseOfData		0065e000
ImageBase		00400000
SectionAlignment	00001000

```


Video Recomendado [Português] - https://www.youtube.com/watch?v=WB8pLhfr_hU
Video Recomendado [Inglês] - https://www.youtube.com/watch?v=-ojciptvVtY

## Executable and Linkable Format (ELF)

Video Recomendado [Inglês] - https://youtu.be/WnqOhgI_8wA?t=113

**Assinatura:**
O que define um arquivo como ser executável é o que chamamos de assinatura, se analisarmos o hexadecimal desses arquivos, todos começam com `7f 45 4c 46` que indicam `.elf` em ASCII

Comando `ls`
![[Pasted image 20240131110945.png]]

Comando `cat`
![[Pasted image 20240131111142.png]]

## System calls (Syscalls)
**O que é system call?**
Uma syscall é uma forma de mantermos uma interação entre o programa e o kernel do sistema operacional, com o objetivo solicitar um serviço/recurso ao sistema operacional.


**Exemplos de Serviços providos de System Calls e Principais utilidades**
1. **Gerenciamento de Processos:**
    - Criação de processos (`fork`, `execve`, `exit`).
    - Controle de processos (por exemplo, `waitpid`, `kill`).
    - Mudanças de contexto de execução de processos (`sched_yield`, `nice`).
2. **Gerenciamento de Arquivos e Diretórios:**
    - Abertura e fechamento de arquivos (`open`, `close`).
    - Leitura e escrita de dados em arquivos (`read`, `write`).
    - Manipulação de diretórios (`mkdir`, `rmdir`, `opendir`, `readdir`).
3. **Controle de Dispositivos e Comunicação de Dados:**
        - Operações de Entrada/Saída em dispositivos (`read`, `write` com dispositivos).
    - Controle de comunicação entre processos (`pipe`, `shmget`, `msgget`).
4. **Gerenciamento de Memória:**
    - Alocação e liberação de memória (`malloc`, `free`).
    - Proteção e mapeamento de páginas de memória (`mmap`).
    - Alteração do tamanho do espaço de endereço de um processo (`brk`, `sbrk`).
5. **Controle de Sinais e Interrupções:**
    - Manipulação de sinais (`signal`, `kill`, `sigaction`).
    - Definição e alteração do comportamento de sinais.
6. **Rede:**
    - Comunicação em rede (`socket`, `bind`, `connect`, `send`, `recv`).
    - Configuração de interfaces de rede (`ifconfig`, `ioctl`).
7. **Relógio e Tempo:**
    - Obtenção do tempo atual (`time`, `gettimeofday`).
    - Configuração do relógio do sistema (`adjtimex`).
8. **Controle de Permissões e Identidade:**
    - Manipulação de permissões e propriedades de arquivos (`chmod`, `chown`).
    - Controle de identidade de processo (`setuid`, `setgid`).
9. **Controle de Recursos do Sistema:**
    - Obtenção de informações sobre o sistema (`uname`, `sysctl`).
    - Gerenciamento de recursos do sistema (`nice`, `ulimit`).
10. **Comunicação Interprocesso (IPC):**
    - Uso de mecanismos de IPC como filas de mensagens (`msgget`, `msgsnd`, `msgrcv`), semáforos (`semget`, `semop`), e memória compartilhada (`shmget`, `shmat`).
Obs: Cada sistema operacional pode ter sua própria lista de chamadas de sistema para fornecer serviços aos programas de aplicação.
[Linux Syscalls](https://www.ime.usp.br/~kon/MAC211/syscalls.html)

Leitura recomendada [Inglês] - https://www.geeksforgeeks.org/introduction-of-system-call/

## Kernel
**O que é o kernel?**
O kernel é um componente do sistema operacional que gerencia operações e o hardware (memória e CPU). Serve como uma ponte entre a aplicação e o processamento de dados (a nível de hardware), usando syscalls e comunicação inter-process.

O kernel é carregado primeiro na memória quando um sistema operacional é carregado e permanece na memória até o sistema operacional ser novamente encerrado. É responsável por várias tarefas, como a gestão do disco, a gestão de tarefas e a gestão da memória.

Leitura recomendada [Inglês] - https://www.geeksforgeeks.org/kernel-in-operating-system/


# Introdução aos Desafios
Os desafios resolvidos ao longo desse guia estão presentes no link abaixo:
https://github.com/RPISEC/MBE/tree/master/src

## Ordem de Dificuldade
A ordem de dificuldade é crescente indo da pasta lab01 até a pasta 
lab10. Dentro da pasta existem sempre 3 arquivos, a ordem de dificuldade crescente é do C para o A. Essa nomenclatura indica a nota do estudante no sistema norte-americano, se o aluno resolver apenas o desafio C ficaria com nota C, se resolver B e C receberia nota B, caso o aluno resolvesse os três desafios receberia nota A.

# Semana X

## Guia Ghidra
1. instale o ghidra
```
sudo snap install ghidra
```
2. configure o ghidra
	![[Pasted image 20240129235801.png]]
3. crie um projeto
	![[Pasted image 20240130000118.png]]
4. Abra o menu CodeBrowser (clica no dragão)
	   ![[Pasted image 20240130000245.png]]
5. Arraste o arquivo lab1C para o menu CodeBrowser
	   ![[Pasted image 20240130000326.png]]
6. clique em ok
	 ![[Pasted image 20240130000413.png]] 
7. clique em yes
	![[Pasted image 20240130000522.png]]
8. clique em Analyze
	![[Pasted image 20240130000547.png]]

9. clique em Ok
	![[Pasted image 20240130000626.png]]

Agora que temos o arquivo analisado pelo Ghidra, podemos examinar o comportamento da função main do programa lab1C. 
No menu Symbol tree do ghidra, faça o caminho Functions -> main
![[Pasted image 20240130000840.png]]

## Lab1
## Desafio 1C
**Analise Dinamica:**
Aqui executamos o arquivo lab1C para verificarmos o que é feito.
```
./lab1C 
-----------------------------
--- RPISEC - CrackMe v1.0 ---
-----------------------------

Password: teste

Invalid Password!!!

```

**[[#Guia Ghidra|Análise Estática]]**

**Código:**
```
bool main(void)

{
  int local_14 [4];
  
  puts("-----------------------------");
  puts("--- RPISEC - CrackMe v1.0 ---");
  puts("-----------------------------");
  printf("\nPassword: ");
  __isoc99_scanf(&DAT_08048818,local_14);
  if (local_14[0] != 0x149a) {
    puts("\nInvalid Password!!!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
  }
  return local_14[0] != 0x149a;
}

```

Com base no trecho acima, podemos verificar que se a senha for diferente de 0x149a então a senha é considerada errada. Com isso, basta digitarmos uma senha igual a 0x149a, entretanto precisamos descobrir se trata-se de um int, char, float ou double. Uma maneira ingênua seria testar todas as possibilidades, entretanto podemos usar a função scanf para nos ajudar a decidir qual é o tipo de variável.

### Relembrando Scanf
A função scanf recebe dois paramêtros, o primeiro é o que chamamos de Format String e o segundo é a posição de memória.

**Format String (%specifier)**
A Format String  é composta por especificadores de formato que indicam o tipo de dados que será lido. Exemplos de especificadores incluem `%d` para inteiros, `%f` para floats, `%c` para caracteres, `%s` para strings.

```C
int numero; 
scanf("%d", &numero); // int

```

```C
float valor; 
scanf("%f", &valor); // float
```

```c
char caractere;
scanf("%c", &caractere); // char
```

Com isso, ao clicarmos na variável `&DAT_08048818`, vamos para o trecho em assembly correspondente, como mostra a imagem abaixo:

![[Pasted image 20240131110234.png]]
Pelo assembly `%d`, conseguimos definir que a variável trata-se de um número inteiro, ou seja, a senha em hexadecimal `0x149a` armazenada em um vetor de 4 posições é o inteiro 5274.
**Video de Resolução:**
https://www.youtube.com/watch?v=UxTG7bS3nuY



# Tools
vim/nano (não será necessário usar os dois editores de texto, escolha o que preferir)
make
gcc
objdump
ltrace

Instale todas as ferramentas com o comando abaixo:
```
sudo apt install vim nano make gcc objdump ltrace
```
# Referências
Definindo System Calls https://www.geeksforgeeks.org/introduction-of-system-call/

Guia de engenharia reversa https://github.com/mytechnotalent/Reverse-Engineering



# parei em 
https://www.youtube.com/watch?v=WB8pLhfr_hU