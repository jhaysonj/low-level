Projeto do GRIS - Grupo de Resposta a Incidentes de Segurança - Universidade Federal do Rio de Janeiro 

Idealizado por [Jhayson Jales, [João Pedro Nunes](https://github.com/Mistersz), [Pedro Kitagawa](https://github.com/k1sta)] 
Dificuldade do Guia [Medium]
Ano de inicio [2024 - ∞]




## Links relevantes

Mente binária - https://mentebinaria.gitbook.io/engenharia-reversa
Introdução Assembly - https://asmtutor.com/
Desafio realizado: [https://guyinatuxedo.github.io/04-bof_variable/csaw18_boi/index.html](https://guyinatuxedo.github.io/04-bof_variable/csaw18_boi/index.html)
Lista de instruções de Assembly: [https://www.cs.virginia.edu/~evans/cs216/guides/x86.html](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
BufferOverFlow: [https://www.rapid7.com/blog/post/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/](https://www.rapid7.com/blog/post/2019/02/19/stack-based-buffer-overflow-attacks-what-you-need-to-know/)



# Tópicos Abordados

1. Linguagem C: A maioria dos desafios de pwn envolve programas escritos em C. Portanto, é crucial ter uma compreensão sólida da linguagem C, incluindo conceitos como variáveis, ponteiros, arrays, estruturas de controle, funções e alocação dinâmica de memória.

2. Arquitetura de Computadores: Ter uma compreensão básica da arquitetura de computadores é importante para entender como a memória é organizada, como os programas são executados na CPU e como as instruções assembly são executadas.

3. Linguagem Assembly: Embora nem todos os desafios de pwn exijam conhecimento profundo de assembly, é útil ter uma compreensão básica da linguagem assembly da arquitetura que você está explorando. Normalmente, as arquiteturas x86 e x86-64 são as mais comuns.

4. Buffer Overflows: Buffer overflows são uma das vulnerabilidades mais comuns exploradas na pwn. Entender como eles funcionam e como podem ser explorados para alterar o comportamento de um programa é fundamental.

5. Format String Vulnerabilities: Outra vulnerabilidade comum é a format string vulnerability, que ocorre quando uma entrada do usuário é tratada incorretamente como uma string de formato em funções de formatação de string, como printf.

6. Segurança de Software: Ter uma compreensão básica de conceitos de segurança de software, como ASLR (Address Space Layout Randomization), DEP (Data Execution Prevention), e stack canaries, pode ajudá-lo a entender melhor os desafios de pwn.

7. Ferramentas de Depuração: Familiarize-se com ferramentas de depuração como GDB (GNU Debugger) ou LLDB (LLVM Debugger), que são frequentemente usadas para analisar e depurar programas durante a exploração de vulnerabilidades.

8. Ferramentas de Análise de Binários: Explore ferramentas como objdump, readelf e radare2, que podem ajudá-lo a analisar binários e entender sua estrutura e comportamento.

  

# Plataformas de Estudo

Plataformas e desafios online dedicados à prática de pwn (exploração de vulnerabilidades de software) e engenharia reversa. Algumas das mais populares incluem:

1. **pwnable.kr**: Esta plataforma oferece uma série de desafios de pwn, desde níveis básicos até avançados. Cada desafio geralmente envolve encontrar uma vulnerabilidade em um programa binário e explorá-la para obter acesso privilegiado ou executar código arbitrário.

2. **Exploit Exercises**: Este site oferece uma série de desafios de pwn, além de outros relacionados à segurança de sistemas. Os desafios são organizados em diferentes categorias, como Buffer Overflow, Format Strings, Heap Exploitation, etc.

3. **MicroCorruption**: Este é um jogo baseado na web que simula a exploração de falhas em dispositivos embarcados. Os jogadores são desafiados a encontrar e explorar vulnerabilidades em códigos assembly para dispositivos específicos.

4. **Crackmes.one**: Esta plataforma é dedicada a desafios de engenharia reversa. Os usuários podem fazer o download de binários e tentar descobrir como eles funcionam, muitas vezes procurando por chaves secretas ou entendendo algoritmos de criptografia.

5. **REverse.it**: Esta plataforma oferece uma variedade de desafios de engenharia reversa, incluindo crackmes, desafios de unpacking. Os usuários podem fazer upload de binários e compartilhar soluções com a comunidade.

6. **Root-Me**: Embora seja mais conhecido por desafios de hacking web, o Root-Me também oferece uma seção dedicada à engenharia reversa, com vários desafios para praticar suas habilidades de análise de malware e reversão de binários.

7. Over The Wire: Os wargames oferecidos pela comunidade OverTheWire podem ajudá-lo a aprender e praticar conceitos de segurança sob a forma de jogos divertidos.

# Assembly x86
Todas as linguagens de Assembly são feitas a partir de sets de instrução

**Instruções**
As instruções, também chamadas de Operadores, OpCodes, Op(s) ou mnemônicos são operações aritméticas simples que aceitam registradores ou valores constantes como argumentos. Existem duas versões de sintaxe para o Assembly x86:



**Sintaxe da Intel:**     `operador destino, origem`
	            `mov eax, 5`

- Sintaxe da AT&T:   `operador origem, destino`
				 `mov $5, %eax`


As sintaxes adotam padrões diferentes, como o uso dos prefixos % (para registradores) e $ (para valores constantes) na sintaxe da AT&T. Abordaremos a sintaxe da Intel ao decorrer desse estudo.


**Registradores Importantes:**
- EAX EBX ECX EDX: registradores de propósito geral
- ESP: ponteiro da stack (pilha), aponta para o topo da  pilha (memória baixa)
- EBP: Ponteiro base, aponta para a "base" do quadro de pilha atual (memória superior)
- EIP: Ponteiro de instrução, ponteiro para a próxima instrução a ser executada pela CPU
   
**Nome dos registradores em inglês:**
EAX: Extended Accumulator  
EBX: Extended Base  
ECX: Extended Counter  
EDX: Extended Data  
ESP: Extended Stack Pointer  
EBP: Extended Base Pointer  
EIP: Extended Instruction Pointer


**Memória Inferior e Memória Superior:**
"Memória inferior" se refere à parte da memória do sistema que possui endereços menores, enquanto "memória superior" refere-se à parte da memória com endereços maiores. Essa terminologia é frequentemente usada em arquiteturas de computadores para descrever a organização da memória física. Portanto, ao dizer que ESP é o "topo" da pilha (memória inferior) e EBP é a "base" da pilha (memória superior), estamos nos referindo às regiões relativas da memória onde a pilha é armazenada. A pilha cresce da memória inferior para a memória superior.


## Flags de Processador
As EFLAGS, são um conjunto de bits usados para armazenar o estado atual da CPU em operações e condições específicas. Elas são parte do registrador de flags, também conhecido como registrador EFLAGS em arquiteturas x86.
### Grupo de Flags (X, S e C)
As flags em arquiteturas x86/x86-64 são geralmente divididas em três grupos principais: X (eXtended Flags), S (Status Flags) e C (Control Flags). Essa divisão ajuda a organizar as flags de acordo com sua função e propósito:

**Flags Estendidas (X - eXtended Flags):**
- Essas flags incluem informações adicionais sobre o estado atual da CPU, como modo de operação, capacidades de processamento e controle de exceções.
- Alguns exemplos de flags estendidas incluem a flag de controle de modo de operação (PE), a flag de controle de interrupção (IF) e a flag de controle de direção (DF).


**Flags de Status (S - Status Flags):**
- Essas flags indicam o resultado de operações aritméticas e lógicas e fornecem informações sobre o estado atual do processamento.
- Exemplos de flags de status: a flag de sinal (SF), a flag de zeramento (ZF), a flag de overflow (OF), a flag de paridade (PF) e a flag de carry (CF).
    

**Flags de Controle (C - Control Flags):**
- Essas flags são usadas para controlar o funcionamento da CPU e do sistema, como o controle de interrupções, direção de operações e o modo de processamento.
- Exemplos de flags de controle incluem a flag de controle de interrupção (IF), a flag de controle de direção (DF), a flag de controle de modo de operação (PE) e a flag de controle de trap (TF).

### Descrição de Flags
As E-flags incluem uma série de bits que indicam informações sobre o estado atual da CPU, como por exemplo:
- Overflow Flag (OF): Seta o bit 1 caso tenha ocorrido um overflow (estouro) no resultado da última operação aritmética.
- Sign Flag (SF): Seta o bit 1 caso o resultado da última operação aritmética seja negativo.
- Zero Flag (ZF): Seta o bit 1 caso o resultado da última operação foi zero.
- Auxiliary Carry Flag (AF): Seta o bit 1 caso tenha “vai um” ou “empréstimo” no 4º bit (i.e., bit 3) da última operação aritmética.
- Parity Flag (PF): Seta o bit 1 caso o resultado da última operação seja par.
- Carry Flag (CF): Seta o bit 1 caso tenha “vai um” ou “empréstimo” na última operação aritmética.

Para maiores descrições sobre flag - https://mentebinaria.gitbook.io/assembly/aprofundando-em-assembly/flags-do-processador

## Sintaxe
### Movendo Dados
Move o valor de eax para ebx
```assembly
mov ebx, eax
```

Move o conteúdo de 0xDEADBEEF para eax
```
mov eax, 0xDEADBEEF
```    

Move o valor de 4 bytes do endereço 0x41424344 para edx
```
mov edx, DWORD PTR [0X41424344]
```
  
Move o valor de 4 bytes no registrador edx para ecx
```
mov ecx, DWORD PTR [edx]
```    

Move o valor de 4 bytes do endereço ecx+esi\*8 para eax
```
mov eax, DWORD PTR [ecx+esi*8]
```




### Operações Aritméticas

Subtrai 0x11 (valor em hexadecimal) de edx.
```
sub edx, 0x11
```


Soma eax e ebx, guardando o resultado em eax.
```
add eax, ebx
```

Incrementa edx
```
inc edx
```

Decrementa edx
```
dec ebx
```

Realiza uma operação xor bitwise(bit-a-bit) de eax com ele mesmo (zera eax);
```
xor eax, aex
```

Realiza uma operação or bitwise de edx com 0x1337
```
or  edx, 0x1337
```


### Saltos condicionais

**Jump if Zero (jz)**
```
jz $LOC ; Pula para $LOC se ZF = 1
```

**Jump if Not Zero (jnz)**
```
jnz $LOC ; Pula para $LOC se ZF ≠ 0 (ou seja, se não estiver zero)
```

**Jump if Greater (jg)**
```
jg $LOC ; Pula para $LOC se ZF=0 e SF=OF
```
**ZF = 0**: Zero Flag é igual a zero.
**SF = OF**: Sign Flag é igual ao Overflow Flag.

### Manipulação da Stack
**Instrução push:**
```
PUSH ebx
```
O valor armazenado no registrador EBX é colocado na stack.
O stack pointer (`esp`) é decrementado em 4 bytes e passa a apontar para uma nova posição no stack, já que a stack cresce "para baixo" em x86.

**instrução pop:**
```
POP ebx
```
O valor da stack é armazenado no registrador EBX
O stack pointer (`esp`) é incrementado em 4 bytes e passa a apontar para uma posição superior na stack, já que a stack cresce para baixo.

**Instrução add:**
```
add esp, 4
```
**Incrementa o Stack Pointer**: A instrução adiciona 4 bytes ao valor atual do stack pointer, movendo-o para cima na pilha. Como o stack cresce para baixo em x86, ao incrementar o ESP, você está essencialmente "removendo" a parte superior da pilha, liberando espaço.
**Libera Espaço no Stack**: Ao incrementar o ESP, você está indicando que não precisa mais dos dados que estavam no topo do stack. Isso pode ser uma forma de limpar ou "desfazer" uma operação de push, ou liberar espaço após uma função ou operação.


**Instrução sub:**
```
sub esp, 4
```
**Decrementa o ESP**: A instrução `sub esp, 4` reduz o valor do stack pointer em 4 bytes. Como a pilha em x86 cresce para baixo (ou seja, para endereços de memória menores), esta instrução efetivamente cria espaço no topo do stack.

**Reserva Espaço na Pilha**: Ao diminuir o stack pointer, você cria espaço para armazenar dados adicionais na pilha. Por exemplo, isso pode ser usado para alocar espaço para uma variável local ou para preparar uma área temporária para manipulação de dados.



**Instrução mov:**
```
mov DWORD PTR [esp], ebx
```
A instrução "mov" em Assembly x86 é usada para copiar dados de um lugar para outro.
- **Destino**: `[esp]` refere-se ao endereço de memória apontado pelo stack pointer (ESP).
- **Origem**: `ebx` é um registrador que contém um valor de 32 bits.
- **Ação**: A instrução move o valor de EBX para o endereço apontado por ESP, sem alterar o stack pointer.

Outro exemplo de mov:
```
mov ebx, DWORD PTR [esp]
```
- A instrução `mov ebx, DWORD PTR [esp]` recupera um valor do topo do stack (onde ESP aponta) e o armazena no registrador EBX.
- Essa operação não muda a posição do stack pointer, mantendo sua posição atual.


**Exemplo de programa com instruções de manipulação da stack**

```
; Criar espaço no stack para um valor temporário
sub esp, 4
; Escrever um valor no stack
mov DWORD PTR [esp], 12345
; Ler o valor para EBX sem alterar ESP
mov ebx, DWORD PTR [esp]
```

### Chamada e retorno de funções
Exemplo de aplicação da instrução call e ret:
```
section .text
    global _start  ; Ponto de entrada do programa

_start:
    mov eax, 10  ; Carrega diretamente 10 em EAX
    mov ebx, 20  ; Carrega diretamente 20 em EBX

    ; Chamar a função para somar os números
    call add_numbers  ; Chama a função add_numbers

    ; O resultado está em EAX após o retorno da função
    ; Finaliza o programa com um syscall para sair
    mov eax, 60  ; Syscall para saída
    xor edi, edi  ; Código de saída 0 (sucesso)
    syscall  ; Executar syscall para sair

; Função para somar dois números
add_numbers:
    add eax, ebx  ; Soma EBX a EAX
    ret  ; Retorna ao ponto após o `call`

```


```
call add_numbers
```
- A instrução `call add_numbers` salva o endereço de retorno no stack antes de saltar para a função. Isso faz com que o stack pointer seja decrementado.
- O stack pointer é ajustado para um valor menor, criando espaço no stack para armazenar o endereço de retorno.
- O comportamento é como se tivesse feito um `push`, armazenando um valor no stack
- 
```
ret 
```
- A instrução `ret` remove o endereço de retorno do stack e retorna o stack pointer à posição anterior, movendo-o para cima.
- O stack pointer é incrementado, já que está liberando o espaço onde estava o endereço de retorno.
- Isso é semelhante a um `pop`, restaurando o stack pointer para o estado anterior à chamada da função.

**Instrução nop:**
```
nop
```
- nop (No operation): Não faz nada, literalmente.

### Básico de x86

```
section .data
    string db "GRISGRIS", 0  ; Define a string terminada por nulo

section .text
    global _start  ; Ponto de entrada do programa

_start:
    mov ebx, string  ; EBX aponta para a string
    mov eax, 0       ; Contador para o comprimento da string

LOOPY:               ; Label para o loop
    mov cl, [ebx]    ; Carrega um byte da posição de EBX em CL
    cmp cl, 0        ; Compara com zero (final da string)
    jz end           ; Se for zero, pula para 'end'
    inc eax          ; Incrementa o contador
    inc ebx          ; Move EBX para o próximo byte
    jmp LOOPY        ; Volta para o início do loop

end:                 ; Label para o final
    ret              ; Retorna (com o comprimento da string)

```

O código retorna o comprimento da string armazenada na variável `string`, no exemplo acima, a variável `string` armazena `GRISGRIS` tem 8 letras, então retornamos o valor 8.

Os trechos `string db "GRISGRIS", 0` e `string db "GRISGRIS\0"` são equivalentes.


# Linguagem C (falta fazer)


# Engenharia Reversa (falta fazer)
Ferramentas
- gdb:  É um debugger, serve para vermos o que está acontecendo durante a execução de um programa
  

- ghidra
- ltrace
- objdump
- readelf
- strace

## gdb
Para abrir o programa com o gdb
```
┌──(romio@pop-os)-[~/Desktop/testes]
└─$ gdb stack_zero 

```

**comandos importantes do GDB**
1. breakpoint main
   adicionamos um breaking na função especificada, no exemplo acima, na função main
   ![[Pasted image 20240813134401.png]]
2. run
   Inicia o processo de debbuging do programa.
   
3. info registers
   Exibe as informações dos registradores
    - primeira coluna é o nome do registrador
	- segunda coluna valor em hexadecimal armazenado no registrador
	- terceira coluna valor em decimal armazenado no registrador
	
	![[Pasted image 20240813132725.png]]


2. info pro mappings: 
   Lista as regiões de memória mapeadas pelo nosso processo(executável)   ![[Pasted image 20240813131816.png]]

3. disassemble main
   realiza o disassemble da função especificada, no exemplo acima, a função main
   ![[Pasted image 20240813132653.png]]

Para mais informações sobre comandos importantes do gdb: https://ccrma.stanford.edu/~jos/stkintro/Useful_commands_gdb.html

**endbr64 (End Branch 64 bit)**
É uma instrução usada para marcar endereços de destino válidos para saltos indiretos e chamadas indiretas no código. Isso ajuda a garantir que o fluxo de controle do programa esteja indo para locais legítimos.
 
# Buffer Overflow
Um *buffer overflow* ocorre quando mais dados são escritos em um buffer (uma área de memória destinada a armazenar dados) do que ele pode suportar. Isso abre margem para que os dados transbordem para áreas adjacentes de memória, corrompendo ou sobrescrevendo o conteúdo existente.

## Crescimento da pilha

A pilha cresce em direção aos endereços mais baixos da memória. Isso significa que, à medida que novos dados são empilhados (como chamadas de função e variáveis locais), o ponteiro da pilha (`esp` ou `rsp` em arquiteturas x86/x86_64) é decrementado. Portanto, quando você "adiciona" algo à pilha, a memória ocupada vai de um endereço maior para um endereço menor.

Vamos simular o comportamento da stack ao longo da execução do código abaixo
```
```c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  volatile int modified;
  char buffer[64];

  modified = 0;
  gets(buffer);

  if(modified != 0) {
      printf("you have changed the 'modified' variable\n");
  } else {
      printf("Try again?\n");
  }
}
```
assembly
```
(gdb) disassemble main
Dump of assembler code for function main:
   0x080483f4 <+0>:	push   ebp
   0x080483f5 <+1>:	mov    ebp,esp
   0x080483f7 <+3>:	and    esp,0xfffffff0
   0x080483fa <+6>:	sub    esp,0x60
=> 0x080483fd <+9>:	mov    DWORD PTR [esp+0x5c],0x0
   0x08048405 <+17>:	lea    eax,[esp+0x1c]
   0x08048409 <+21>:	mov    DWORD PTR [esp],eax
   0x0804840c <+24>:	call   0x804830c <gets@plt>
   0x08048411 <+29>:	mov    eax,DWORD PTR [esp+0x5c]
   0x08048415 <+33>:	test   eax,eax
   0x08048417 <+35>:	je     0x8048427 <main+51>
   0x08048419 <+37>:	mov    DWORD PTR [esp],0x8048500
   0x08048420 <+44>:	call   0x804832c <puts@plt>
   0x08048425 <+49>:	jmp    0x8048433 <main+63>
   0x08048427 <+51>:	mov    DWORD PTR [esp],0x8048529
   0x0804842e <+58>:	call   0x804832c <puts@plt>
   0x08048433 <+63>:	leave  
   0x08048434 <+64>:	ret    
```






## Medidas de Proteção
- **Stack Canaries**: São valores especiais, chamados "canários", inseridos pelo compilador entre os buffers de dados (como arrays ou strings) e as áreas sensíveis da pilha (como o endereço de retorno da função). Se um buffer overflow ocorre, o canário deve ser sobrescrito antes de qualquer outra parte crítica da pilha.
- Address Space Layout Randomization (ASLR)
- Non-Executable Stack (NX)

Existem programas que verificam se o nosso binário possui medidas de proteção, um exemplo é o [checksec](https://github.com/slimm609/checksec.sh)

```
└─$ checksec --file=unvulnerable    
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
Full RELRO      Canary found      NX enabled    PIE enabled     No RPATH   No RUNPATH   38) Symbols	  No	0		1		unvulnerable

┌──(romio@pop-os)-[~/Desktop]
└─$ checksec --file=vulnerable 
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
Full RELRO      No canary found   NX disabled   PIE enabled     No RPATH   No RUNPATH   37) Symbols	  No	0		1		vulnerable

```
No binário `vulnerable` temos `No canary found`
No binário `unvulnerable` temos `canary found`

```
┌──(romio@pop-os)-[~/Desktop]
└─$ readelf -s vulnerable | grep '__stack_chk_fail' 

┌──(romio@pop-os)-[~/Desktop]
└─$ readelf -s unvulnerable | grep '__stack_chk_fail'
    24: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __stack_chk_fail[...]

```
Na versão unvulnerable, há canário.

## Exploiting Stack Overflows (falta fazer)

Exemplo de código vulnerável a buffer overflow
```
#include <stdio.h>
#include <string.h>

void vulnerable_function(char *user_input) {
    char buffer[10];
    strcpy(buffer, user_input);  // Vulnerável a buffer overflow
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        vulnerable_function(argv[1]);
    }
    return 0;
}
```

Compilação:
```
gcc -fno-stack-protector -z execstack -o vulnerable vulnerable.c
```

Explicação das flags:
A opção`-fno-stack-protector` desabilita essa `stack canaries`

Execução
Note que a execução do código acima funciona quando passamos uma string com 9 caracteres ou menos, isso por que há necessidade de armazenarmos no buffer o `\0`, caracter que marca o fim de uma string.
```
┌──(romio@pop-os)-[~/Desktop]
└─$ ./vulnerable AAAABBBBC                               
┌──(romio@pop-os)-[~/Desktop]
└─$ ./vulnerable AAAABBBBCC
zsh: segmentation fault (core dumped)  ./vulnerable AAAABBBBCC
```

- **`"AAAABBBBC"` (9 + 1 caracteres):** Cabe exatamente no buffer de 10 bytes. Sem erro.
- **`"AAAABBBBCC"` (10 + 1 caracteres):** Ultrapassa o buffer, sobrescreve a memória adjacente, causando _segmentation fault_.


## Dúvidas
1. por que devemos compilar com esses parametros que desabilitam as medidas de segurança? qual a diferença de compilar com/sem essas medidas? como visualizar essa diferença?
2. falta abordar como usar buffer overflow de forma maliciosa


# falta abordar (apenas rascunhos pra eu me guiar)

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