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

#ifndef _FirstPass_library
  #define _FirstPass_library
    #include "firstPass.h"
#endif

#ifndef _SecondPass_library
  #define _SecondPass_library
    #include "secondPass.h"
#endif

void DoSecondPass(char* argv, objCode* codes, symbolTable* symbols, definitionTable* definitions, int isModule)
{
  FILE  *saida;
  char line[500], lineOld[500], instruction[50], backuplabel[51], *dump, flag2=0, flag=0, label[50];
  int number=0, number2=0, i=0,j=0, k=0;
  int *printVector;
  objCode* aux;
  constTable *constList;

  constList = addConstTable(codes);
  strcpy(line,argv);
  saida = fopen( strcat(line,".obj"),"w");

  if(isModule) //se é módulo
  {
    symbolTable *aux3;
    aux3 = symbols;
    fprintf(saida,"TABLE USE\n");

    while(aux3 != NULL)
    {
      if(aux3->isExtern == 1)
      {
        aux = codes;
        while(aux != NULL)
        {
          CopyString(aux->Operator1, backuplabel, 51);
          GetLabelFromSum(aux->Operator1, 51);
          if(strcmp(aux->Operator1,aux3->Label) == 0)
          {
            fprintf(saida,"%s %d\n", aux3->Label, aux->Operator1LocationCouter);
          }
          CopyString(backuplabel, aux->Operator1, 51);
          CopyString(aux->Operator2, backuplabel, 51);
          GetLabelFromSum(aux->Operator1, 51);
          if(strcmp(aux->Operator2,aux3->Label)==0)
          {
            fprintf(saida,"%s %d\n", aux3->Label, aux->Operator2LocationCouter);
          }
          CopyString(backuplabel, aux->Operator2, 51);
          aux = aux->nextLine;
        }
      }
      aux3 = aux3->nextItem;
    }

    definitionTable *aux4;
    aux4 = definitions;
    fprintf(saida,"TABLE DEFINITION\n");

    while(aux4 != NULL)
    {
      fprintf(saida,"%s %d\n", aux4->Label, aux4->Value);
      aux4 = aux4->nextItem;
    }

    aux = codes;
    i=0;
    fprintf(saida,"RELATIVE\n");

    while(aux!= NULL)
    {
      if(aux->isRelative1 == 1)
      {
        if(i == 1)
          fprintf(saida, "%c", 0x20);

        fprintf(saida, "%d", aux->Operator1LocationCouter);
        i = 1;
      }

      if(aux->isRelative2 == 1 )
      {
        if(i == 1)
          fprintf(saida, "%c", 0x20);

        fprintf(saida, "%d", aux->Operator2LocationCouter);
        i = 1;
      }
      aux = aux->nextLine;
    }
    fprintf(saida,"\nCODE\n");
  }

  i = 0;
  aux = codes;

  while (aux != NULL)
  {
    locateError(codes, aux, symbols, constList);
    if(aux->Opcode == 0 && aux->isRelative1 == 0) // é space
    {
        for(i = 0; i < strtol(aux->Operator1,&dump,10); i++)
        {
          fprintf(saida, "0");

          if(aux->nextLine != NULL)
            fprintf(saida, "%c", 0x20);
        }
    }
    else if(aux->Opcode == -1 && aux->isRelative1 == 0) // é const
    {
      fprintf(saida, "%s",aux->Operator1);

      if(aux->nextLine != NULL)
        fprintf(saida, "%c", 0x20);
    }
    else
    {
      number = strtol(aux->Operator1,&dump,10);
      if(strstr(dump, "+") != NULL)
      {
        i=0;

        do
        {
          label[i]=dump[i];
          i++;
        } while(dump[i] !='+');

        label[i] = '\0';
        i++;
        j=i;

        do
        {
          line[i-j]=dump[i];
          i++;
        } while(dump[i] !='\0');

        number2 = strtol(line,&dump,10);
        flag2 = 1;
      }

      if(aux->Opcode != -1)
      {
        fprintf(saida, "%d ", aux->Opcode);
      }

      if(number != -1)
      {
        if(dump[0]=='\0')
        {
          if(aux->Operator1[0] != '\0')
            fprintf(saida, "%s ",aux->Operator1);
        }
        else
        {
          if(flag2==0)
          {
            flag = findSymbol(symbols, aux->Operator1);

            if(flag >= 0)
            {
              fprintf(saida, "%d ", flag);
            }
            else
            {
              printf("Erro semântico na linha: %d.\n", aux->LineCounter);
            }
          }
          else if(flag2 == 1)
          {
            flag = findSymbol(symbols, label);

            if(flag >= 0)
            {
              fprintf(saida, "%d ", flag+number2);
            }
            else
            {
              printf("Erro semântico na linha: %d.\n", aux->LineCounter);
            }
          }
        }
      }

      number = strtol(aux->Operator2,&dump,10);
      flag2 = 0;

      if(strstr(dump, "+") > 0)
      {
        int j=0;
        i=0;

        do
        {
          label[i]=dump[i];
          i++;
        } while(dump[i] !='+');

        label[i] = '\0';
        i++;
        j=i;

        do
        {
          line[i-j]=dump[i];
          i++;
        } while(dump[i] !='\0');

        number2 = strtol(line,&dump,10);
        flag2 = 2;
      }

      if(number != -1)
      {
        if(dump[0]=='\0')
        {
          if(aux->Operator2[0] != '\0')
            fprintf(saida, "%s ",aux->Operator2);
        }
        else
        {
          if(flag2==0)
          {
            flag = findSymbol(symbols, aux->Operator2);

            if(flag >= 0)
            {
              fprintf(saida, "%d ", flag);
            }
            else
            {
              flag = findDefinition(definitions, aux->Operator2);

              if(flag >= 0)
              {
                fprintf(saida, "%d ", flag);
              }
              else
              {
                printf("Erro semântico na linha: %d.\n", aux->LineCounter);
              }
            }
          }
          else if(flag2 == 2)
          {
            flag = findSymbol(symbols, label);
            if(flag >= 0)
            {
              fprintf(saida, "%d ", flag+number2);
            }
            else
            {
              printf("Erro semântico na linha: %d.\n", aux->LineCounter);
            }
          }
        }
      }
    }
    aux = aux->nextLine;
  }
  deleteConstTable(constList);
  fclose(saida);
}

