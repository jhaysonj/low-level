# Descrição do Guia
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
msg: db "Gris UFRJ", 10
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

# Desafio 1C
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

**Análise Estática**
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

Código
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

Com base no trecho acima, podemos verificar que se a senha for diferente de 0x149a então a senha é considerada errada. Com isso, basta digitarmos uma senha igual a 0x149a

# Tools
vim/nano 
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