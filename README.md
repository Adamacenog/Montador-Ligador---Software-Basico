# Montador-Ligador---Software-Basico
Trabalho 1 da matéria de Software Básico da UnB.

Alunos:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat. 14/0090983


Como compilar:

* Crie a pasta ./Montador-Ligador---Software-Basico/src/obj
* Abra ./Montador-Ligador---Software-Basico/src/ no terminal
* Digite 'make' isso irá compilar o projeto inteiro (montador e ligador)
* Para compilar apenas o montador, digite 'make montador'
* Para compilar apenas o ligador, digite 'make ligador'

*O programa é compilado usando o padrão -std=c99 do gcc.

Ambiente:
Ubuntu 16.04.5 LTS - Release: 16.04, versão gcc: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609

---------------------------------------------------------------------------
*OBSERVAÇÃO:

*O separador de linhas usado para analisar os arquivos '.asm' é o '\n'
Portanto, caso esteja usando MAC OS 9 ou alguma versão de compatibilidade
do MAC que use '\r' como nova linha, o montador não funcionará corretamente.

*Não há problemas no uso de '\r\n' (comumente usado em windows)