int findSymbol(symbolTable *symbols, char* Lable)
{
  symbolTable *aux;

  aux = symbols;
  while(aux != NULL)
  {
    if(!strcmp(aux->Label, Lable))
      return aux->Value;

    aux = aux->nextItem;
  }
  return -1;
}

int findDefinition(definitionTable *definitions, char* Lable)
{
  definitionTable *aux;

  aux = definitions;
  while(aux != NULL)
  {
    if(!strcmp(aux->Label, Lable))
      return aux->Value;

    aux = aux->nextItem;
  }
  return -1;
}

void locateError(objCode* codeList, objCode* code, symbolTable *symbols, constTable *constList)
{
  int number=0, number2=0, i=0, j=0;
  char flag=0, flag2=0, label[100], *dump, line[100];
  constTable *constAux = constList;

  switch(code->Opcode)
  {
    case 4://divisão por zero
    if(strstr(code->Operator1, "+") != NULL)
    {
      number = strtol(code->Operator1,&dump,10);
      do
      {
        label[i]=dump[i];
        i++;
      } while(dump[i] !='+');

      label[i] = '\0';
      i++;
      j=i;

      do
      {
        line[i-j]=dump[i];
        i++;
      } while(dump[i] !='\0');

      number2 = strtol(line,&dump,10);
      if(!evaluateNum(findSymbol(symbols,label)+number2, codeList))
      {
        printf("Erro semântico na linha: %d.\n", code->LineCounter);
      }
    }else
    {
      if(code->Operator1[0] == '0' || !evaluate( code->Operator1, symbols, codeList))
      {
        printf("Erro semântico na linha: %d.\n", code->LineCounter);
      }
    }
      break;

    case 5:
    case 6:
    case 7:
    case 8://erros em jumps
      flag = findSymbol(symbols, code->Operator1);
      if(flag >= 0)
      {
         if(!findIlegalJump(code->Operator1, symbols, codeList))
          printf("Erro semântico na linha: %d.\n", code->LineCounter);
      }
      else
      {
        if(!findIlegalJumpNum(strtol(code->Operator1, &dump,10), codeList))
          printf("Erro semântico na linha: %d.\n", code->LineCounter);
      }

      break;
      case 9:  // Copy (operator 1 pode ser const ou space, operator 2 só pode ser space)
        if(!isSymbolExtern(symbols, code->Operator1))
        {
          flag = findSymbol(symbols, code->Operator1);
          if(flag >= 0)
          {
             if(findIlegalJump(code->Operator1, symbols, codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
          else
          {
            if(findIlegalJumpNum(strtol(code->Operator1, &dump,10), codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
        }

        if(!isSymbolExtern(symbols, code->Operator2))
        {
          flag = findSymbol(symbols, code->Operator2);
          if(flag >= 0)
          {
             if(!isSpace(code->Operator2, symbols, codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
          else
          {
            if(!isSpaceNum(strtol(code->Operator2, &dump,10), codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
        }
        break;

      case 11://alterar const
        if(!isSymbolExtern(symbols, code->Operator1))
        {
          flag = findSymbol(symbols, code->Operator1);
          if(flag >= 0)
          {
             if(!isSpace(code->Operator1, symbols, codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
          else
          {
            if(!isSpaceNum(strtol(code->Operator1, &dump,10), codeList))
              printf("Erro semântico na linha: %d.\n", code->LineCounter);
          }
        }
        break;
  }
}

int evaluate(char* operator, symbolTable *symbols, objCode* codeList)
{
  int address = 0;
  objCode* code = codeList;
  address = findSymbol(symbols, operator); //acha o endereçonce

  if(address != -1)
  {
    while(code != NULL)
    {
      if(code->Operator1LocationCouter == address && code->Opcode == -1 && code->Operator1[0] == '0')
        return 0;

      code = code->nextLine;
    }
    return 1;
  }
}

int evaluateNum(int address, objCode* codeList)
{
  objCode* code = codeList;

  while(code != NULL)
  {
    if(code->Operator1LocationCouter == address && code->Opcode == -1)
      return 0;

    code = code->nextLine;
  }
  return 1;
}

constTable* addConstTable(objCode* codes)
{
  objCode *aux = codes;
  constTable *previous = NULL, *next = NULL, *head;
  while(aux != NULL)
  {
    if(aux->Opcode == -1 && aux->isRelative1 == 0)//é const
    {
      next = (constTable*) malloc(sizeof(constTable));
      next->value = aux->Operator1LocationCouter;
      next->nextItem = NULL;
      if(previous == NULL)
      {
        previous = next;
        head = previous;
      }else
      {
        previous->nextItem = next;
        previous = next;
      }
    }
    aux = aux->nextLine;
  }
  return previous;
}
void deleteConstTable(constTable* table)
{
  constTable *aux, *aux2;
  aux = table;
  while(aux != NULL)
  {
    aux2 = aux->nextItem;
    if(aux!=NULL)
    free(aux);
    aux = aux2;
  }
}

char findIlegalJump(char* operator, symbolTable *symbols, objCode* codeList)
{
  int address = 0;
  objCode* code = codeList;
  address = findSymbol(symbols, operator); //acha o endereçonce

  if(address != -1)
  {
    while(code != NULL)
    {
      if(code->Operator1LocationCouter == address && (code->Opcode == -1 || code->Opcode == 0))
        return 0;

      code = code->nextLine;
    }
    return 1;
  }
}

int isSpace(char* operator, symbolTable *symbols, objCode* codeList)
{
  int address = 0;
  objCode* code = codeList;
  address = findSymbol(symbols, operator); //acha o endereçonce

  if(address != -1)
  {
    while(code != NULL)
    {
      if(code->Operator1LocationCouter == address && code->Opcode == 0)
        return 1;

      code = code->nextLine;
    }
    return 0;
  }
}

int isSpaceNum(int address, objCode* codeList)
{
  objCode* code = codeList;

  while(code != NULL)
  {
    if(code->Operator1LocationCouter == address && code->Opcode == 0)
      return 1;

    code = code->nextLine;
  }
  return 0;
}

int isSymbolExtern(symbolTable *symbols, char* Lable)
{
  symbolTable *aux;
  char backupLabel[51], *ptr;
  int number;

  CopyString(Lable, backupLabel, 51);
  if(StringContains(Lable, '+', 51))
    GetLabelFromSum(Lable, 51);

  aux = symbols;
  while(aux != NULL)
  {
    if(!strcmp(aux->Label, Lable) && aux->isExtern)
    {
      CopyString(backupLabel, Lable, 51);
      return 1;
    }

    aux = aux->nextItem;
  }

  CopyString(backupLabel, Lable, 51);

  return 0;
}

char findIlegalJumpNum(int address, objCode* codeList)
{
  objCode* code = codeList;

  while(code != NULL)
  {
    if(code->Operator1LocationCouter == address && (code->Opcode == -1 || code->Opcode == 0))
      return 0;

    code = code->nextLine;
  }
  return 1;
}
