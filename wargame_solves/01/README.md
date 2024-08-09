# Labs 01 - Modern Binary Exploitation

Este documento contém minhas soluções para os problemas do laboratório 1 do programa Modern Binary Exploitation

## Lab1C

O primeiro problema nos prompta com uma interface para inserir uma senha: 

```
-----------------------------
--- RPISEC - CrackMe v1.0 ---
-----------------------------

Password: 
```

Realizando o disassembly da `main()`:

```x86
Dump of assembler code for function main:
   0x080486ad <+0>:	push   ebp
   0x080486ae <+1>:	mov    ebp,esp
   0x080486b0 <+3>:	and    esp,0xfffffff0
   0x080486b3 <+6>:	sub    esp,0x20
   0x080486b6 <+9>:	mov    DWORD PTR [esp],0x80487d0
   0x080486bd <+16>:	call   0x8048560 <puts@plt>
   0x080486c2 <+21>:	mov    DWORD PTR [esp],0x80487ee
   0x080486c9 <+28>:	call   0x8048560 <puts@plt>
   0x080486ce <+33>:	mov    DWORD PTR [esp],0x80487d0
   0x080486d5 <+40>:	call   0x8048560 <puts@plt>
   0x080486da <+45>:	mov    DWORD PTR [esp],0x804880c
   0x080486e1 <+52>:	call   0x8048550 <printf@plt>
   0x080486e6 <+57>:	lea    eax,[esp+0x1c]
   0x080486ea <+61>:	mov    DWORD PTR [esp+0x4],eax
   0x080486ee <+65>:	mov    DWORD PTR [esp],0x8048818
   0x080486f5 <+72>:	call   0x80485a0 <__isoc99_scanf@plt>
   0x080486fa <+77>:	mov    eax,DWORD PTR [esp+0x1c]
   0x080486fe <+81>:	cmp    eax,0x149a
   0x08048703 <+86>:	jne    0x8048724 <main+119>
   0x08048705 <+88>:	mov    DWORD PTR [esp],0x804881b
   0x0804870c <+95>:	call   0x8048560 <puts@plt>
   0x08048711 <+100>:	mov    DWORD PTR [esp],0x804882b
   0x08048718 <+107>:	call   0x8048570 <system@plt>
   0x0804871d <+112>:	mov    eax,0x0
   0x08048722 <+117>:	jmp    0x8048735 <main+136>
   0x08048724 <+119>:	mov    DWORD PTR [esp],0x8048833
   0x0804872b <+126>:	call   0x8048560 <puts@plt>
   0x08048730 <+131>:	mov    eax,0x1
   0x08048735 <+136>:	leave  
   0x08048736 <+137>:	ret    
End of assembler dump.
```

Nota-se, em `<main+81>`, que há uma comparação entre o valor inputado e `0x149a` (5274 em decimal). Portanto, ao inserir `5274` como senha, acessamos o usuário `lab1B`.

## Lab1B

### Análise das funções

Dessa vez, somos promptados com a mesma inteface de login:

