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
  char item[51];
  // Section = -1: indefinido,  0 - TEXT, 1 - DATA, 2 - BSS | isModule - 1: tem Begin, 0 - : não tem Begin. Marca o inicio e fim do 'text' do modulo
  int lineCounter = 0, section = -1, isModule = 0, isEndOfLine = 0;

  if(preProcessHead != NULL)
  {
    while(preProcessHead != NULL)
    {
      GetItem(preProcessHead, item, &isEndOfLine);
      printf("Program: |%s|, Item: |%s|, isEndOfLine: %d\n", preProcessHead->Program, item, isEndOfLine);

      if(isEndOfLine)
      {
        preProcessHead = preProcessHead->nextLine;
        isEndOfLine = 0;
      }
    }
  }
  else
  {
    printf("Erro: Programa vazio.\n");
  }

  return objCodeHead;
}

void AddObjCode(objCode **objCodeHead, int Opcode, int OpcodeLocationCouter, char *Operator1, int Operator1LocationCouter, char *Operator2, int Operator2LocationCouter, int LineCounter)
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
  objCodeCreator->LineCounter = LineCounter;
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

void DeleteObjCode(objCode **objCodeHead)
{
  objCode *aux;

  while(*objCodeHead != NULL)
  {
    aux = *objCodeHead;
    *objCodeHead = (*objCodeHead)->nextLine;
    free(aux);
  }
}

void DeleteSymbolTabel(symbolTable **symbolTableHead)
{
  symbolTable *aux;

  while(*symbolTableHead != NULL)
  {
    aux = *symbolTableHead;
    *symbolTableHead = (*symbolTableHead)->nextItem;
    free(aux);
  }
}

void DeleteDefinitionTable(definitionTable **definitionTableHead)
{
  definitionTable *aux;

  while(*definitionTableHead != NULL)
  {
    aux = *definitionTableHead;
    *definitionTableHead = (*definitionTableHead)->nextItem;
    free(aux);
  }
}

// Retorna 1 se é opcode, 0 se não (de acordo com o char passado), bota tambem no 'argummentsN' a quantidade de argumentos do opcode, o opcode e o tamanho da operação em memoria
int isOpcode(char *operation, int *argummentsN, int *Opcode, int *size, int section, int lineCounter)
{
  if(section != 0)
    printf("Erro semântico na linha: %d.\n", lineCounter);

  if(strcmp(operation, "ADD") == 0)
  {
    *argummentsN = 1;
    *Opcode = 1;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "SUB") == 0)
  {
    *argummentsN = 1;
    *Opcode = 2;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "MULT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 3;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "DIV") == 0)
  {
    *argummentsN = 1;
    *Opcode = 4;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "JMP") == 0)
  {
    *argummentsN = 1;
    *Opcode = 5;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "JMPN") == 0)
  {
    *argummentsN = 1;
    *Opcode = 6;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "JMPP") == 0)
  {
    *argummentsN = 1;
    *Opcode = 7;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "JMPZ") == 0)
  {
    *argummentsN = 1;
    *Opcode = 8;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "COPY") == 0)
  {
    *argummentsN = 2;
    *Opcode = 9;
    *size = 3;
    return 1;
  }
  else if(strcmp(operation, "LOAD") == 0)
  {
    *argummentsN = 1;
    *Opcode = 10;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "STORE") == 0)
  {
    *argummentsN = 1;
    *Opcode = 11;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "INPUT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 12;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "OUTPUT") == 0)
  {
    *argummentsN = 1;
    *Opcode = 13;
    *size = 2;
    return 1;
  }
  else if(strcmp(operation, "STOP") == 0)
  {
    *argummentsN = 0;
    *Opcode = 14;
    *size = 1;
    return 1;
  }
  else
  {
    return 0;
  }
}

// retorna 1 se for diretiva, 0 se não. primeiro item é o numero de operandos e o segundo item o tamanho.
int isDirective(char *directive, int *argummentsN, int *size, int *section, int lineCounter, int *isModule)
{
  if(strcmp(directive, "SECTION") == 0)
  {
    *argummentsN = 1;
    *size = 0;
    return 1;
  }
  else if(strcmp(directive, "SPACE") == 0)
  {
    *argummentsN = 1;
    *size = 1;

    if(*section != 2)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(directive, "CONST") == 0)
  {
    *argummentsN = 1;
    *size = 1;

    if(*section != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(directive, "PUBLIC") == 0)
  {
    *argummentsN = 0;
    *size = 0;

    if(*section != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(directive, "EXTERN") == 0)
  {
    *argummentsN = 0;
    *size = 0;

    if(*section != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(directive, "BEGIN") == 0)
  {
    *argummentsN = 0;
    *size = 0;
    *isModule = 1;

    if(*section != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(directive, "END") == 0)
  {
    *argummentsN = 0;
    *size = 0;

    if(*section != 0 || *isModule != 1)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    *section = -1;
    *isModule = 0;

    return 1;
  }
  else
  {
    return 0;
  }
}

// Retorna 1 se for uma declaração de label, 0 se não
int isLabelDeclaration(char *Label)
{
  return StringContainsAtEnd(Label, ':', 51);
}

// Verifica se é TEXT, DATA ou BSS, retornando 1 se é algo, 0 se nenhum (altera o int de acordo com o formato section)
int isWhichSection(char *item, int *section, int isModule, int lineCounter)
{
  if(strcmp(item, "TEXT") == 0)
  {
    *section = 1;
    return 1;
  }
  else if(strcmp(item, "DATA") == 0)
  {
    *section = 2;

    if(isModule != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else if(strcmp(item, "BSS") == 0)
  {
    *section = 3;

    if(isModule != 0)
      printf("Erro semântico na linha: %d.\n", lineCounter);

    return 1;
  }
  else
  {
    return 0;
  }
}

// Retorna o item do programa, e verifica se é o fim da linha ou não
void GetItem(preProcess *preProcessLine, char *item, int *isEndOfLine)
{
  int i, j;
  char vector[204];

  ClearString(item, 51);
  ClearString(vector, 204);

  for(i = 0; i < 51; i++)
  {
    if(preProcessLine->Program[i] != 0x20 && preProcessLine->Program[i] != '\0')
    {
      item[i] = preProcessLine->Program[i];
    }
    else
    {
      if(preProcessLine->Program[i] == '\0')
        *isEndOfLine = 1;

      break;
    }
  }

  i += 1;
  for(j = 0; i < 204; j++, i++)
    vector[j] = preProcessLine->Program[i];

  ClearString(preProcessLine->Program, 204);
  strcpy(preProcessLine->Program, vector);
}
