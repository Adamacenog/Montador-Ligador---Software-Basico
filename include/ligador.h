/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat.
*/



typedef struct UseTable
{
    char lable[50];
    struct UseTable *nextItem;
} useTable;

int address;
typedef struct DefinitionTable
{
    int address;
    char lable[50];
    struct DefinitionTable *nextItem;
} definitionTable;

typedef struct CodeTable
{
    int Value;
    int address;
    struct CodeTable *nextItem;
} codeTable;

typedef struct RelativeTable
{
    int address;
    struct RelativeTable *nextItem;
} relativeTable;

void addCodeObj(codeTable* tab, int value, int address);
void addDefinition(definitionTable* tab, char* lable, int address);
void addUse(useTable* tab, char* lable, int address);
void addRelative(relativeTable*tab, int address);
void dropCodeObj(codeTable* tab);
void dropDefinition(definitionTable* tab);
void dropUse(useTable* tab);
void dropRelative(relativeTable*tab);
void swapDefInUse(useTable* use, definitionTable* definition, codeTable* code);
void makeRelative(codeTable* code, relativeTable* relative, int* offset);
void printEx(codeTable* tab, FILE* output);