```
-----------------------------
--- RPISEC - CrackMe v1.0 ---
-----------------------------

Password: 
```
Temos que realizar, então, o disassembly da main:
```x86
Dump of assembler code for function main:
   0x08048be4 <+0>:	push   ebp
   0x08048be5 <+1>:	mov    ebp,esp
   0x08048be7 <+3>:	and    esp,0xfffffff0
   0x08048bea <+6>:	sub    esp,0x20
   0x08048bed <+9>:	push   eax
   0x08048bee <+10>:	xor    eax,eax
   0x08048bf0 <+12>:	je     0x8048bf5 <main+17>
   0x08048bf2 <+14>:	add    esp,0x4
   0x08048bf5 <+17>:	pop    eax
   0x08048bf6 <+18>:	mov    DWORD PTR [esp],0x0
   0x08048bfd <+25>:	call   0x80487b0 <time@plt>
   0x08048c02 <+30>:	mov    DWORD PTR [esp],eax
   0x08048c05 <+33>:	call   0x8048800 <srand@plt>
   0x08048c0a <+38>:	mov    DWORD PTR [esp],0x8048d88
   0x08048c11 <+45>:	call   0x80487d0 <puts@plt>
   0x08048c16 <+50>:	mov    DWORD PTR [esp],0x8048da6
   0x08048c1d <+57>:	call   0x80487d0 <puts@plt>
   0x08048c22 <+62>:	mov    DWORD PTR [esp],0x8048dc4
   0x08048c29 <+69>:	call   0x80487d0 <puts@plt>
   0x08048c2e <+74>:	mov    DWORD PTR [esp],0x8048de2
   0x08048c35 <+81>:	call   0x8048780 <printf@plt>
   0x08048c3a <+86>:	lea    eax,[esp+0x1c]
   0x08048c3e <+90>:	mov    DWORD PTR [esp+0x4],eax
   0x08048c42 <+94>:	mov    DWORD PTR [esp],0x8048dee
   0x08048c49 <+101>:	call   0x8048840 <__isoc99_scanf@plt>
   0x08048c4e <+106>:	mov    eax,DWORD PTR [esp+0x1c]
   0x08048c52 <+110>:	mov    DWORD PTR [esp+0x4],0x1337d00d
   0x08048c5a <+118>:	mov    DWORD PTR [esp],eax
   0x08048c5d <+121>:	call   0x8048a74 <test>     # chama test()
   0x08048c62 <+126>:	mov    eax,0x0
   0x08048c67 <+131>:	leave  
   0x08048c68 <+132>:	ret    
End of assembler dump.

```

Ao efetuar o disassembly, reparamos que a função `test()` é chamada em `<main+121>`, logo após a inserção do input do usuário. Os parâmetros que são passados para ela são o valor `0x1337d00d`, em `<main+110>`, e a senha inputada pelo usuário. 

Conferindo a função `test()`:

