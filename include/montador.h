/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

typedef struct PreProcess
{
    char Program[400];
    int LineCounter;
    int LocationCounter;
    struct PreProcess *nextLine, *previousLine;
} preProcess;

typedef struct EquTable
{
  char Label[100];
  int Value;
  struct EquTable *nextItem, *previousItem;
}equTable;

preProcess* DoPreProcess(char **);
