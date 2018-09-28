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

#ifndef _PreProcess_library
  #define _PreProcess_library
    #include "preProcess.h"
#endif

#ifndef _FirstPass_library
  #define _FirstPass_library
    #include "firstPass.h"
#endif

int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    preProcess *asmFile = NULL;
    objCode *objCodeHead = NULL;
    symbolTable *symbolTableHead = NULL;
    definitionTable *definitionTableHead = NULL;
    int isModule = 0;

    asmFile = DoPreProcess(argv);
    // preProcess *aux;
    // aux = asmFile;
    // printf("\n");
    // while(aux != NULL)
    // {
    //   printf("%s : LineCounter: %d\n", aux->Program, aux->LineCounter);
    //   aux = aux->nextLine;
    // }
    objCodeHead = DoFirstPass(asmFile, &symbolTableHead, &definitionTableHead, &isModule);
    DeletePreProcess(&asmFile);
    DeleteObjCode(&objCodeHead);
    DeleteSymbolTabel(&symbolTableHead);
    DeleteDefinitionTable(&definitionTableHead);
  }
  else
  {
    printf("ERRO: Um arquivo '.asm' é esperado.\n");
  }

  return 0;
}