```x86
Dump of assembler code for function test:
   0x08048a74 <+0>:	push   ebp
   0x08048a75 <+1>:	mov    ebp,esp
   0x08048a77 <+3>:	sub    esp,0x28
   0x08048a7a <+6>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048a7d <+9>:	mov    edx,DWORD PTR [ebp+0xc]
   0x08048a80 <+12>:	sub    edx,eax                  #subtrai os inputs
   0x08048a82 <+14>:	mov    eax,edx
   0x08048a84 <+16>:	mov    DWORD PTR [ebp-0xc],eax
   0x08048a87 <+19>:	cmp    DWORD PTR [ebp-0xc],0x15
   0x08048a8b <+23>:	ja     0x8048bd5 <test+353>         
   0x08048a91 <+29>:	mov    eax,DWORD PTR [ebp-0xc]      
   0x08048a94 <+32>:	shl    eax,0x2
   0x08048a97 <+35>:	add    eax,0x8048d30
   0x08048a9c <+40>:	mov    eax,DWORD PTR [eax]
   0x08048a9e <+42>:	jmp    eax
   0x08048aa0 <+44>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048aa3 <+47>:	mov    DWORD PTR [esp],eax
   0x08048aa6 <+50>:	call   0x80489b7 <decrypt>
   0x08048aab <+55>:	jmp    0x8048be2 <test+366>
   0x08048ab0 <+60>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048ab3 <+63>:	mov    DWORD PTR [esp],eax
   0x08048ab6 <+66>:	call   0x80489b7 <decrypt>
   0x08048abb <+71>:	jmp    0x8048be2 <test+366>
   0x08048ac0 <+76>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048ac3 <+79>:	mov    DWORD PTR [esp],eax
   0x08048ac6 <+82>:	call   0x80489b7 <decrypt>
   0x08048acb <+87>:	jmp    0x8048be2 <test+366>
   0x08048ad0 <+92>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048ad3 <+95>:	mov    DWORD PTR [esp],eax
   0x08048ad6 <+98>:	call   0x80489b7 <decrypt>
   0x08048adb <+103>:	jmp    0x8048be2 <test+366>
   0x08048ae0 <+108>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048ae3 <+111>:	mov    DWORD PTR [esp],eax
   0x08048ae6 <+114>:	call   0x80489b7 <decrypt>
   0x08048aeb <+119>:	jmp    0x8048be2 <test+366>
   0x08048af0 <+124>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048af3 <+127>:	mov    DWORD PTR [esp],eax
   0x08048af6 <+130>:	call   0x80489b7 <decrypt>
   0x08048afb <+135>:	jmp    0x8048be2 <test+366>
   0x08048b00 <+140>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b03 <+143>:	mov    DWORD PTR [esp],eax
   0x08048b06 <+146>:	call   0x80489b7 <decrypt>
   0x08048b0b <+151>:	jmp    0x8048be2 <test+366>
   0x08048b10 <+156>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b13 <+159>:	mov    DWORD PTR [esp],eax
   0x08048b16 <+162>:	call   0x80489b7 <decrypt>
   0x08048b1b <+167>:	jmp    0x8048be2 <test+366>
   0x08048b20 <+172>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b23 <+175>:	mov    DWORD PTR [esp],eax
   0x08048b26 <+178>:	call   0x80489b7 <decrypt>
   0x08048b2b <+183>:	jmp    0x8048be2 <test+366>
   0x08048b30 <+188>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b33 <+191>:	mov    DWORD PTR [esp],eax
   0x08048b36 <+194>:	call   0x80489b7 <decrypt>
   0x08048b3b <+199>:	jmp    0x8048be2 <test+366>
   0x08048b40 <+204>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b43 <+207>:	mov    DWORD PTR [esp],eax
   0x08048b46 <+210>:	call   0x80489b7 <decrypt>
   0x08048b4b <+215>:	jmp    0x8048be2 <test+366>
   0x08048b50 <+220>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b53 <+223>:	mov    DWORD PTR [esp],eax
   0x08048b56 <+226>:	call   0x80489b7 <decrypt>
   0x08048b5b <+231>:	jmp    0x8048be2 <test+366>
   0x08048b60 <+236>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b63 <+239>:	mov    DWORD PTR [esp],eax
   0x08048b66 <+242>:	call   0x80489b7 <decrypt>
   0x08048b6b <+247>:	jmp    0x8048be2 <test+366>
   0x08048b6d <+249>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b70 <+252>:	mov    DWORD PTR [esp],eax
   0x08048b73 <+255>:	call   0x80489b7 <decrypt>
   0x08048b78 <+260>:	jmp    0x8048be2 <test+366>
   0x08048b7a <+262>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b7d <+265>:	mov    DWORD PTR [esp],eax
   0x08048b80 <+268>:	call   0x80489b7 <decrypt>
   0x08048b85 <+273>:	jmp    0x8048be2 <test+366>
   0x08048b87 <+275>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b8a <+278>:	mov    DWORD PTR [esp],eax
   0x08048b8d <+281>:	call   0x80489b7 <decrypt>
   0x08048b92 <+286>:	jmp    0x8048be2 <test+366>
   0x08048b94 <+288>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048b97 <+291>:	mov    DWORD PTR [esp],eax
   0x08048b9a <+294>:	call   0x80489b7 <decrypt>
   0x08048b9f <+299>:	jmp    0x8048be2 <test+366>
   0x08048ba1 <+301>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048ba4 <+304>:	mov    DWORD PTR [esp],eax
   0x08048ba7 <+307>:	call   0x80489b7 <decrypt>
   0x08048bac <+312>:	jmp    0x8048be2 <test+366>
   0x08048bae <+314>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048bb1 <+317>:	mov    DWORD PTR [esp],eax
   0x08048bb4 <+320>:	call   0x80489b7 <decrypt>
   0x08048bb9 <+325>:	jmp    0x8048be2 <test+366>
   0x08048bbb <+327>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048bbe <+330>:	mov    DWORD PTR [esp],eax
   0x08048bc1 <+333>:	call   0x80489b7 <decrypt>
   0x08048bc6 <+338>:	jmp    0x8048be2 <test+366>
   0x08048bc8 <+340>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048bcb <+343>:	mov    DWORD PTR [esp],eax
   0x08048bce <+346>:	call   0x80489b7 <decrypt>
   0x08048bd3 <+351>:	jmp    0x8048be2 <test+366>
   0x08048bd5 <+353>:	call   0x8048830 <rand@plt>
   0x08048bda <+358>:	mov    DWORD PTR [esp],eax
   0x08048bdd <+361>:	call   0x80489b7 <decrypt>
   0x08048be2 <+366>:	leave  
   0x08048be3 <+367>:	ret    
End of assembler dump.
```

