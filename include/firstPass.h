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
    int isRelative1; // 1 - endereço relativo, 0 - não relativo
    char Operator2[51];
    int Operator2LocationCouter;
    int isRelative2; // 1 - endereço relativo, 0 - não relativo
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

objCode * DoFirstPass(preProcess *, symbolTable **, definitionTable **, int *);
void AddObjCode(objCode **, int, int, char *, int, int, char *, int, int, int);
void AddSymBolTable(symbolTable **, char *, int, int, int);
int SymbolTableContains(symbolTable *, char *); // Checa se a tabela de simbolos ja contem o label, retornando 1 se contem, 0 se não.
int SymbolTableContainsValue(symbolTable *, int, int); // Checa se está sendo add mais de um label em uma unica linha
void AddDefinitionTableLabel(definitionTable **, char *, int); // Adiciona apenas o label na tabela de definições, deixando o valor preenchido com a linha do label (para identificação do erro quando não se encontrar o simbolo na tabela de simbolos)
void AddDefinitionTableValue(definitionTable *, symbolTable *);
void DeleteObjCode(objCode **);
void DeleteSymbolTabel(symbolTable **);
void DeleteDefinitionTable(definitionTable **);
void isOpcode(char *, int *, int *, int *, int, int); // 'argummentsN' a quantidade de argumentos do opcode, o opcode e o tamanho da operação em memoria
int isDirective(char *, int *, int *, int *, int, int *, char *, symbolTable **); // retorna 1 se for diretiva, 0 se não. primeiro item é o numero de operandos e o segundo item o tamanho.
int isLabelDeclaration(char *); // Retorna 1 se for uma declaração de label, 0 se não
int isWhichSection(char *, int *, int, int *, int); // Verifica se é TEXT, DATA ou BSS, retornando 1 se é algo, 0 se nenhum (altera o int de acordo com o formato section)
void GetItem(preProcess *, char *, int *); // Retorna o item do programa, e verifica se é o fim da linha ou não
int DefinitionTableContains(definitionTable *, char *); // Verifica se a tabela de definições já contem o label.
