/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

typedef struct PreProcess
{
    char Program[400];
    int LineCounter;
    struct PreProcess *nextLine, *previousLine;
} preProcess;

typedef struct EquTable
{
  char Label[100];
  int Value;
  struct EquTable *nextItem, *previousItem;
}equTable;

preProcess* DoPreProcess(char **);
void IsInEqu(equTable*, char *);
void RemoveChar(char, char *);
void AddPreProcess(preProcess**, char *, int); // Adiciona ao fim da lista PreProcess (ou cria a lista caso seja NULL)
void DeletaPreProcess(preProcess**); // Deleta toda a lista PreProcess
void AddLabelEquTable(equTable**, char *); // Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL)
void AddValueEquTable(equTable *, int); // Seta o valor do fim da lista EquTable
void DeletaEquTable(equTable**); // Deleta toda a lista EquTable
void PrintPreProcess(preProcess*, char**); // Imprime todo conteudo da lista preProcess em um arquivo nome.pre