Nesse sentido, investigando a função `test()`, vemos que é calculada a diferença entre a senha inputada e o valor hexadecimal em `<test+12>` e, a partir deles, é chamada uma terceira função `decrypt()` em um GIGANTESCO Switch-Case. 

Vejamos, então, a função `decrypt()`:

```x86
Dump of assembler code for function decrypt:
   0x080489b7 <+0>:	push   ebp
   0x080489b8 <+1>:	mov    ebp,esp
   0x080489ba <+3>:	sub    esp,0x38
   0x080489bd <+6>:	mov    eax,gs:0x14
   0x080489c3 <+12>:	mov    DWORD PTR [ebp-0xc],eax
   0x080489c6 <+15>:	xor    eax,eax
   0x080489c8 <+17>:	mov    DWORD PTR [ebp-0x1d],0x757c7d51
   0x080489cf <+24>:	mov    DWORD PTR [ebp-0x19],0x67667360
   0x080489d6 <+31>:	mov    DWORD PTR [ebp-0x15],0x7b66737e
   0x080489dd <+38>:	mov    DWORD PTR [ebp-0x11],0x33617c7d
   0x080489e4 <+45>:	mov    BYTE PTR [ebp-0xd],0x0
   0x080489e8 <+49>:	push   eax
   0x080489e9 <+50>:	xor    eax,eax
   0x080489eb <+52>:	je     0x80489f0 <decrypt+57>
   0x080489ed <+54>:	add    esp,0x4
   0x080489f0 <+57>:	pop    eax
   0x080489f1 <+58>:	lea    eax,[ebp-0x1d]
   0x080489f4 <+61>:	mov    DWORD PTR [esp],eax
   0x080489f7 <+64>:	call   0x8048810 <strlen@plt>
   0x080489fc <+69>:	mov    DWORD PTR [ebp-0x24],eax
   0x080489ff <+72>:	mov    DWORD PTR [ebp-0x28],0x0
   0x08048a06 <+79>:	jmp    0x8048a28 <decrypt+113>
   0x08048a08 <+81>:	lea    edx,[ebp-0x1d]
   0x08048a0b <+84>:	mov    eax,DWORD PTR [ebp-0x28]
   0x08048a0e <+87>:	add    eax,edx
   0x08048a10 <+89>:	movzx  eax,BYTE PTR [eax]
   0x08048a13 <+92>:	mov    edx,eax
   0x08048a15 <+94>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048a18 <+97>:	xor    eax,edx
   0x08048a1a <+99>:	lea    ecx,[ebp-0x1d]
   0x08048a1d <+102>:	mov    edx,DWORD PTR [ebp-0x28]
   0x08048a20 <+105>:	add    edx,ecx
   0x08048a22 <+107>:	mov    BYTE PTR [edx],al
   0x08048a24 <+109>:	add    DWORD PTR [ebp-0x28],0x1
   0x08048a28 <+113>:	mov    eax,DWORD PTR [ebp-0x28]
   0x08048a2b <+116>:	cmp    eax,DWORD PTR [ebp-0x24]
   0x08048a2e <+119>:	jb     0x8048a08 <decrypt+81>
   0x08048a30 <+121>:	mov    DWORD PTR [esp+0x4],0x8048d03
   0x08048a38 <+129>:	lea    eax,[ebp-0x1d]
   0x08048a3b <+132>:	mov    DWORD PTR [esp],eax
   0x08048a3e <+135>:	call   0x8048770 <strcmp@plt>
   0x08048a43 <+140>:	test   eax,eax
   0x08048a45 <+142>:	jne    0x8048a55 <decrypt+158>
   0x08048a47 <+144>:	mov    DWORD PTR [esp],0x8048d14
   0x08048a4e <+151>:	call   0x80487e0 <system@plt>
   0x08048a53 <+156>:	jmp    0x8048a61 <decrypt+170>
   0x08048a55 <+158>:	mov    DWORD PTR [esp],0x8048d1c
   0x08048a5c <+165>:	call   0x80487d0 <puts@plt>
   0x08048a61 <+170>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048a64 <+173>:	xor    eax,DWORD PTR gs:0x14
   0x08048a6b <+180>:	je     0x8048a72 <decrypt+187>
   0x08048a6d <+182>:	call   0x80487c0 <__stack_chk_fail@plt>
   0x08048a72 <+187>:	leave  
   0x08048a73 <+188>:	ret    
End of assembler dump.
```

