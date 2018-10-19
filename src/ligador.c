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

#ifndef _Ligador_Library
  #define _Ligador_Library
    #include <ligador.h>
#endif

int main()
{
  int i=0, offset=0, k =0, number=0, offsetAux[5];
  FILE *object, *output;
  char aux[50], auxChar[50], *dump, lable[50];
  codeTable *code=NULL;
  definitionTable *definition=NULL;
  useTable *use=NULL;
  relativeTable *relative=NULL;

  offsetAux[0] = 0;
  strcpy(aux, argv[1]);
  aux[strlen(aux)+1] = '.';
  aux[strlen(aux)+2] = 'e';
  aux[strlen(aux)+3] = '\0';
  output = fopen(aux, "w");
  for(i=1;i++;i<argc)
  {
    strcpy(aux,argv[i]);
    aux[strlen(aux)+1] = '.';
    aux[strlen(aux)+2] = 'e';
    aux[strlen(aux)+3] = '\0';
    object=fopen(aux, "r");
    fscanf(object, "%c",&auxChar);
if(auxChar<58 && auxChar>47 )
{
  while(!feof(object) || auxChar=='\n')
  {
    fprintf(output, "%c", auxChar);
    fscanf(object, "%c",&auxChar);
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
          while(strcmp(auxChar,"TABLE"))
          {
            fscanf(object, "%s",auxChar);
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
          }
        }else if(!strcmp(auxChar,"DEFINITION"))
        {
          while(strcmp(auxChar,"RELATIVE"))
          {
            fscanf(object, "%s",auxChar);
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
          }
        }
      }else if(!strcmp(auxChar,"RELATIVE"))
      {
        while(strcmp(auxChar,"CODES"))
        {
          fscanf(object, "%s",auxChar);
          number = strtol(auxChar,&dump, 10);
          if(relative==NULL)
          {
            relative = addRelative(relative, number+offsetAux[i-1]);
          }else
          {
             addRelative(relative, number+offsetAux[i-1]);
          }
        }
      }else if(!strcmp(auxChar,"CODES"))
      {
        offset=0;
        while(!feof(object))
        {
          fscanf(object, "%s",auxChar);
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
        offsetAux[i] = offset;
      }
}

}
    fclose(object);
  }
  swapDefInUse(use, definition, code);
  makeRelative(code, relative, offsetAux);
  printEx(code, output);
  fclose(output);
  dropCodeObj(code);
  dropUse(use);
  dropRelative(relative);
  dropDefinition(definition);
  return 0;
}

codeTable* addCodeObj(codeTable* tab, int value, int address)
{
  codeTable *aux;
  aux = malloc(sizeof(codeTable));
  aux->nextItem = NULL;
  aux->value= value;
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
relativeTable* addRelative(relativeTable* tab, int address);
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
void dropRelative(relativeTable*tab);
{
  relativeTable *aux;
  while(tab!= NULL)
  {
    aux = tab->nextItem;
    free(tab);
    tab=aux;
  }
};
void swapDefInUse(useTable* use, definitionTable* definition, codeTable* code)
{
  definitionTable *aux = definition;
  codeTable *auxCode=code;
  while(use != NULL)
  {
    aux=definition;
    while(strcmp(use->lable, aux->lable))
    {
      aux=aux->nextItem; //acha a lable utilizada na tabela de definições
    }
    auxCode=code
    while(auxCode->address != use->address)
    {
      auxcode = auxCode->nextItem; //acha o endereço de use na tabla code
    }
    auxCode->value = aux->address; // substitui o valor pelo valor do endereço na tabela definition
    use=use->nextItem;//para todos os uses
  }
};
void makeRelative(codeTable* code, relativeTable* relative, int* offset)
{
  codeTable *auxCode=code;
  while(relative != NULL)
  {
    while(auxCode->address != relative->address)
    {
      auxcode = auxCode->nextItem; //acha o endereço de relative na tabla code
    }
    for(i=0;i<4;i++) // descobre qual é o offset
    {
      if(auxCode->address>offset[i] && auxCode->address<offset[i+1])
      {
        auxCode->value += offset[i]; //seta o offset
      }
    }
    relative = relative->nextItem;//para todo relative
  }
};

void printEx(codeTable* tab, FILE* output)
{
  while(tab!= NULL)
  {
    fprintf(output, "%d ", tab->value);
    tab=tab->nextItem;
  }
};
