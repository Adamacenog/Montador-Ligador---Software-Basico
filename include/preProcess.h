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
FILE * OpenAsmFile(char **);
void ClearString(char *, int); // Limpa a string por completo botando '\0' em todos itens, de acordo com seu tamanho
void IsInEqu(equTable*, char *); // Verifica se esta na tabela Equ, se estiver, bota o valor do item da tabela na string passada.
void RemoveChar(char, char *, int, int); // Remove apenas o primeiro e o ultimo elemento encontrado em 'removeChar', ou apenas o ultimo elemento se especificado pelo ultimo int com 1 (0 para inicio ou fim)
void AddPreProcess(preProcess**, char *, int); // Adiciona ao fim da lista PreProcess (ou cria a lista caso seja NULL)
void DeletePreProcess(preProcess**); // Deleta toda a lista PreProcess
void AddLabelEquTable(equTable**, char *, int); // Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL), inteiro serve para identificar o numero da linha com erro.
void AddValueEquTable(equTable *, int); // Seta o valor do fim da lista EquTable
void DeleteEquTable(equTable**); // Deleta toda a lista EquTable
void PrintPreProcess(preProcess*, char**); // Imprime todo conteudo da lista preProcess em um arquivo nome.pre
int StringContains(char *, char, int); // Verifica a quantidade de vezes que um item aparece na string
int StringContainsAtEnd(char *, char, int); // Verifica se a string contem um caracter na sua ultima posição preenchida (retorna 1 se tiver, 0 se não)