Em `decrypt()`, vemos a existência de uma string dada por `0x33617c7d7b66737e67667360757c7d51` que, transformando em caracteres, resulta em 
```Q}|u\`sfg~sf{}|a3``` (tenha em mente que estamos tratando de Little Endian!).

Além disso, existe um loop onde todos os caracteres dessa string estranha são decodificados (usando XOR) baseados no valor da diferença calculado em `test()`. Portanto, devemos entender qual é o valor certo da diferença para decriptar corretamente a mensagem a fim de obter `Congratulations!` como a mensagem final! 

### Resolvendo

Dado o funcionamento do XOR, podemos realizar XOR entre a string original e a encriptada e, assim, sabemos a chave simétrica. Usando Python:

```
>>> ord("Q") ^ ord("C")
18
```

Precisamos, então, de uma diferença de 18! Como fazemos `0x1337D00D - input`, sabemos que `0x1337D00D + 0x12` é o input desejado. Assim, chegamos a `322424827` e voilà!

## Lab1A

### Análise das funções

Realizando o disassembly da `main()`, verificamos que há 2 inputs pertinentes: um usuário e um serial.

```
.---------------------------.
|---------  RPISEC  --------|
|+ SECURE LOGIN SYS v. 3.0 +|
|---------------------------|
|~- Enter your Username:  ~-|
'---------------------------'
UsernameTeste
.---------------------------.
| !! NEW ACCOUNT DETECTED !!|
|---------------------------|
|~- Input your serial:    ~-|
'---------------------------'
123456
```

```x86
Dump of assembler code for function main:
   0x08048b44 <+0>:	push   ebp
   0x08048b45 <+1>:	mov    ebp,esp
   0x08048b47 <+3>:	and    esp,0xfffffff0
   0x08048b4a <+6>:	sub    esp,0x40
   0x08048b4d <+9>:	mov    eax,DWORD PTR [ebp+0xc]
   0x08048b50 <+12>:	mov    DWORD PTR [esp+0xc],eax
   0x08048b54 <+16>:	mov    eax,gs:0x14
   0x08048b5a <+22>:	mov    DWORD PTR [esp+0x3c],eax
   0x08048b5e <+26>:	xor    eax,eax
   0x08048b60 <+28>:	push   eax
   0x08048b61 <+29>:	xor    eax,eax
   0x08048b63 <+31>:	je     0x8048b68 <main+36>
   0x08048b65 <+33>:	add    esp,0x4
   0x08048b68 <+36>:	pop    eax
   0x08048b69 <+37>:	mov    DWORD PTR [esp],0x8048d73
   0x08048b70 <+44>:	call   0x8048810 <puts@plt>
   0x08048b75 <+49>:	mov    DWORD PTR [esp],0x8048d91
   0x08048b7c <+56>:	call   0x8048810 <puts@plt>
   0x08048b81 <+61>:	mov    DWORD PTR [esp],0x8048daf
   0x08048b88 <+68>:	call   0x8048810 <puts@plt>
   0x08048b8d <+73>:	mov    DWORD PTR [esp],0x8048dcd
   0x08048b94 <+80>:	call   0x8048810 <puts@plt>
   0x08048b99 <+85>:	mov    DWORD PTR [esp],0x8048deb
   0x08048ba0 <+92>:	call   0x8048810 <puts@plt>
   0x08048ba5 <+97>:	mov    DWORD PTR [esp],0x8048e09
   0x08048bac <+104>:	call   0x8048810 <puts@plt>
   0x08048bb1 <+109>:	mov    eax,ds:0x804b060
   0x08048bb6 <+114>:	mov    DWORD PTR [esp+0x8],eax
   0x08048bba <+118>:	mov    DWORD PTR [esp+0x4],0x20
   0x08048bc2 <+126>:	lea    eax,[esp+0x1c]
   0x08048bc6 <+130>:	mov    DWORD PTR [esp],eax
   0x08048bc9 <+133>:	call   0x80487d0 <fgets@plt>
   0x08048bce <+138>:	mov    DWORD PTR [esp],0x8048d73
   0x08048bd5 <+145>:	call   0x8048810 <puts@plt>
   0x08048bda <+150>:	mov    DWORD PTR [esp],0x8048e27
   0x08048be1 <+157>:	call   0x8048810 <puts@plt>
   0x08048be6 <+162>:	mov    DWORD PTR [esp],0x8048dcd
   0x08048bed <+169>:	call   0x8048810 <puts@plt>
   0x08048bf2 <+174>:	mov    DWORD PTR [esp],0x8048e45
   0x08048bf9 <+181>:	call   0x8048810 <puts@plt>
   0x08048bfe <+186>:	mov    DWORD PTR [esp],0x8048e09
   0x08048c05 <+193>:	call   0x8048810 <puts@plt>
   0x08048c0a <+198>:	lea    eax,[esp+0x18]
   0x08048c0e <+202>:	mov    DWORD PTR [esp+0x4],eax
   0x08048c12 <+206>:	mov    DWORD PTR [esp],0x8048d00
   0x08048c19 <+213>:	call   0x8048860 <__isoc99_scanf@plt>
   0x08048c1e <+218>:	mov    eax,DWORD PTR [esp+0x18]
   0x08048c22 <+222>:	mov    DWORD PTR [esp+0x4],eax
   0x08048c26 <+226>:	lea    eax,[esp+0x1c]
   0x08048c2a <+230>:	mov    DWORD PTR [esp],eax
   0x08048c2d <+233>:	call   0x8048a0f <auth>
   0x08048c32 <+238>:	test   eax,eax
   0x08048c34 <+240>:	jne    0x8048c55 <main+273>
   0x08048c36 <+242>:	mov    DWORD PTR [esp],0x8048e63
   0x08048c3d <+249>:	call   0x8048810 <puts@plt>
   0x08048c42 <+254>:	mov    DWORD PTR [esp],0x8048e72
   0x08048c49 <+261>:	call   0x8048820 <system@plt>
   0x08048c4e <+266>:	mov    eax,0x0
   0x08048c53 <+271>:	jmp    0x8048c5a <main+278>
   0x08048c55 <+273>:	mov    eax,0x1
   0x08048c5a <+278>:	mov    edx,DWORD PTR [esp+0x3c]
   0x08048c5e <+282>:	xor    edx,DWORD PTR gs:0x14
   0x08048c65 <+289>:	je     0x8048c6c <main+296>
   0x08048c67 <+291>:	call   0x8048800 <__stack_chk_fail@plt>
   0x08048c6c <+296>:	leave  
   0x08048c6d <+297>:	ret    
End of assembler dump.
```

