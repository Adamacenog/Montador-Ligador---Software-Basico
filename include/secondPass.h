typedef struct ConstTable
{
    int value;
    struct ConstTable *nextItem;
} constTable;

void DoSecondPass(char*, objCode* , symbolTable* , definitionTable*, int);
int findSymbol(symbolTable*, char*);
int findDefinition(definitionTable*, char*);
int evaluateNum(int, objCode*);
int evaluate(char*, symbolTable*, objCode*);
void locateError(objCode*, objCode*, symbolTable*, constTable*);
constTable* addConstTable(objCode*);
void deleteConstTable(constTable*);
char findIlegalJump(char* operator, symbolTable *symbols, objCode* codeList);
char findIlegalJumpNum(int address, objCode* codeList);
int isSpace(char*, symbolTable *, objCode*);
int isSpaceNum(int , objCode*);
