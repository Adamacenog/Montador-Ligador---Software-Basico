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

preProcess* DoPreProcess(char **name)
{
  FILE *asmFile;
  preProcess *asmContent = NULL;
  equTable *tableHead = NULL;
  int lineCount = 1, i = 0, removeLine = 0, wasEqu = 0, wasIf = 0, noMoreItem = 0, valueInt, twoPointsQuantity,commaQuantity, plusQuantity, minusQuantity;
  char fileItem, fileString[51], saveFile[204], *ptr;

  // Abertura do arquivo '.asm'
  asmFile = OpenAsmFile(name);

  // Limpar por completo a string fileString e saveFile
  ClearString(fileString, 51);
  ClearString(saveFile, 204);

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = toupper((char) fgetc(asmFile))) != EOF)
  {
    // Remoção dos comentários
    if(fileItem == ';')
    {
      while((fileItem = fgetc(asmFile)) != '\n' && fileItem != EOF);
    }

    // Remoção de tabs, espaços, novas linhas e 'carriage return'
    if(fileItem != 0x20 && fileItem != 0x09 && fileItem != '\n' && fileItem != 0xD)
    {
        // Checa os caracteres para caracter ilegal
        if(fileItem == 0x2D || fileItem == 0x5F || fileItem == 0x3A || fileItem == 0x2C || fileItem == 0x2B || fileItem >= 0x30 && fileItem <= 0x39 || fileItem >= 0x41 && fileItem <= 0x5A)
        {
          fileString[i] =  fileItem;
          i++;
        }
        else
        {
          printf("Erro léxico na linha: %d.\n", lineCount);
        }

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
        valueInt = strtol(fileString,&ptr,10);
        AddValueEquTable(tableHead, valueInt);
        noMoreItem = 1;

        // Caso tenha apenas letras sem numero
        if(strcmp(ptr, "") != 0 && (fileString[0] < 0x30 || fileString[0] > 0x39) || strcmp(fileString, "") == 0)
          printf("Erro sintático na linha: %d.\n", lineCount);

        wasEqu = 0;
      }

      // Caso tenha tido algum IF no codigo
      if(wasIf)
      {
        IsInEqu(tableHead, fileString, lineCount);

        valueInt = strtol(fileString,&ptr,10);

        if(valueInt != 1 && strcmp(ptr, "") == 0)
          removeLine = 2; // remove a linha do 'if' e a linha abaixo dele

        // Caso tenha apenas letras sem numero
        if(strcmp(ptr, "") != 0 && (fileString[0] < 0x30 || fileString[0] > 0x39) || strcmp(fileString, "") == 0)
          printf("Erro sintático na linha: %d.\n", lineCount);

        noMoreItem = 1;
        wasIf = 0;
      }

      if(strcmp(fileString, "") != 0)
      {
        // Caso seja encontrado um EQU
        if(strcmp(fileString, "EQU") == 0)
        {
          // Remoção de tabs, espaços
          RemoveChar(0x20, saveFile, 204, 0);
          RemoveChar(0x09, saveFile, 204, 0);

          if(StringContains(saveFile, 0x20, 204) == 0 && StringContains(saveFile, 0x09, 204) == 0)
          {
            if(AddLabelEquTable(&tableHead, saveFile, lineCount) == 1)
            {
              wasEqu = 1;
            }
            else
            {
              wasEqu = 0;
            }

            removeLine = 1;
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
          RemoveChar(0x20, saveFile, 204, 0);
          RemoveChar(0x09, saveFile, 204, 0);

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

        // Verificação final de erros léxicos (pode conter ':', ',' apenas no final e não pode ter numeros no inicio,
        // '+' devem estar separados unicamente por espaços (ficar sozinhos na string)) e o '-' deve vir acompanhado de um numero
        twoPointsQuantity = StringContains(fileString, ':', 51);
        commaQuantity = StringContains(fileString, ',', 51);
        plusQuantity = StringContains(fileString, '+', 51);
        minusQuantity = StringContains(fileString, '-', 51);
        if(fileString[0] >= 0x30 && fileString[0] <= 0x39 && fileString[1] != '\0' && (strstr(saveFile, "CONST") == NULL)
        && fileString[1] == 'X' && fileString[4] == '\0' || twoPointsQuantity > 1 || twoPointsQuantity == 1
        && StringContainsAtEnd(fileString, ':', 51) == 0 || commaQuantity > 1 || plusQuantity > 1 || minusQuantity > 1)
        {
          printf("Erro léxico na linha: %d.\n", lineCount);
        }


        IsInEqu(tableHead, fileString, lineCount);
        strcat(saveFile, fileString);
        strcat(saveFile, " ");
      }

      // Apaga toda string do fileString
      ClearString(fileString, 51);

      i = 0;
    }

    // Contador de linhas do programa e criação de um novo item da lista
    if(fileItem == '\n')
    {
      if(strcmp(saveFile, "") != 0)
      {
        // Se tiver algo na string do arquivo e a quantidade de linhas ignoradas for zero
        if(removeLine == 0)
        {
          // Identificação de erros na 'frase': Só pode ter um ':', '+', '-', ',' na frase
          twoPointsQuantity = StringContains(saveFile, ':', 204);
          commaQuantity = StringContains(saveFile, ',', 204);
          plusQuantity = StringContains(saveFile, '+', 204);
          minusQuantity = StringContains(saveFile, '-', 204);
          if(twoPointsQuantity > 1 || commaQuantity > 1 || plusQuantity > 1 || minusQuantity > 1 || plusQuantity == 1 && minusQuantity == 1)
            printf("Erro sintático na linha: %d.\n", lineCount);

          // Remoção de espaço e tabs no final da instrução
          RemoveChar(0x20, saveFile, 204, 1);
          RemoveChar(0x09, saveFile, 204, 1);
          AddPreProcess(&asmContent, saveFile, lineCount);
        }

        // Caso alguma linha tenha sido ignorada
        if(removeLine != 0)
          removeLine--; // Remove o contador de linha ignorada
      }

      // Prox - linha
      lineCount++;

      // Limpa o noMoreItem
      noMoreItem = 0;

      // Limpar por completo a string saveFile
      ClearString(saveFile, 204);
    }
  }

  // Caso o arquivo '.asm' termine e não tenha um '\n' no final do arquivo (antes de terminar) e esteja no fileString
  if(fileString[0] != '\0' && strcmp(fileString, "") != 0)
  {
    // Verificação final de erros léxicos (pode conter ':', ',' apenas no final e não pode ter numeros no inicio,
    // '+' devem estar separados unicamente por espaços (ficar sozinhos na string)) e o '-' deve vir acompanhado de um numero
    twoPointsQuantity = StringContains(fileString, ':', 51);
    commaQuantity = StringContains(fileString, ',', 51);
    plusQuantity = StringContains(fileString, '+', 51);
    minusQuantity = StringContains(fileString, '-', 51);
    if(fileString[0] >= 0x30 && fileString[0] <= 0x39 && fileString[1] != '\0' && (strstr(saveFile, "CONST") == NULL)
    && fileString[1] == 'X' && fileString[4] == '\0' || twoPointsQuantity > 1 || twoPointsQuantity == 1
    && StringContainsAtEnd(fileString, ':', 51) == 0 || commaQuantity > 1 || plusQuantity > 1 || minusQuantity > 1)
    {
      printf("Erro léxico na linha: %d.\n", lineCount);
    }

    IsInEqu(tableHead, fileString, lineCount);
    strcpy(saveFile, fileString);
  }

  // Caso o arquivo '.asm' termine e não tenha um '\n' no final do arquivo (antes de terminar) e esteja no saveFile
  if(saveFile[0] != '\0' && strcmp(saveFile, "") != 0)
  {
    // Se tiver algo na string do arquivo e a quantidade de linhas ignoradas for zero
    if(removeLine == 0)
    {
      // Identificação de erros na 'frase': Só pode ter um ':', '+', '-', ',' na frase
      twoPointsQuantity = StringContains(saveFile, ':', 204);
      commaQuantity = StringContains(saveFile, ',', 204);
      plusQuantity = StringContains(saveFile, '+', 204);
      minusQuantity = StringContains(saveFile, '-', 204);
      if(twoPointsQuantity > 1 || commaQuantity > 1 || plusQuantity > 1 || minusQuantity > 1 || plusQuantity == 1 && minusQuantity == 1)
        printf("Erro sintático na linha: %d.\n", lineCount);

      // Remoção de espaço e tabs no final da instrução
      RemoveChar(0x20, saveFile, 204, 1);
      RemoveChar(0x09, saveFile, 204, 1);
      AddPreProcess(&asmContent, saveFile, lineCount);
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

void IsInEqu(equTable *EquHead, char *item, int lineCount)
{
  while(EquHead != NULL)
  {
    if(strcmp(EquHead->Label, item) == 0)
    {
      sprintf(item, "%d", EquHead->Value);
      break;
    }

    if(StringContainsAtEnd(item, ':', 51) == 1 && StringCompareButEnd(item, EquHead->Label, 51, 51) == 1)
      printf("Erro semântico na linha: %d.\n", lineCount);

    EquHead = EquHead->nextItem;
  }
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

// Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL), inteiro serve para identificar o numero da linha com erro. Retorna 1 se foi add com sucesso, 0 caso contrario
int AddLabelEquTable(equTable **tableHead, char *saveFile, int lineCount)
{
  equTable *tableCreator = NULL, *tableAux = NULL;

  // Identificação da existência de ':' APENAS no final do label
  if(StringContains(saveFile, ':', 204) == 1 && StringContainsAtEnd(saveFile, ':', 204) == 1 && EquTableContains(*tableHead, saveFile) == 0)
  {
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

    // Remoção de ':'
    RemoveChar(':', saveFile, 204, 1);
    strcpy(tableCreator->Label, saveFile);
    return 1;
  }
  else
  {
    // Para a mensagem de erro quando não tem nenhum ':'
    if(StringContains(saveFile, ':', 204) == 0)
      printf("Erro léxico na linha: %d.\n", lineCount);

    return 0;
  }
}

// Verifica se label ja esta na tabela, se estiver, retorna 1, caso contrario 0
int EquTableContains(equTable *tableHead, char *string)
{
  while(tableHead != NULL)
  {
    if(StringCompareButEnd(tableHead->Label, string, 51, 51) == 1)
      return 1;

    tableHead = tableHead->nextItem;
  }

  return 0;
}

// Seta o valor do fim da lista EquTable
void AddValueEquTable(equTable *tableHead, int value)
{
  if(tableHead != NULL)
  {
    while(tableHead->nextItem != NULL)
      tableHead = tableHead->nextItem;

    tableHead->Value = value;
  }
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

  fclose(asmFile);
}