Todos eles são enviados para uma outra função, `auth()`. Realizando o disassembly de `auth()`:

```x86
Dump of assembler code for function auth:
   0x08048a0f <+0>:	push   ebp
   0x08048a10 <+1>:	mov    ebp,esp
   0x08048a12 <+3>:	sub    esp,0x28
   0x08048a15 <+6>:	mov    DWORD PTR [esp+0x4],0x8048d03
   0x08048a1d <+14>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048a20 <+17>:	mov    DWORD PTR [esp],eax
   0x08048a23 <+20>:	call   0x80487a0 <strcspn@plt>
   0x08048a28 <+25>:	mov    edx,DWORD PTR [ebp+0x8]
   0x08048a2b <+28>:	add    eax,edx
   0x08048a2d <+30>:	mov    BYTE PTR [eax],0x0
   0x08048a30 <+33>:	mov    DWORD PTR [esp+0x4],0x20
   0x08048a38 <+41>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048a3b <+44>:	mov    DWORD PTR [esp],eax
   0x08048a3e <+47>:	call   0x8048850 <strnlen@plt>
   0x08048a43 <+52>:	mov    DWORD PTR [ebp-0xc],eax
   0x08048a46 <+55>:	push   eax
   0x08048a47 <+56>:	xor    eax,eax
   0x08048a49 <+58>:	je     0x8048a4e <auth+63>
   0x08048a4b <+60>:	add    esp,0x4
   0x08048a4e <+63>:	pop    eax
   0x08048a4f <+64>:	cmp    DWORD PTR [ebp-0xc],0x5
   0x08048a53 <+68>:	jg     0x8048a5f <auth+80>
   0x08048a55 <+70>:	mov    eax,0x1
   0x08048a5a <+75>:	jmp    0x8048b42 <auth+307>
   0x08048a5f <+80>:	mov    DWORD PTR [esp+0xc],0x0
   0x08048a67 <+88>:	mov    DWORD PTR [esp+0x8],0x1
   0x08048a6f <+96>:	mov    DWORD PTR [esp+0x4],0x0
   0x08048a77 <+104>:	mov    DWORD PTR [esp],0x0
   0x08048a7e <+111>:	call   0x8048870 <ptrace@plt>
   0x08048a83 <+116>:	cmp    eax,0xffffffff
   0x08048a86 <+119>:	jne    0x8048ab6 <auth+167>
   0x08048a88 <+121>:	mov    DWORD PTR [esp],0x8048d08
   0x08048a8f <+128>:	call   0x8048810 <puts@plt>
   0x08048a94 <+133>:	mov    DWORD PTR [esp],0x8048d2c
   0x08048a9b <+140>:	call   0x8048810 <puts@plt>
   0x08048aa0 <+145>:	mov    DWORD PTR [esp],0x8048d50
   0x08048aa7 <+152>:	call   0x8048810 <puts@plt>
   0x08048aac <+157>:	mov    eax,0x1
   0x08048ab1 <+162>:	jmp    0x8048b42 <auth+307>
   0x08048ab6 <+167>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048ab9 <+170>:	add    eax,0x3
   0x08048abc <+173>:	movzx  eax,BYTE PTR [eax]
   0x08048abf <+176>:	movsx  eax,al
   0x08048ac2 <+179>:	xor    eax,0x1337
   0x08048ac7 <+184>:	add    eax,0x5eeded
   0x08048acc <+189>:	mov    DWORD PTR [ebp-0x10],eax
   0x08048acf <+192>:	mov    DWORD PTR [ebp-0x14],0x0
   0x08048ad6 <+199>:	jmp    0x8048b26 <auth+279>
   0x08048ad8 <+201>:	mov    edx,DWORD PTR [ebp-0x14]
   0x08048adb <+204>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048ade <+207>:	add    eax,edx
   0x08048ae0 <+209>:	movzx  eax,BYTE PTR [eax]
   0x08048ae3 <+212>:	cmp    al,0x1f
   0x08048ae5 <+214>:	jg     0x8048aee <auth+223>
   0x08048ae7 <+216>:	mov    eax,0x1
   0x08048aec <+221>:	jmp    0x8048b42 <auth+307>
   0x08048aee <+223>:	mov    edx,DWORD PTR [ebp-0x14]
   0x08048af1 <+226>:	mov    eax,DWORD PTR [ebp+0x8]
   0x08048af4 <+229>:	add    eax,edx
   0x08048af6 <+231>:	movzx  eax,BYTE PTR [eax]
   0x08048af9 <+234>:	movsx  eax,al
   0x08048afc <+237>:	xor    eax,DWORD PTR [ebp-0x10]
   0x08048aff <+240>:	mov    ecx,eax
   0x08048b01 <+242>:	mov    edx,0x88233b2b
   0x08048b06 <+247>:	mov    eax,ecx
   0x08048b08 <+249>:	mul    edx
   0x08048b0a <+251>:	mov    eax,ecx
   0x08048b0c <+253>:	sub    eax,edx
   0x08048b0e <+255>:	shr    eax,1
   0x08048b10 <+257>:	add    eax,edx
   0x08048b12 <+259>:	shr    eax,0xa
   0x08048b15 <+262>:	imul   eax,eax,0x539
   0x08048b1b <+268>:	sub    ecx,eax
   0x08048b1d <+270>:	mov    eax,ecx
   0x08048b1f <+272>:	add    DWORD PTR [ebp-0x10],eax
   0x08048b22 <+275>:	add    DWORD PTR [ebp-0x14],0x1
   0x08048b26 <+279>:	mov    eax,DWORD PTR [ebp-0x14]
   0x08048b29 <+282>:	cmp    eax,DWORD PTR [ebp-0xc]
   0x08048b2c <+285>:	jl     0x8048ad8 <auth+201>
   0x08048b2e <+287>:	mov    eax,DWORD PTR [ebp+0xc]
   0x08048b31 <+290>:	cmp    eax,DWORD PTR [ebp-0x10]
   0x08048b34 <+293>:	je     0x8048b3d <auth+302>
   0x08048b36 <+295>:	mov    eax,0x1
   0x08048b3b <+300>:	jmp    0x8048b42 <auth+307>
   0x08048b3d <+302>:	mov    eax,0x0
   0x08048b42 <+307>:	leave  
   0x08048b43 <+308>:	ret    
End of assembler dump.
```

