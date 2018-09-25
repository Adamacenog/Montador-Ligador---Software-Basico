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

int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    preProcess *asmFile;
    asmFile = DoPreProcess(argv);

    while(asmFile->nextLine != NULL)
    {
      printf("%s : LineCounter: %d, LocationCounter: %d\n", asmFile->Program, asmFile->LineCounter, asmFile->LocationCounter);
      asmFile = asmFile->nextLine;
      free(asmFile->previousLine);
    }
    printf("%s : LineCounter: %d, LocationCounter: %d\n", asmFile->Program, asmFile->LineCounter, asmFile->LocationCounter);
    free(asmFile);
  }
  else
  {
    printf("ERRO: Um arquivo '.asm' é esperado.\n");
  }

  return 0;
}

preProcess* DoPreProcess(char **name)
{
  FILE *asmFile;
  preProcess *asmContent = NULL;
  equTable *tableEqu = NULL, *tableCreator = NULL, *tableAux = NULL, *tableHead = NULL;
  int lineCount = 1, locationCount = 0, i=0, removeLine = 0, wasEqu = 0, wasIf = 0;
  char asmFileName[100], fileItem, fileString[100], saveFile[400];

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

// Limpar por completo a string fileString e saveFile
for(i = 0; i < 100; i++)
{
  fileString[i] = '\0';
  saveFile[i] = '\0';
}

for(i = 100; i < 400; i++)
  saveFile[i] = '\0';

// Resetar os valores de i
i = 0;

// Leitura de caracter em caracter do arquivo.
while ((fileItem = fgetc(asmFile)) != EOF)
{
  // Bota todos os caracteres em maiúsculo
  fileItem = toupper(fileItem);

  // Remoção dos comentários
  if(fileItem == ';')
  {
    while((fileItem = fgetc(asmFile)) != '\n' && fileItem!= EOF);
  }

  // Remoção de tabs, espaços e novas linhas
  if(fileItem != 0x20 && fileItem != 0x09 && fileItem != '\n' && fileItem != ':')
  {
      fileString[i] =  fileItem;
      i++;
  }
  else
  {
    // Caso tenha tido algum EQU no codigo
    if(wasEqu == 1)
    {
      char *ptr;
      tableEqu->Value = strtol(fileString,&ptr,10);
      wasEqu = 0;
    }

    // Caso tenha tido algum IF no codigo
    if(wasIf == 1)
    {
      char *ptr;
      IsInEqu(tableHead, fileString);
      if(strtol(fileString,&ptr,10) != 1)
        removeLine = 2; // remove a linha do 'if' e a linha abaixo dele
      wasIf = 0;
    }

    if(strcmp(fileString, "") != 0)
    {
      if(strcmp(fileString, "EQU") == 0)
      {
        removeLine = 1;

        // Criação da tabela de itens 'equ'
        tableCreator = (equTable *) malloc(sizeof(equTable));

        if(tableEqu == NULL)
        {
          tableEqu = tableCreator;
          tableHead = tableEqu;
          tableEqu->nextItem = NULL;
          tableEqu->previousItem = NULL;
        }
        else
        {
          tableCreator->nextItem = NULL;
          tableCreator->previousItem = tableEqu;
          tableEqu->nextItem = tableCreator;
          tableEqu = tableCreator;
        }

        // Remoção de tabs e espaços
        RemoveChar(0x20, saveFile);
        RemoveChar(0x09, saveFile);
        strcpy(tableEqu->Label, saveFile);
        wasEqu = 1;
      }

      if(strcmp(fileString, "IF") == 0)
      {
        wasIf = 1;
        removeLine = 1;
      }

      locationCount++;
      IsInEqu(tableHead, fileString);
      strcat(saveFile, fileString);
      strcat(saveFile, " ");
    }

    // Contador de linhas do programa e criação de um novo item da lista
    if(fileItem == '\n')
    {
      // Se tiver algo na string do arquivo e a quantidade de linhas ignoradas for zero
      if(saveFile[0] != '\0' && removeLine == 0)
      {
        AddPreProcess(&asmContent, saveFile, lineCount, locationCount);
      }

      // Caso alguma linha tenha sido ignorada
      if(removeLine != 0)
        removeLine--; // Remove o contador de linha ignorada

      // Prox - linha
      lineCount++;

      // Limpar por completo a string saveFile
      for(i = 0; i < 400; i++)
        saveFile[i] = '\0';
    }

    // Apaga toda string do fileString
    for(i=0; i<100; i++)
      fileString[i] = '\0';

    i = 0;
  }
}

tableAux = tableHead;
while(tableAux != NULL)
{
  printf("Label: %s, Value: %d\n", tableAux->Label, tableAux->Value);
  tableAux = tableAux->nextItem;
}
printf("\n");

fclose(asmFile);

PrintPreProcess(asmContent, name);

return asmContent;
}

void IsInEqu(equTable *EquHead, char *item)
{
  equTable *tableAux;
  tableAux = EquHead;

  while(tableAux != NULL)
  {
    if(strcmp(tableAux->Label, item) == 0)
    {
      sprintf(item, "%d", tableAux->Value);
      break;
    }

    tableAux = tableAux->nextItem;
  }
}

void RemoveChar(char removeChar, char *item)
{
  int i;
  if(item[0] == removeChar)
  {
    for(i = 0; i<98; i++)
    {
      item[i] = item[i + 1];
    }
  }
  else
  {
    for(i = 99; i>0; i--)
    {
      if(item[i] == removeChar)
        item[i] = '\0';
    }
  }
}

// Adiciona ao fim da lista PreProcess (ou cria a lista caso seja NULL)
void AddPreProcess(preProcess **preProcessHead, char *saveFile, int lineCount, int locationCount)
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
  contentCreator->LocationCounter = locationCount;
}

// Deleta toda a lista PreProcess
/*void DeletaPreProcess(preProcess**)
{

}

// Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL)
void AddEquTable(equTable**, char *, int)
{

}

// Deleta toda a lista EquTable
void DeletaEquTable(equTable**)
{

}*/

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
