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
    preProcess *asmFile;
    asmFile = DoPreProcess(argv);

    while(asmFile->nextLine != NULL)
    {
      printf("%s\n", asmFile->Program);
      asmFile = asmFile->nextLine;
      free(asmFile->previousLine);
    }
    printf("%s\n", asmFile->Program);
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
  preProcess *asmContent = NULL, *contentCreator = NULL;
  int lineCount = 1, locationCount = 0, i=0, j=0;
  char asmFileName[100], fileItem, fileString[3][100];

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

// Limpar por completo a string fileString
for(i = 0; i < 4; i++)
  for(j = 0; j < 100; j++)
    fileString[i][j] = '\0';

// Resetar os valores de i e j
i = 0;
j = 0;

// Leitura de caracter em caracter do arquivo.
while ((fileItem = fgetc(asmFile)) != EOF)
{
  // Remoção dos comentários
  if((char) fileItem == ';')
  {
    while((fileItem = fgetc(asmFile)) != '\n' && fileItem!= EOF);
  }

  // Contador de linhas do programa e criação de um novo item da lista
  if((char) fileItem == '\n')
  {
    // Se tiver algo na string do arquivo
    if(fileString[0][0] != '\0')
    {
      // Criação da lista de itens pre-processados
      contentCreator = (preProcess *) malloc(sizeof(preProcess));
      if(asmContent == NULL)
      {
        asmContent = contentCreator;
        asmContent->nextLine = NULL;
        asmContent->previousLine = NULL;
      }
      else
      {
        contentCreator->previousLine = asmContent;
        asmContent->nextLine = contentCreator;
        asmContent = contentCreator;
        asmContent->nextLine = NULL;
      }

      asmContent->Program[0] = '\0';
      for(i = 0; i<4; i++)
      {
        if(fileString[i][0] != '\0')
        {
          strcat(asmContent->Program, fileString[i]);
          strcat(asmContent->Program, " \0");
        }
      }
      asmContent->LineCounter = lineCount;
      asmContent->LocationCounter = locationCount;
    }

    // Prox - linha
    lineCount++;

    // Limpar por completo a string fileString
    for(i = 0; i < 4; i++)
      for(j = 0; j < 100; j++)
        fileString[i][j] = '\0';

    i = 0;
    j = 0;
  }

  // Remoção de tabs, espaços e novas linhas
  if((char) fileItem != 0x20 && (char) fileItem != 0x09 && (char) fileItem != '\n')
  {
    fileString[i][j] = (char) fileItem;
    j++;
  }
  else
  {
    if(fileString[i][0] != '\0')
    {
      if(i < 4)
      {
        i++;
        locationCount++;
      }
    }
    j = 0;
  }
}

fclose(asmFile);

while(asmContent->previousLine != NULL)
  asmContent = asmContent->previousLine;

return asmContent;
}
