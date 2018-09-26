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

preProcess* DoPreProcess(char **name)
{
  FILE *asmFile;
  preProcess *asmContent = NULL;
  equTable *tableHead = NULL;
  int lineCount = 1, i = 0, removeLine = 0, wasEqu = 0, wasIf = 0, noMoreItem = 0;
  char fileItem, fileString[100], saveFile[400];

  // Abertura do arquivo '.asm'
  asmFile = OpenAsmFile(name);

  // Limpar por completo a string fileString e saveFile
  ClearString(fileString, 100);
  ClearString(saveFile, 400);

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = toupper((char) fgetc(asmFile))) != EOF)
  {
    // Remoção dos comentários
    if(fileItem == ';')
    {
      while((fileItem = fgetc(asmFile)) != '\n' && fileItem != EOF);
    }

    // Remoção de tabs, espaços e novas linhas
    if(fileItem != 0x20 && fileItem != 0x09 && fileItem != '\n')
    {
        fileString[i] =  fileItem;
        i++;

        // Quando setado em 1, não aceita nenhum caracter a mais (casos após valores do IF e EQU)
        if(noMoreItem == 1)
        {
          printf("Erro sintático na linha: %d.\n", lineCount);
          noMoreItem = 0;
        }
    }
    else
    {
      // Caso tenha tido algum EQU no codigo
      if(wasEqu)
      {
        char *ptr;

        if(strcmp(fileString, "") == 0)
          printf("Erro sintático na linha: %d.\n", lineCount);

        int value = strtol(fileString,&ptr,10);

        AddValueEquTable(tableHead, value);
        noMoreItem = 1;

        // Caso tenha algo junto ao numero (Ex: 1bla)
        if(strcmp(ptr, "") != 0)
          printf("Erro léxico na linha: %d.\n", lineCount);

        wasEqu = 0;
      }

      // Caso tenha tido algum IF no codigo
      if(wasIf)
      {
        char *ptr;

        if(strcmp(fileString, "") == 0)
          printf("Erro sintático na linha: %d.\n", lineCount);

        IsInEqu(tableHead, fileString);

        if(strtol(fileString,&ptr,10) != 1 && strcmp(ptr, "") == 0)
          removeLine = 2; // remove a linha do 'if' e a linha abaixo dele

        if(strcmp(ptr, "") != 0)
        {
          printf("Erro léxico na linha: %d.\n", lineCount);
          removeLine = 0;
        }

        noMoreItem = 1;
        wasIf = 0;
      }

      if(strcmp(fileString, "") != 0)
      {
        // Caso seja encontrado um EQU
        if(strcmp(fileString, "EQU") == 0)
        {
          // Remoção de tabs, espaços
          RemoveChar(0x20, saveFile, 400, 0);
          RemoveChar(0x09, saveFile, 400, 0);

          if(StringContains(saveFile, 0x20, 400) == 0 && StringContains(saveFile, 0x09, 400) == 0)
          {
            AddLabelEquTable(&tableHead, saveFile, lineCount);
            removeLine = 1;
            wasEqu = 1;
          }
          else
          {
            printf("Erro sintático na linha: %d.\n", lineCount);
          }
        }

        // Caso seja encontrado um 'IF'
        if(strcmp(fileString, "IF") == 0)
        {
          // Remoção de tabs, espaços antes do IF
          RemoveChar(0x20, saveFile, 400, 0);
          RemoveChar(0x09, saveFile, 400, 0);

          // Não pode conter labels ou qualquer coisa antes do IF
          if(strcmp(saveFile, "") == 0)
          {
            wasIf = 1;
            removeLine = 1;
          }
          else
          {
            printf("Erro sintático na linha: %d.\n", lineCount);
          }
        }

        IsInEqu(tableHead, fileString);
        strcat(saveFile, fileString);
        strcat(saveFile, " ");
      }

      // Apaga toda string do fileString
      ClearString(fileString, 100);

      i = 0;
    }

    // Contador de linhas do programa e criação de um novo item da lista
    if(fileItem == '\n' && strcmp(saveFile, "") != 0)
    {
      // Se tiver algo na string do arquivo e a quantidade de linhas ignoradas for zero
      if(removeLine == 0)
      {
        // Remoção de espaço e tabs no final da instrução
        RemoveChar(0x20, saveFile, 400, 1);
        RemoveChar(0x09, saveFile, 400, 1);
        AddPreProcess(&asmContent, saveFile, lineCount);
      }

      // Caso alguma linha tenha sido ignorada
      if(removeLine != 0)
        removeLine--; // Remove o contador de linha ignorada

      // Prox - linha
      lineCount++;

      // Limpa o noMoreItem
      noMoreItem = 0;

      // Limpar por completo a string saveFile
      ClearString(saveFile, 400);
    }
  }

  fclose(asmFile);
  PrintPreProcess(asmContent, name);
  DeleteEquTable(&tableHead);

  return asmContent;
}

FILE * OpenAsmFile(char **name)
{
  FILE *asmFile;
  char asmFileName[100];

  // Adicionando o '.asm' no nome do arquivo
  strcpy(asmFileName,name[1]);
  strcat(asmFileName,".asm");

  // Abertura do arquivo '.asm'
  asmFile = fopen(asmFileName,"r");
  if(asmFile == NULL)
  {
    printf("ERRO: Arquivo não encontrado\n");
    exit(1);
  }
  return asmFile;
}

