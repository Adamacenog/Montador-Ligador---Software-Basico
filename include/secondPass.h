

void secondPass(char*, objCode* , symbolTable* , definitionTable*);
int findSymbol(symbolTable*, char*);
int findDefinition(definitionTable*, char*);
int evaluateNum(int address, objCode* codeList);
int evaluate(char* operator, symbolTable *symbols, objCode* codeList);
