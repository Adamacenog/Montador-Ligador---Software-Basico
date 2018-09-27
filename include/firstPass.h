/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/

typedef struct ObjCode
{
    int Opcode;
    int OpcodeLocationCouter;
    char Operator1[51];
    int Operator1LocationCouter;
    char Operator2[51];
    int Operator2LocationCouter;
    struct ObjCode *nextLine, *previousLine;
} objCode;

typedef struct SymbolTable
{
    char Label[51];
    int Value;
    int isExtern; // 1 - yes, 0 - no
    struct SymbolTable *nextItem, *previousItem;
} symbolTable;

typedef struct DefinitionTable
{
    char Label[51];
    int Value;
    struct DefinitionTable *nextItem, *previousItem;
} definitionTable;

objCode * DoFirstPass(preProcess *, symbolTable **, definitionTable **);
void AddObjCode(objCode **, int, int, char *, int, char *, int);
void AddSymBolTable(symbolTable **, char *, int, int);
void AddDefinitionTableLabel(definitionTable **, char *); // Adiciona apenas o label na tabela de definições, deixando o valor não preenchido
void AddDefinitionTableValue(definitionTable *, symbolTable *);
