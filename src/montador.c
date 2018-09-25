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
    #include <ctype.h>
#endif

#ifndef _Montador_library
  #define _Montador_library
    #include "montador.h"
#endif

#ifndef _PreProcess_library
  #define _PreProcess_library
    #include "preProcess.h"
#endif

int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    preProcess *asmFile, *aux;
    asmFile = DoPreProcess(argv);

    aux = asmFile;
    printf("\n");
    while(aux != NULL)
    {
      printf("%s : LineCounter: %d\n", aux->Program, aux->LineCounter);
      aux = aux->nextLine;
    }

    DeletePreProcess(&asmFile);
  }
  else
  {
    printf("ERRO: Um arquivo '.asm' é esperado.\n");
  }

  return 0;
}
