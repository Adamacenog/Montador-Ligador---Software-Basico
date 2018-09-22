/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

#ifndef _Primary_libraries
  #define _Primary_libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
#endif

#ifndef _Montador_library
  #define _Montador_library
    #include "montador.h"
#endif

int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    printf("O arquivo '.asm' fornecido foi: %s\n", argv[1]);
  }
  else if(argc > 2)
  {
    printf("ERRO: Muitos arquivos passados, é possível montar apenas um arquivo '.asm' por vez.\n");
  }
  else
  {
    printf("ERRO: Ao menos um arquivo '.asm' é esperado.\n");
  }

  return 0;
}
