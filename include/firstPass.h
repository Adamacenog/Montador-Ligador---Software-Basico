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
    int LineCounter;
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
void AddObjCode(objCode **, int, int, char *, int, char *, int, int);
void AddSymBolTable(symbolTable **, char *, int, int);
void AddDefinitionTableLabel(definitionTable **, char *); // Adiciona apenas o label na tabela de definições, deixando o valor não preenchido
void AddDefinitionTableValue(definitionTable *, symbolTable *);
void DeleteObjCode(objCode **);
void DeleteSymbolTabel(symbolTable **);
void DeleteDefinitionTable(definitionTable **);
int isOpcode(char *, int *, int *, int *, int, int); // Retorna 1 se é opcode, 0 se não (de acordo com o char passado), bota tambem no 'argummentsN' a quantidade de argumentos do opcode, o opcode e o tamanho da operação em memoria
int isDirective(char *, int *, int *, int *, int, int *); // retorna 1 se for diretiva, 0 se não. primeiro item é o numero de operandos e o segundo item o tamanho.
int isLabelDeclaration(char *); // Retorna 1 se for uma declaração de label, 0 se não
int isWhichSection(char *, int *, int, int); // Verifica se é TEXT, DATA ou BSS, retornando 1 se é algo, 0 se nenhum (altera o int de acordo com o formato section)
void GetItem(preProcess *, char *, int *); // Retorna o item do programa, e verifica se é o fim da linha ou não
