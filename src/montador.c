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

    // int Opcode;
    // int OpcodeLocationCouter;
    // char Operator1[51];
    // int Operator1LocationCouter;
    // int isRelative1; // 1 - endereço relativo, 0 - não relativo
    // char Operator2[51];
    // int Operator2LocationCouter;
    // int isRelative2; // 1 - endereço relativo, 0 - não relativo
    // int LineCounter;
    DeletePreProcess(&asmFile);

    objCode *aux2;
    aux2 = objCodeHead;
    printf("\nCodigo objeto:\n");
    while(aux2 != NULL)
    {
      printf("Opcode: %d, LC: %d, Opt1: %s, Opt1LC: %d, Rel1: %d, Opt2: %s, Opt2LC: %d, Rel2: %d, LineCounter: %d\n", aux2->Opcode,
      aux2->OpcodeLocationCouter, aux2->Operator1, aux2->Operator1LocationCouter, aux2->isRelative1, aux2->Operator2,
      aux2->Operator2LocationCouter, aux2->isRelative2, aux2->LineCounter);
      aux2 = aux2->nextLine;
    }

    symbolTable *aux3;
    aux3 = symbolTableHead;
    printf("\nTabela de simbolos:\n");
    while(aux3 != NULL)
    {
      printf("Label: %s, Value: %d, Esterno: %d\n", aux3->Label, aux3->Value, aux3->isExtern);
      aux3 = aux3->nextItem;
    }

    definitionTable *aux4;
    aux4 = definitionTableHead;
    printf("\nTabela de definições:\n");
    while(aux4 != NULL)
    {
      printf("Label: %s, Value: %d\n", aux4->Label, aux4->Value);
      aux4 = aux4->nextItem;
    }

    printf("\n");

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
