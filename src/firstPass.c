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

#ifndef _StringFunctions_library
  #define _StringFunctions_library
    #include "stringFunctions.h"
#endif

#ifndef _FirstPass_library
  #define _FirstPass_library
    #include "firstPass.h"
#endif

objCode * DoFirstPass(preProcess *preProcessHead, symbolTable **symbolTableHead, definitionTable **definitionTableHead)
{
  objCode *objCodeHead = NULL;

  return objCodeHead;
}

void AddObjCode(objCode **objCodeHead, int Opcode, int OpcodeLocationCouter, char *Operator1, int Operator1LocationCouter, char *Operator2, int Operator2LocationCouter)
{
  objCode *objCodeCreator, *lastElem;

  // Alocação da memória para o objCode
  objCodeCreator = (objCode *) malloc(sizeof(objCode));

  if(*objCodeHead == NULL)
  {
    (*objCodeHead) = objCodeCreator;
    (*objCodeHead)->nextLine = NULL;
    (*objCodeHead)->previousLine = NULL;
  }
  else
  {
    lastElem = *objCodeHead;

    while(lastElem->nextLine != NULL)
      lastElem = lastElem->nextLine;

    objCodeCreator->previousLine = lastElem;
    lastElem->nextLine = objCodeCreator;
    objCodeCreator->nextLine = NULL;
  }

  // Copia os dados para o novo item do objCode
  objCodeCreator->Opcode = Opcode;
  objCodeCreator->OpcodeLocationCouter = OpcodeLocationCouter;
  strcpy(objCodeCreator->Operator1, Operator1);
  objCodeCreator->Operator1LocationCouter = Operator1LocationCouter;
  strcpy(objCodeCreator->Operator2, Operator2);
  objCodeCreator->Operator2LocationCouter = Operator2LocationCouter;
}

void AddSymBolTable(symbolTable **symbolTableHead, char *Label, int Value, int isExtern)
{
  symbolTable *symbolTableCreator, *lastElem;

  // Alocação da memória para o symbolTable
  symbolTableCreator = (symbolTable *) malloc(sizeof(symbolTable));

  if(*symbolTableHead == NULL)
  {
    (*symbolTableHead) = symbolTableCreator;
    (*symbolTableHead)->nextItem = NULL;
    (*symbolTableHead)->previousItem = NULL;
  }
  else
  {
    lastElem = *symbolTableHead;

    while(lastElem->nextItem != NULL)
      lastElem = lastElem->nextItem;

    symbolTableCreator->previousItem = lastElem;
    lastElem->nextItem = symbolTableCreator;
    symbolTableCreator->nextItem = NULL;
  }

  // Copia os dados para o novo item do symbolTable
  strcpy(symbolTableCreator->Label, Label);
  symbolTableCreator->Value = Value;
  symbolTableCreator->isExtern = isExtern;
}

// Adiciona apenas o label na tabela de definições, deixando o valor não preenchido
void AddDefinitionTableLabel(definitionTable **definitionTableHead, char *Label)
{
  definitionTable *definitionTableCreator, *lastElem;

  // Alocação da memória para o definitionTable
  definitionTableCreator = (definitionTable *) malloc(sizeof(definitionTable));

  if(*definitionTableHead == NULL)
  {
    (*definitionTableHead) = definitionTableCreator;
    (*definitionTableHead)->nextItem = NULL;
    (*definitionTableHead)->previousItem = NULL;
  }
  else
  {
    lastElem = *definitionTableHead;

    while(lastElem->nextItem != NULL)
      lastElem = lastElem->nextItem;

    definitionTableCreator->previousItem = lastElem;
    lastElem->nextItem = definitionTableCreator;
    definitionTableCreator->nextItem = NULL;
  }

  // Copia os dados para o novo item do definitionTable
  strcpy(definitionTableCreator->Label, Label);
}

// Adiciona o valor dos itens definidos na tabela de simbolos na tabela de definições que ainda estão sem valor.
void AddDefinitionTableValue(definitionTable *definitionTableHead, symbolTable *symbolTableHead)
{
  symbolTable *auxSymbolTable;

  while(definitionTableHead->nextItem != NULL)
  {
    auxSymbolTable = symbolTableHead;
    while(auxSymbolTable->nextItem != NULL)
    {
      if(strcmp(auxSymbolTable->Label, definitionTableHead->Label) == 0)
      {
        definitionTableHead->Value = auxSymbolTable->Value;
        break;
      }
      auxSymbolTable = auxSymbolTable->nextItem;
    }
    definitionTableHead = definitionTableHead->nextItem;
  }
}
