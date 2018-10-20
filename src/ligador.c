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
#endif

#ifndef _Ligador_Library
  #define _Ligador_Library
    #include <ligador.h>
#endif

int main(int argc, char** argv)
{
  int i=0, offset=0, k =0, number=0, offsetAux[5], size, flag;
  FILE *object, *output;
  char aux[50], auxChar[50], *dump, lable[50];
  codeTable *code=NULL;
  definitionTable *definition=NULL;
  useTable *use=NULL;
  relativeTable *relative=NULL;
  if(argc > 1 )
  {
    offsetAux[0] = 0;
    strcpy(aux, argv[1]);
    size = strlen(aux);
    aux[size] = '.';
    aux[size+1] = 'e';
    aux[size+2] = '\0';
    output = fopen(aux, "w");
    i = 1;
    while(i<argc)
    {
      strcpy(aux,argv[i]);
      size = strlen(aux);
      aux[size] = '.';
      aux[size+1] = 'o';
      aux[size+2] = 'b';
      aux[size+3] = 'j';
      aux[size+4] = '\0';
      object=fopen(aux, "r");
      if(object == NULL)
      {
        printf("Arqivo Invalido\n");
      }
      fscanf(object, "%c",&auxChar[0]);
  if(auxChar[0]<58 && auxChar[0]>47 )
  {
    while(!feof(object) && auxChar[0]!='\n')
    {
      fprintf(output, "%c", auxChar[0]);
      fscanf(object, "%c",&auxChar[0]);
    }
  }else
  {
      rewind(object);
      fscanf(object, "%s",auxChar);
      while(1)
      {
        if(!strcmp(auxChar,"TABLE") )
        {
          fscanf(object, "%s",auxChar);
          if(!strcmp(auxChar,"USE"))
          {
            flag=0;
            while(!(!strcmp(auxChar,"TABLE")))
            {
              if(flag==0)
              {
                fscanf(object, "%s",auxChar);
              }
              strcpy(lable, auxChar);
              fscanf(object, "%s",auxChar);
              number = strtol(auxChar,&dump, 10);
              if(use == NULL)
              {
                use = addUse(use, lable, number+offsetAux[i-1]);
              }else
              {
                addUse(use, lable, number+offsetAux[i-1]);
              }
              fscanf(object, "%s",auxChar);
              flag=1;

            }
          }else if(!strcmp(auxChar,"DEFINITION"))
          {
            flag=0;
            while(!(!strcmp(auxChar,"RELATIVE")))
            {
              if(flag==0)
              {
                fscanf(object, "%s",auxChar);
              }
              strcpy(lable, auxChar);
              fscanf(object, "%s",auxChar);
              number = strtol(auxChar,&dump, 10);
              if(definition==NULL)
              {
                definition=addDefinition(definition, lable, number+offsetAux[i-1]);
              }else
              {
                addDefinition(definition, lable, number+offsetAux[i-1]);
              }
              fscanf(object, "%s",auxChar);
              flag=1;
            }
          }
        }else if(!strcmp(auxChar,"RELATIVE"))
        {
          flag=0;
          while(!(!strcmp(auxChar,"CODE")))
          {
            if(flag==0)
            {
              fscanf(object, "%s",auxChar);
            }
            number = strtol(auxChar,&dump, 10);
            if(relative==NULL)
            {
              relative = addRelative(relative, number+offsetAux[i-1]);
            }else
            {
               addRelative(relative, number+offsetAux[i-1]);
            }
            fscanf(object, "%s",auxChar);
            flag=1;

          }
        }else if(!strcmp(auxChar,"CODE"))
        {
          offset=0;
          while(!feof(object))
          {
            fscanf(object, "%s",auxChar);
            if(strcmp(auxChar, "\n") != 0)
            {
              number = strtol(auxChar,&dump, 10);
              if(code==NULL)
              {
                code = addCodeObj(code, number, offsetAux[i-1]+offset);
              }else
              {
                addCodeObj(code, number, offsetAux[i-1]+offset);
              }
              offset++;
            }
            else
            {
              break;
            }
          }
          offsetAux[i] = offset+offsetAux[i-1];
          break;
        }
  }

  }
      fclose(object);
      i++;
    }
    makeRelative(code, relative, offsetAux);
    flag = swapDefInUse(use, definition, code);
    if(flag ==0)
    {
        fclose(output);
        dropCodeObj(code);
        dropUse(use);
        dropRelative(relative);
        dropDefinition(definition);
        exit(1);
    }
    printEx(code, output);
    fclose(output);
    dropCodeObj(code);
    dropUse(use);
    dropRelative(relative);
    dropDefinition(definition);
  }
    return 0;
  }

  codeTable* addCodeObj(codeTable* tab, int value, int address)
  {
    codeTable *aux;
    aux = malloc(sizeof(codeTable));
    aux->nextItem = NULL;
    aux->Value= value;
    aux->address=address;
    if(tab==NULL)
    {
      return aux;
    }else
    {
        while(tab->nextItem!=NULL)
        {
          tab=tab->nextItem;
        }
        tab->nextItem = aux;
  }
};
definitionTable* addDefinition(definitionTable* tab, char* lable, int address)
{
  definitionTable *aux;
  aux = malloc(sizeof(definitionTable));
  aux->nextItem = NULL;
  aux->address=address;
  strcpy(aux->lable, lable);
  if(tab==NULL)
  {
    return aux;
  }else
  {
      while(tab->nextItem!=NULL)
      {
        tab=tab->nextItem;
      }
      tab->nextItem = aux;
  }
};
useTable* addUse(useTable* tab, char* lable, int address)
{
  useTable *aux;
  aux = malloc(sizeof(useTable));
  aux->nextItem = NULL;
  aux->address=address;
  strcpy(aux->lable, lable);
  if(tab==NULL)
  {
    return aux;
  }else
  {
    while(tab->nextItem!=NULL)
    {
      tab=tab->nextItem;
    }
    tab->nextItem = aux;
  }
};
relativeTable* addRelative(relativeTable* tab, int address)
{
  relativeTable *aux;
  aux = malloc(sizeof(relativeTable));
  aux->nextItem = NULL;
  aux->address=address;
  if(tab==NULL)
  {
    return aux;
  }else
  {
    while(tab->nextItem!=NULL)
    {
      tab=tab->nextItem;
    }
    tab->nextItem = aux;
  }
};
void dropCodeObj(codeTable* tab)
{
  codeTable *aux;
  while(tab!= NULL)
  {
    aux = tab->nextItem;
    free(tab);
    tab=aux;
  }
};
void dropDefinition(definitionTable* tab)
{
  definitionTable *aux;
  while(tab!= NULL)
  {
    aux = tab->nextItem;
    free(tab);
    tab=aux;
  }
};
void dropUse(useTable* tab)
{
  useTable *aux;
  while(tab!= NULL)
  {
    aux = tab->nextItem;
    free(tab);
    tab=aux;
  }
};
void dropRelative(relativeTable*tab)
{
  relativeTable *aux;
  while(tab!= NULL)
  {
    aux = tab->nextItem;
    free(tab);
    tab=aux;
  }
};
int swapDefInUse(useTable* use, definitionTable* definition, codeTable* code)
{
  definitionTable *aux = definition;
  codeTable *auxCode = code;
  int flag = 0;
  while(use != NULL)
  {
    flag = 0;
    aux=definition;
    while(aux != NULL && strcmp(use->lable, aux->lable))
    {
      aux=aux->nextItem; //acha a lable utilizada na tabela de definições
    }
    if(aux!=NULL)
    {
      flag=1;
    }
    auxCode=code;
    while(auxCode->address != use->address)
    {
      auxCode = auxCode->nextItem; //acha o endereço de use na tabla code
    }
    if(flag==1)
    {
      auxCode->Value = aux->address; // substitui o valor pelo valor do endereço na tabela definition
    }else
    {
      return 0;
    }
    use=use->nextItem;//para todos os uses
  }
  return 1;
};
void makeRelative(codeTable* code, relativeTable* relative, int* offset)
{
  codeTable *auxCode=code;
  int i = 0;
  while(relative != NULL)
  {
    while(auxCode->address != relative->address)
    {
      auxCode = auxCode->nextItem; //acha o endereço de relative na tabla code
    }
    for(i=0;i<4;i++) // descobre qual é o offset
    {
      if(auxCode->address>=offset[i] && auxCode->address<offset[i+1])
      {
        auxCode->Value += offset[i]; //seta o offset
        break;
      }
    }
    relative = relative->nextItem;//para todo relative
  }
};

void printEx(codeTable* tab, FILE* output)
{
  int i = 0;
  while(tab!= NULL)
  {
    if(tab->address == i)
    {
      fprintf(output, "%d ", tab->Value);
      i++;
    }
    tab=tab->nextItem;
  }
};