O assembly dessa função está muito extenso. Portanto, para facilitar, utilizei o decompilador do Ghidra para entender melhor o que estava ocorrendo:


```C
/* WARNING: Removing unreachable block (ram,0x08048a4b) */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 auth(char *param_1,uint param_2)

{
  size_t sVar1;
  undefined4 uVar2;
  long lVar3;
  int local_18;
  uint local_14;
  
  sVar1 = strcspn(param_1,"\n");
  param_1[sVar1] = '\0';
  sVar1 = strnlen(param_1,0x20);
  if ((int)sVar1 < 6) {
    uVar2 = 1;
  }
  else {
    lVar3 = ptrace(PTRACE_TRACEME);
    if (lVar3 == -1) {
      puts("\x1b[32m.---------------------------.");
      puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
      puts("\x1b[32m\'---------------------------\'");
      uVar2 = 1;
    }
    else {
      local_14 = ((int)param_1[3] ^ 0x1337U) + 0x5eeded;
      for (local_18 = 0; local_18 < (int)sVar1; local_18 = local_18 + 1) {
        if (param_1[local_18] < ' ') {
          return 1;
        }
        local_14 = local_14 + ((int)param_1[local_18] ^ local_14) % 0x539;
      }
      if (param_2 == local_14) {
        uVar2 = 0;
      }
      else {
        uVar2 = 1;
      }
    }
  }
  return uVar2;
}
```

