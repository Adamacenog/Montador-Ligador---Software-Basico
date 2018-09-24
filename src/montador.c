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
  preProcess *asmContent = NULL, *contentAux = NULL, *contentCreator = NULL;
  int lineCount = 1, preProcessItem = 0, i=0;
  char asmFileName[100], fileItem, fileString[100];

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

// Criação da lista de itens pre-processados
asmContent = (preProcess *) malloc(sizeof(preProcess));
asmContent->nextLine = NULL;
asmContent->previousLine = NULL;
contentAux = asmContent;

// Limpar por completo a string fileString
for(i = 0; i < 100; i++)
  fileString[i] = '\0';

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
    lineCount++;
    preProcessItem = 0;
  }

  // Remoção de tabs e espaços
  if((char) fileItem != 0x20 && (char) fileItem != 0x09 && (char) fileItem != '\n')
  {
    fileString[i] = (char) fileItem;
    i++;
  }
  else
  {
    if(fileString[0] != '\0')
    {
      printf("%s\n", fileString);

      // Opcode, diretiva ou label
      if(preProcessItem == 0)
      {

      }
      // opt1
      else if(preProcessItem == 1)
      {

        preProcessItem++;
      }
      // opt2
      else if(preProcessItem == 2)
      {

      }
    }

    // Limpar por completo a string fileString
    for(i = 0; i < 100; i++)
      fileString[i] = '\0';

    i = 0;
  }
}

  fclose(asmFile);

  return asmContent;
}
