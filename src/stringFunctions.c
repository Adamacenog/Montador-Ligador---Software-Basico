/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat. 14/0090983
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

// Limpa a string por completo, colocando '\0'
void ClearString(char *string, int size)
{
  for(int i = 0; i<size; i++)
    string[i] = '\0';
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

// Compara todos os caracteres menos o ultimo, retorna 1 se for igual, 0 se for diferente
int StringCompareButEnd(char *item1, char *item2, int size1, int size2)
{
  int smallest = size1 > size2 ? size2 : size1;

  for(int i = 0; i < smallest; i++)
  {
    if(item1[i] != item2[i])
    {
      if(item1[i] == '\0' && item2[i + 1] == '\0' || item2[i] == '\0' && item1[i + 1] == '\0')
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }

  return 1;
}