Adaptando os nomes de algumas variáveis para facilitar a leitura, resultamos em:

```C
int auth(char *username,uint serial)

{
  size_t inputLength;
  undefined4 flag;
  long ptrace?;
  int i;
  uint temp;
  
  inputLength = strcspn(username,"\n");
  username[inputLength] = '\0';
  inputLength = strnlen(username,0x20);
  if ((int)inputLength < 6) {
    flag = 1;
  }
  else {
    ptrace? = ptrace(PTRACE_TRACEME);
    if (ptrace? == -1) {
      puts("\x1b[32m.---------------------------.");
      puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
      puts("\x1b[32m\'---------------------------\'");
      flag = 1;
    }
    else {
      temp = ((int)username[3] ^ 0x1337U) + 0x5eeded;
      for (i = 0; i < (int)inputLength; i = i + 1) {
        if (username[i] < ' ') {
          return 1;
        }
        temp = temp + ((int)username[i] ^ temp) % 0x539;
      }
      if (serial == temp) {
        flag = 0;
      }
      else {
        flag = 1;
      }
    }
  }
  return flag;
}
```


Em `auth()`, podemos verificar que o programa retira, do nome do usuário, o caracter `\n` e substitui por `\0`, além de calcular e armazenar o comprimento da string. Cabe salientar que usernames com menos de 6 caracteres não são aceitos pelo programa e resultam em retorno imediato com valor 1 (que invalida a sessão). Após isso, ele faz algo em uma lógica fixa com o username e compara com o serial inputado para validar. 

### Resolução

Para resolver o problema, criei um pequeno keygen a partir da lógica exibida pelo decompilador. 

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc < 2){
		printf("1 argument required. Usage: ./solve (argument)\n");
		return -1;
	} else if (argc > 2){
        printf("Only 1 argument is allowed. Usage: ./solve (argument)\n");
        return -2
    }

	int length = strlen(argv[1]); 
    int num = (argv[1][3] ^ 0x1337) + 6221293;

	for (int i = 0; i < length; i = i + 1) {
        	if (argv[1][i] < ' ') {
        		printf("Spaces are not permitted. Exiting.\n");
			return -3;
        	}

        	num = num + (argv[1][i]^ num) % 1337;
	}

	printf("Serial: %d\n", num);
	return 0;
}
```

Ao inserir o Serial gerado pelo programa junto com o usuário, voilà! Fim de laboratório!