void ClearString(char *string, int size)
{
  for(int i = 0; i<size; i++)
    string[i] = '\0';
}

void IsInEqu(equTable *EquHead, char *item)
{
  while(EquHead != NULL)
  {
    if(strcmp(EquHead->Label, item) == 0)
    {
      sprintf(item, "%d", EquHead->Value);
      break;
    }

    EquHead = EquHead->nextItem;
  }
}

// Remove apenas o char 'removeChar' que estiver no inicio e no fim da string, ou apenas checa no final caso lastOnly seja maior ou menor que 0
void RemoveChar(char removeChar, char *item, int size, int lastOnly)
{
  int i;
  if(item[0] == removeChar && lastOnly == 0)
  {
    for(i = 0; i<(size - 2); i++)
    {
      item[i] = item[i + 1];
    }
  }

  for(i = (size -2); i > 0; i--)
  {
    if(item[i] == removeChar && item[i+1] == '\0')
    {
      item[i] = '\0';
      break;
    }
  }

  if(item[(size -1)] == removeChar)
    item[(size -1)] = '\0';
}

// Adiciona ao fim da lista PreProcess (ou cria a lista caso seja NULL)
void AddPreProcess(preProcess **preProcessHead, char *saveFile, int lineCount)
{
  preProcess *contentCreator, *lastElem;

  // Criação da lista de itens pre-processados
  contentCreator = (preProcess *) malloc(sizeof(preProcess));
  if(*preProcessHead == NULL)
  {
    *preProcessHead = contentCreator;
    (*preProcessHead)->nextLine = NULL;
    (*preProcessHead)->previousLine = NULL;
  }
  else
  {
    lastElem = *preProcessHead;

    while(lastElem->nextLine != NULL)
      lastElem = lastElem->nextLine;

    contentCreator->previousLine = lastElem;
    lastElem->nextLine = contentCreator;
    contentCreator->nextLine = NULL;
  }

  strcpy(contentCreator->Program, saveFile);
  contentCreator->LineCounter = lineCount;
}

// Deleta toda a lista PreProcess
void DeletePreProcess(preProcess **preProcessHead)
{
  preProcess *aux;

  while(*preProcessHead != NULL)
  {
    aux = *preProcessHead;
    *preProcessHead = (*preProcessHead)->nextLine;
    free(aux);
  }
}

// Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL), inteiro serve para identificar o numero da linha com erro.
void AddLabelEquTable(equTable **tableHead, char *saveFile, int lineCount)
{
  equTable *tableCreator = NULL, *tableAux = NULL;

  // Criação da tabela de itens 'equ'
  tableCreator = (equTable *) malloc(sizeof(equTable));

  if(*tableHead == NULL)
  {
    *tableHead = tableCreator;
    tableCreator->nextItem = NULL;
    tableCreator->previousItem = NULL;
  }
  else
  {
    tableAux = (*tableHead);
    while(tableAux->nextItem != NULL)
      tableAux = tableAux->nextItem;

    tableCreator->nextItem = NULL;
    tableCreator->previousItem = tableAux;
    tableAux->nextItem = tableCreator;
  }

  // Identificação da existência de ':' APENAS no final do label
  if(StringContains(saveFile, ':', 400) != 1 || StringContainsAtEnd(saveFile, ':', 400) == 0)
    printf("Erro léxico na linha: %d.\n", lineCount);

  // Remoção de ':'
  RemoveChar(':', saveFile, 400, 1);
  strcpy(tableCreator->Label, saveFile);
}

// Seta o valor do fim da lista EquTable
void AddValueEquTable(equTable *tableHead, int value)
{
  while(tableHead->nextItem != NULL)
    tableHead = tableHead->nextItem;

  tableHead->Value = value;
}

// Deleta toda a lista EquTable
void DeleteEquTable(equTable **tableHead)
{
  equTable *aux;

  while(*tableHead != NULL)
  {
    aux = *tableHead;
    *tableHead = (*tableHead)->nextItem;
    free(aux);
  }
}

// Imprime todo conteudo da lista preProcess em um arquivo nome.pre
void PrintPreProcess(preProcess *preProcessHead, char **name)
{
  char asmFileName[100];
  FILE * asmFile;

  // Adicionando o '.asm' no nome do arquivo
  strcpy(asmFileName,name[1]);
  strcat(asmFileName,".pre");

  // Abertura do arquivo '.asm'
  asmFile = fopen(asmFileName,"w");
  if(asmFile == NULL)
  {
    printf("ERRO: Arquivo não criado\n");
    exit(1);
  }

  while(preProcessHead != NULL)
  {
    fprintf(asmFile, "%s\n", preProcessHead->Program);
    preProcessHead = preProcessHead->nextLine;
  }
}

// Verifica a quantidade de vezes que um item aparece na string
int StringContains(char *string, char item, int size)
{
  int quantity = 0;
  for(int i = 0; i < (size-1); i++)
    if(string[i] == item)
      quantity += 1;

  return quantity;
}

// Verifica se a string contem um caracter na sua ultima posição preenchida (retorna 1 se tiver, 0 se não)
int StringContainsAtEnd(char *string, char item, int size)
{
  if(string[size-1] == item)
    return 1;

  for(int i = (size-2); i > 0; i--)
    if(string[i] == item && string[i + 1] == '\0')
      return 1;

  return 0;
}
