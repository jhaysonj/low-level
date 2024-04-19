Projeto do GRIS - Grupo de Resposta a Incidentes de Segurança - Universidade Federal do Rio de Janeiro 

Idealizado por [Jhayson Jales, João Pedro Nunes, Pedro Kitagawa] 
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

## Assembly x86
Todas as linguagens de Assembly são feitas a partir de sets de instrução

**Instruções**
As instruções, também chamadas de Operadores, OpCodes, Op(s) ou mnemônicos são operações aritméticas simples que aceitam registradores ou valores constantes como argumentos. Existem duas versões de sintaxe para o Assembly x86:


**Sintaxe da Intel:**     `operador destino, fonte`
	            `mov eax, 5`

- Sintaxe da AT&T:   `operador fonte, destino`
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

Move o valor de 4 bytes do endereço ecx+esi*8 para eax
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

**Jump If Greater (jg)**
```
jg $LOC ; Pula para $LOC se ZF=0 e SF=OF
```
**ZF = 0**: Zero Flag é igual a zero.
**SF = OF**: Sign Flag é igual ao Overflow Flag.

### Manipulação da Stack
