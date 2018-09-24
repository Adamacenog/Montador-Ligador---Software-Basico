/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

typedef struct PreProcess
{
    char Label[100];
    char Opcode[8];
    char Directive[8];
    char Op1[100];
    char Op2[100];
    int LineCounter;
    int LocationCounter;
    struct PreProcess *nextLine, *previousLine;
} preProcess;

typedef struct EquTable
{
  char Label[100];
  int Value;
}equTable;

preProcess* DoPreProcess(char **);
