/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

typedef struct PreProcess
{
    char Program[204];
    int LineCounter;
    struct PreProcess *nextLine, *previousLine;
} preProcess;

typedef struct EquTable
{
  char Label[51];
  int Value;
  struct EquTable *nextItem, *previousItem;
}equTable;

preProcess* DoPreProcess(char **);
FILE * OpenAsmFile(char **);
void IsInEqu(equTable*, char *); // Verifica se esta na tabela Equ, se estiver, bota o valor do item da tabela na string passada.
void AddPreProcess(preProcess**, char *, int); // Adiciona ao fim da lista PreProcess (ou cria a lista caso seja NULL)
void DeletePreProcess(preProcess**); // Deleta toda a lista PreProcess
void AddLabelEquTable(equTable**, char *, int); // Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL), inteiro serve para identificar o numero da linha com erro.
void AddValueEquTable(equTable *, int); // Seta o valor do fim da lista EquTable
void DeleteEquTable(equTable**); // Deleta toda a lista EquTable
void PrintPreProcess(preProcess*, char**); // Imprime todo conteudo da lista preProcess em um arquivo nome.pre
