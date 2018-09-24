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
  preProcess *asmContent = NULL;
  char asmFileName[100], fileItem;

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

  // Leitura de caracter em caracter do arquivo.
  while ((fileItem = fgetc(asmFile)) != EOF)
  {
    if((char) fileItem != 0x20 && (char) fileItem != 0x09)
      printf("%c",(char) fileItem);
  }

  fclose(asmFile);

  return asmContent;
}
